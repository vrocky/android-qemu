// Copyright 2017 The Android Open Source Project
//
// This software is licensed under the terms of the GNU General Public
// License version 2, as published by the Free Software Foundation, and
// may be copied, distributed, and modified under those terms.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "android/snapshot/RamLoader.h"

#include "android/avd/info.h"
#include "android/base/ArraySize.h"
#include "android/base/ContiguousRangeMapper.h"
#include "android/base/EintrWrapper.h"
#include "android/base/files/MemStream.h"
#include "android/base/files/preadwrite.h"
#include "android/base/memory/MemoryHints.h"
#include "android/base/Profiler.h"
#include "android/base/Stopwatch.h"
#include "android/snapshot/Compressor.h"
#include "android/snapshot/Decompressor.h"
#include "android/utils/debug.h"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <memory>

using android::base::ContiguousRangeMapper;
using android::base::ScopedMemoryProfiler;
using android::base::MemStream;
using android::base::Stopwatch;

namespace android {
namespace snapshot {

void RamLoader::FileIndex::clear() {
    decltype(pages)().swap(pages);
    decltype(blocks)().swap(blocks);
}

RamLoader::RamLoader(base::StdioStream&& stream,
                     bool indexOnly,
                     const RamLoader::RamBlockStructure& blockStructure)
    : mStream(std::move(stream)),
      mReaderThread([this]() { readerWorker(); }),
      mIndexOnly(indexOnly) {

    if (mIndexOnly) {
        applyRamBlockStructure(blockStructure);
        readIndex();
        mStream.close();
        return;
    }

    if (MemoryAccessWatch::isSupported()) {
        mAccessWatch.emplace([this](void* ptr) { loadRamPage(ptr); },
                             [this]() { return backgroundPageLoad(); });
        if (mAccessWatch->valid()) {
            mOnDemandEnabled = true;
        } else {
            derror("Failed to initialize memory access watcher, falling back "
                   "to synchronous RAM loading");
            mAccessWatch.clear();
        }
    }
}

RamLoader::~RamLoader() {
    if (mWasStarted) {
        interruptReading();
        mReaderThread.wait();
        if (mAccessWatch) {
            mAccessWatch->join();
            mAccessWatch.clear();
        }
        assert(hasError() || !mAccessWatch);
    }
}

RamLoader::RamBlockStructure RamLoader::getRamBlockStructure() const {
    RamBlockStructure res;
    res.pageSize = mPageSize;

    res.blocks.reserve(mIndex.blocks.size());

    for (const auto& blockInfo : mIndex.blocks) {
        res.blocks.push_back(blockInfo.ramBlock);
    }

    return res;
}

void RamLoader::applyRamBlockStructure(const RamBlockStructure& blockStructure) {
    mPageSize = blockStructure.pageSize;

    mIndex.clear();
    mIndex.blocks.reserve(blockStructure.blocks.size());

    for (const auto ramBlock : blockStructure.blocks) {
        mIndex.blocks.push_back({ ramBlock, {}, {}});
    }
}

void RamLoader::loadRam(void* ptr, uint64_t size) {
    uint32_t num_pages = (size + mPageSize - 1) / mPageSize;
    char* pagePtr = (char*)((uintptr_t)ptr & ~(mPageSize - 1));
    for (uint32_t i = 0; i < num_pages; i++) {
        loadRamPage(pagePtr + i * mPageSize);
    }
}

void RamLoader::registerBlock(const RamBlock& block) {
    mPageSize = block.pageSize;
    mIndex.blocks.push_back({block, {}, {}});
}

bool RamLoader::start(bool isQuickboot) {
    if (mWasStarted) {
        return !mHasError;
    }

    mIsQuickboot = isQuickboot;

    mStartTime = base::System::get()->getHighResTimeUs();

    mWasStarted = true;
    if (!readIndex()) {
        mHasError = true;
        return false;
    }
    if (!mAccessWatch) {
        bool res = readAllPages();
        mEndTime = base::System::get()->getHighResTimeUs();
#if SNAPSHOT_PROFILE > 1
        printf("Eager RAM load complete in %.03f ms\n",
               (mEndTime - mStartTime) / 1000.0);
#endif
        return res;
    }

    if (!registerPageWatches()) {
        mHasError = true;
        return false;
    }
    mBackgroundPageIt = mIndex.pages.begin();
    mAccessWatch->doneRegistering();
    mReaderThread.start();
    return true;
}

void RamLoader::join() {
#if SNAPSHOT_PROFILE > 1
    printf("Finishing background RAM load\n");
    Stopwatch sw;
#endif

    if (mAccessWatch) {
        // Unprotect all. Warning: this assumes the VM is stopped.

        ContiguousRangeMapper bulkFillPreparer([this](uintptr_t start, uintptr_t size) {
            mAccessWatch->initBulkFill((void*)start, size);
        });

        for (const Page& page : mIndex.pages) {
            auto ptr = pagePtr(page);
            auto size = pageSize(page);

            bulkFillPreparer.add((uintptr_t)ptr, size);
        }
    }

    // Set mJoining only after initBulkFill is all done, or we
    // get race conditions in fillPageData.
    mJoining = true;

    mReaderThread.wait();
    if (mAccessWatch) {
        mAccessWatch->join();
        mAccessWatch.clear();
    }
    mStream.close();

#if SNAPSHOT_PROFILE > 1
    printf("Finished remaining RAM load in %f ms\n", sw.elapsedUs() / 1000.0f);
#endif
}

void RamLoader::interrupt() {
    fprintf(stderr, "%s: call\n", __func__);

    mReadDataQueue.stop();
    mReadingQueue.stop();
    mReaderThread.wait();
    if (mAccessWatch) {
        mAccessWatch->join();
        mAccessWatch.clear();
    }
    mDecommitter.finish();
    mStream.close();
}

const RamLoader::Page* RamLoader::findPage(int blockIndex,
                                           const char* id,
                                           int pageIndex) const {
    if (blockIndex < 0 || blockIndex >= mIndex.blocks.size()) {
        return nullptr;
    }
    const auto& block = mIndex.blocks[blockIndex];
    assert(block.ramBlock.id == base::StringView(id));
    if (pageIndex < 0 || pageIndex > block.pagesEnd - block.pagesBegin) {
        return nullptr;
    }
    return &*(block.pagesBegin + pageIndex);
}

void RamLoader::interruptReading() {
    mLoadingCompleted.store(true, std::memory_order_relaxed);
    mReadDataQueue.stop();
    mReadingQueue.stop();
}

void RamLoader::zeroOutPage(const Page& page) {
    auto ptr = pagePtr(page);
    const RamBlock& block = mIndex.blocks[page.blockIndex].ramBlock;
    if (!isBufferZeroed(ptr, block.pageSize)) {
        memset(ptr, 0, size_t(block.pageSize));
    }
}

bool RamLoader::readIndex() {
#if SNAPSHOT_PROFILE > 1
    auto start = base::System::get()->getHighResTimeUs();
#endif
    mStreamFd = fileno(mStream.get());
    base::System::FileSize size;
    if (!base::System::get()->fileSize(mStreamFd, &size)) {
        return false;
    }
    mDiskSize = size;
    mIndexPos = mStream.getBe64();

    MemStream::Buffer buffer(size - mIndexPos);
    if (base::pread(mStreamFd, buffer.data(), buffer.size(), mIndexPos) !=
        buffer.size()) {
        return false;
    }

    MemStream stream(std::move(buffer));

    mVersion = stream.getBe32();
    if (mVersion < 1 || mVersion > 2) {
        return false;
    }
    mIndex.flags = IndexFlags(stream.getBe32());
    const bool compressed = nonzero(mIndex.flags & IndexFlags::CompressedPages);
    auto pageCount = stream.getBe32();
    mIndex.pages.reserve(pageCount);
    int64_t runningFilePos = 8;
    int32_t prevPageSizeOnDisk = 0;
    for (size_t loadedBlockCount = 0; loadedBlockCount < mIndex.blocks.size();
         ++loadedBlockCount) {
        const auto nameLength = stream.getByte();
        char name[256];
        stream.read(name, nameLength);
        name[nameLength] = 0;
        auto blockIt = std::find_if(mIndex.blocks.begin(), mIndex.blocks.end(),
                                    [&name](const FileIndex::Block& b) {
                                        return strcmp(b.ramBlock.id, name) == 0;
                                    });
        if (blockIt == mIndex.blocks.end()) {
            return false;
        }
        readBlockPages(&stream, blockIt, compressed, &runningFilePos,
                       &prevPageSizeOnDisk);
    }

    if (mVersion > 1) {
        mGaps = compressed ? GapTracker::Ptr(new GenericGapTracker())
                           : GapTracker::Ptr(new OneSizeGapTracker());
        mGaps->load(stream);
    }

#if SNAPSHOT_PROFILE > 1
    printf("readIndex() time: %.03f\n",
           (base::System::get()->getHighResTimeUs() - start) / 1000.0);
#endif
    return true;
}

void RamLoader::readBlockPages(base::Stream* stream,
                               FileIndex::Blocks::iterator blockIt,
                               bool compressed,
                               int64_t* runningFilePosPtr,
                               int32_t* prevPageSizeOnDiskPtr) {
    auto runningFilePos = *runningFilePosPtr;
    auto prevPageSizeOnDisk = *prevPageSizeOnDiskPtr;

    const auto blockIndex = std::distance(mIndex.blocks.begin(), blockIt);

    const auto blockPagesCount = stream->getBe32();
    FileIndex::Block& block = *blockIt;
    auto pageIt = mIndex.pages.end();
    block.pagesBegin = pageIt;
    mIndex.pages.resize(mIndex.pages.size() + blockPagesCount);
    const auto endIt = mIndex.pages.end();
    block.pagesEnd = endIt;

    for (; pageIt != endIt; ++pageIt) {
        Page& page = *pageIt;
        page.blockIndex = uint16_t(blockIndex);
        const auto sizeOnDisk = stream->getPackedNum();
        if (sizeOnDisk == 0) {
            // Empty page
            page.state.store(uint8_t(State::Read), std::memory_order_relaxed);
            page.sizeOnDisk = 0;
            page.filePos = 0;
        } else {
            if (mIndexOnly) {
                page.state.store(uint8_t(State::Filled), std::memory_order_relaxed);
            }
            page.blockIndex = uint16_t(blockIndex);
            page.sizeOnDisk = uint32_t(sizeOnDisk);
            auto posDelta = stream->getPackedSignedNum();
            if (compressed) {
                posDelta += prevPageSizeOnDisk;
                prevPageSizeOnDisk = int32_t(page.sizeOnDisk);
            } else {
                page.sizeOnDisk *= uint32_t(block.ramBlock.pageSize);
                posDelta *= block.ramBlock.pageSize;
            }
            if (mVersion == 2) {
                stream->read(page.hash.data(), page.hash.size());
            }
            runningFilePos += posDelta;
            page.filePos = uint64_t(runningFilePos);
        }
    }

    *runningFilePosPtr = runningFilePos;
    *prevPageSizeOnDiskPtr = prevPageSizeOnDisk;
}

bool RamLoader::registerPageWatches() {
    uint8_t* startPtr = nullptr;
    uint64_t curSize = 0;
    for (const Page& page : mIndex.pages) {
        auto ptr = pagePtr(page);
        auto size = pageSize(page);
        if (ptr == startPtr + curSize) {
            curSize += size;
        } else {
            if (startPtr) {
                if (!mAccessWatch->registerMemoryRange(startPtr, curSize)) {
                    return false;
                }
            }
            startPtr = ptr;
            curSize = size;
        }
    }
    if (startPtr) {
        if (!mAccessWatch->registerMemoryRange(startPtr, curSize)) {
            return false;
        }
    }
    return true;
}

uint8_t* RamLoader::pagePtr(const RamLoader::Page& page) const {
    const FileIndex::Block& block = mIndex.blocks[page.blockIndex];
    return block.ramBlock.hostPtr + uint64_t(&page - &*block.pagesBegin) *
                                            uint64_t(block.ramBlock.pageSize);
}

uint32_t RamLoader::pageSize(const RamLoader::Page& page) const {
    return uint32_t(mIndex.blocks[page.blockIndex].ramBlock.pageSize);
}

template <class Num>
static bool isPowerOf2(Num num) {
    return !(num & (num - 1));
}

RamLoader::Page& RamLoader::page(void* ptr) {
    const auto blockIt = std::find_if(
            mIndex.blocks.begin(), mIndex.blocks.end(),
            [ptr](const FileIndex::Block& b) {
                return ptr >= b.ramBlock.hostPtr &&
                       ptr < b.ramBlock.hostPtr + b.ramBlock.totalSize;
            });
    assert(blockIt != mIndex.blocks.end());
    assert(ptr >= blockIt->ramBlock.hostPtr);
    assert(ptr < blockIt->ramBlock.hostPtr + blockIt->ramBlock.totalSize);
    assert(blockIt->pagesBegin != blockIt->pagesEnd);

    assert(isPowerOf2(blockIt->ramBlock.pageSize));
    auto pageStart = reinterpret_cast<uint8_t*>(
            (uintptr_t(ptr)) & uintptr_t(~(blockIt->ramBlock.pageSize - 1)));
    auto pageIndex = (pageStart - blockIt->ramBlock.hostPtr) /
                     blockIt->ramBlock.pageSize;
    auto pageIt = blockIt->pagesBegin + pageIndex;
    assert(pageIt != blockIt->pagesEnd);
    assert(ptr >= pagePtr(*pageIt));
    assert(ptr < pagePtr(*pageIt) + pageSize(*pageIt));
    return *pageIt;
}

void RamLoader::readerWorker() {
    while (auto pagePtr = mReadingQueue.receive()) {
        Page* page = *pagePtr;
        if (!page) {
            mReadDataQueue.send(nullptr);
            mReadingQueue.stop();
            break;
        }

        if (readDataFromDisk(page)) {
            mReadDataQueue.send(page);
        }
    }

    mEndTime = base::System::get()->getHighResTimeUs();
#if SNAPSHOT_PROFILE > 1
    printf("Background loading complete in %.03f ms\n",
           (mEndTime - mStartTime) / 1000.0);
#endif
}

MemoryAccessWatch::IdleCallbackResult RamLoader::backgroundPageLoad() {
    if (mReadingQueue.isStopped() && mReadDataQueue.isStopped()) {
        return MemoryAccessWatch::IdleCallbackResult::AllDone;
    }

    {
        Page* page = nullptr;
        if (mReadDataQueue.tryReceive(&page)) {
            return fillPageInBackground(page);
        }
    }

    for (int i = 0; i < int(mReadingQueue.capacity()); ++i) {
        // Find next page to queue.
        mBackgroundPageIt = std::find_if(
                mBackgroundPageIt, mIndex.pages.end(), [](const Page& page) {
                    auto state = page.state.load(std::memory_order_acquire);
                    return state == uint8_t(State::Empty) ||
                           (state == uint8_t(State::Read) && !page.data);
                });
#if SNAPSHOT_PROFILE > 2
        const auto count = int(mBackgroundPageIt - mIndex.pages.begin());
        if ((count % 10000) == 0 || count == int(mIndex.pages.size())) {
            printf("Background loading: at page #%d of %d\n", count,
                   int(mIndex.pages.size()));
        }
#endif

        if (mBackgroundPageIt == mIndex.pages.end()) {
            if (!mSentEndOfPagesMarker) {
                mSentEndOfPagesMarker = mReadingQueue.trySend(nullptr);
            }
            return mJoining ? MemoryAccessWatch::IdleCallbackResult::RunAgain
                            : MemoryAccessWatch::IdleCallbackResult::Wait;
        }

        if (mBackgroundPageIt->state.load(std::memory_order_relaxed) ==
            uint8_t(State::Read)) {
            Page* const page = &*mBackgroundPageIt++;
            return fillPageInBackground(page);
        }

        if (mReadingQueue.trySend(&*mBackgroundPageIt)) {
            ++mBackgroundPageIt;
        } else {
            // The queue is full - let's wait for a while to give the reader
            // time to empty it.
            return mJoining ? MemoryAccessWatch::IdleCallbackResult::RunAgain
                            : MemoryAccessWatch::IdleCallbackResult::Wait;
        }
    }

    return MemoryAccessWatch::IdleCallbackResult::RunAgain;
}

MemoryAccessWatch::IdleCallbackResult RamLoader::fillPageInBackground(
        RamLoader::Page* page) {
    if (page) {
        fillPageData(page);
        delete[] page->data;
        page->data = nullptr;
        // The guest probably doesn't want to access this page right now.
        // Page it out if possible.
        mDecommitter.add((uintptr_t)pagePtr(*page), mPageSize);
        // If we've loaded a page then this function took quite a while
        // and it's better to check for a pagefault before proceeding to
        // queuing pages into the reader thread.
        return mJoining ? MemoryAccessWatch::IdleCallbackResult::RunAgain
                        : MemoryAccessWatch::IdleCallbackResult::Wait;
    } else {
        mDecommitter.finish();
        // null page == all pages were loaded, stop.
        interruptReading();
        return MemoryAccessWatch::IdleCallbackResult::AllDone;
    }
}

void RamLoader::loadRamPage(void* ptr) {
    // It's possible for us to try to RAM load
    // things that are not registered in the index
    // (like from qemu_iovec_init_external).
    // Make sure that it is in the index.
    const auto blockIt = std::find_if(
            mIndex.blocks.begin(), mIndex.blocks.end(),
            [ptr](const FileIndex::Block& b) {
                return ptr >= b.ramBlock.hostPtr &&
                       ptr < b.ramBlock.hostPtr + b.ramBlock.totalSize;
            });

    if (blockIt == mIndex.blocks.end()) {
        return;
    }

    Page& page = this->page(ptr);
    uint8_t buf[kDefaultPageSize];
    readDataFromDisk(&page, ARRAY_SIZE(buf) >= pageSize(page) ? buf : nullptr);
    fillPageData(&page);
    if (page.data != buf) {
        delete[] page.data;
    }
    page.data = nullptr;
}

bool RamLoader::readDataFromDisk(Page* pagePtr, uint8_t* preallocatedBuffer) {
    Page& page = *pagePtr;
    if (page.sizeOnDisk == 0) {
        assert(page.state.load(std::memory_order_relaxed) >=
               uint8_t(State::Read));
        page.data = nullptr;
        return true;
    }

    auto state = uint8_t(State::Empty);
    if (!page.state.compare_exchange_strong(state, uint8_t(State::Reading),
                                            std::memory_order_acquire)) {
        // Spin until the reading thread finishes.
        while (state < uint8_t(State::Read)) {
            base::System::get()->yield();
            state = uint8_t(page.state.load(std::memory_order_acquire));
        }
        return false;
    }

    uint8_t compressedBuf[compress::maxCompressedSize(kDefaultPageSize)];
    auto size = page.sizeOnDisk;
    const bool compressed =
            nonzero(mIndex.flags & IndexFlags::CompressedPages) &&
            (mVersion == 1 || page.sizeOnDisk < kDefaultPageSize);

    // We need to allocate a dynamic buffer if:
    // - page is compressed and there's a decompressing thread pool
    // - page is compressed and local buffer is too small
    // - there's no preallocated buffer passed from the caller
    bool allocateBuffer = (compressed && (mDecompressor ||
                                          ARRAY_SIZE(compressedBuf) < size)) ||
                          !preallocatedBuffer;
    auto buf = allocateBuffer ? new uint8_t[size]
                              : compressed ? compressedBuf : preallocatedBuffer;
    auto read = HANDLE_EINTR(
            base::pread(mStreamFd, buf, size, int64_t(page.filePos)));
    if (read != int64_t(size)) {
        VERBOSE_PRINT(snapshot,
                      "Error: (%d) Reading page %p from disk returned less "
                      "data: %d of %d at %lld",
                      errno, this->pagePtr(page), int(read), int(size),
                      static_cast<long long>(page.filePos));
        if (allocateBuffer) {
            delete[] buf;
        }
        page.state.store(uint8_t(State::Error));
        mHasError = true;
        return false;
    }

    if (compressed) {
        if (mDecompressor) {
            page.data = buf;
            mDecompressor->enqueue(&page);
        } else {
            auto decompressed = preallocatedBuffer
                                        ? preallocatedBuffer
                                        : new uint8_t[pageSize(page)];
            if (!Decompressor::decompress(buf, int32_t(size), decompressed,
                                          int32_t(pageSize(page)))) {
                VERBOSE_PRINT(snapshot,
                              "Error: Decompressing page %p @%llu (%d -> %d) "
                              "failed",
                              this->pagePtr(page),
                              (unsigned long long)page.filePos, int(size),
                              int(pageSize(page)));
                if (!preallocatedBuffer) {
                    delete[] decompressed;
                }
                page.state.store(uint8_t(State::Error));
                mHasError = true;
                return false;
            }
            buf = decompressed;
        }
    }

    page.data = buf;
    page.state.store(uint8_t(State::Read), std::memory_order_release);
    return true;
}

void RamLoader::fillPageData(Page* pagePtr) {
    Page& page = *pagePtr;
    auto state = uint8_t(State::Read);
    if (!page.state.compare_exchange_strong(state, uint8_t(State::Filling),
                                            std::memory_order_acquire)) {
        while (state < uint8_t(State::Filled)) {
            base::System::get()->yield();
            state = page.state.load(std::memory_order_relaxed);
        }
        return;
    }

#if SNAPSHOT_PROFILE > 2
    printf("%s: loading page %p\n", __func__, this->pagePtr(page));
#endif
    if (mAccessWatch) {

        bool res = mJoining ?
            mAccessWatch->fillPageBulk(this->pagePtr(page), pageSize(page), page.data, mIsQuickboot) :
            mAccessWatch->fillPage(this->pagePtr(page), pageSize(page), page.data, mIsQuickboot);

        if (!res) {
            mHasError = true;
        }
        page.state.store(uint8_t(res ? State::Filled : State::Error),
                         std::memory_order_release);
    }
}

bool RamLoader::readAllPages() {
#if SNAPSHOT_PROFILE > 1
    auto startTime = base::System::get()->getHighResTimeUs();
#endif
    if (nonzero(mIndex.flags & IndexFlags::CompressedPages) && !mAccessWatch) {
        startDecompressor();
    }

    // Rearrange the nonzero pages in sequential disk order for faster reading.
    // Zero out all zero pages right here.
    std::vector<Page*> sortedPages;
    sortedPages.reserve(mIndex.pages.size());

#if SNAPSHOT_PROFILE > 1
    auto startTime1 = base::System::get()->getHighResTimeUs();
#endif

    {

#if SNAPSHOT_PROFILE > 1
        ScopedMemoryProfiler memProf("zeroOut");
#endif

        // Decommit nonzero pages as they are loaded;
        // let the guest decide which ones to page in.
        ContiguousRangeMapper zeroPageDecommitter(
            [this](uintptr_t start, uintptr_t size) {
                if (mIsQuickboot) {
                    android::base::memoryHint((void*)start, size, base::MemoryHint::DontNeed);
                } else {
                    android::base::zeroOutMemory((void*)start, size);
                }
            }, 4096 * mPageSize); // flush them out every 16 mb

        for (Page& page : mIndex.pages) {
            if (page.sizeOnDisk) {
                sortedPages.emplace_back(&page);
            } else {
                // Decommit regardless of quickboot.
                auto ptr = pagePtr(page);
                zeroPageDecommitter.add((uintptr_t)ptr, mPageSize);
            }
        }
    }


#if SNAPSHOT_PROFILE > 1
    printf("zeroing took %.03f ms\n",
           (base::System::get()->getHighResTimeUs() - startTime1) / 1000.0);
#endif

    std::sort(sortedPages.begin(), sortedPages.end(),
              [](const Page* l, const Page* r) {
                  return l->filePos < r->filePos;
              });

#if SNAPSHOT_PROFILE > 1
    printf("Starting unpacker + sorting took %.03f ms\n",
           (base::System::get()->getHighResTimeUs() - startTime) / 1000.0);
#endif

#if SNAPSHOT_PROFILE > 1
    ScopedMemoryProfiler memProf("readingDataFromDisk to decompress finish");
#endif
    {
        for (Page* page : sortedPages) {
            auto ptr = pagePtr(*page);

            if (!readDataFromDisk(page, ptr)) {
                mHasError = true;
                return false;
            }

            mDecommitter.add((uintptr_t)ptr, int32_t(pageSize(*page)));
        }
    }

    mDecompressor.clear();
    mDecommitter.finish();

    // Make everything normal again.
    void* firstHostPtr = pagePtr(mIndex.pages[0]);
    size_t numPages = mIndex.pages.size();
    android::base::memoryHint(firstHostPtr, numPages * mPageSize, base::MemoryHint::Normal);
    return true;
}

void RamLoader::startDecompressor() {
    mDecompressor.emplace([this](Page* page) {
        const bool res = Decompressor::decompress(
                page->data, int32_t(page->sizeOnDisk), pagePtr(*page),
                int32_t(pageSize(*page)));
        delete[] page->data;
        page->data = nullptr;
        android::base::memoryHint(pagePtr(*page), int32_t(pageSize(*page)), base::MemoryHint::PageOut);
        if (!res) {
            derror("Decompressing page %p failed", pagePtr(*page));
            mHasError = true;
            page->state.store(uint8_t(State::Error));
        }
    });
    mDecompressor->start();
}

}  // namespace snapshot
}  // namespace android