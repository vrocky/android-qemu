// Copyright (C) 2019 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "android/emulation/control/EventDispatcher.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <openssl/base64.h>
#include <vector>
#include "android/base/Log.h"

namespace android {
namespace emulation {
namespace control {

using google::protobuf::io::ArrayInputStream;

EventDispatcher::EventDispatcher(const AndroidConsoleAgents* const agents)
    : mAgents(agents), mKeyEventSender(agents), mTouchEventSender(agents) {}

void EventDispatcher::dispatchEvent(const json msg) {
    if (msg.count("label") == 0) {
        LOG(INFO) << "event does not have a label" << msg.dump(2);
        return;
    }

    std::string label = msg["label"];
    std::string blob = msg["msg"];

    // Decode BASE64.
    size_t needed, decoded_size;
    EVP_DecodedLength(&needed, blob.size());
    std::vector<uint8_t> decoded;
    decoded.resize(needed);
    EVP_DecodeBase64(decoded.data(), &decoded_size, decoded.size(),
                     (const uint8_t*)(blob.data()), blob.size());


    KeyboardEvent keyEvent;
    MouseEvent mouseEvent;
    TouchEvent touchEvent;

    // Valid labels are defined in Participant.h
    if (label == "keyboard" &&
        keyEvent.ParseFromArray(decoded.data(), decoded_size)) {
        mKeyEventSender.send(&keyEvent);
    } else if (label == "mouse" &&
               mouseEvent.ParseFromArray(decoded.data(), decoded_size)) {
        mAgents->user_event->sendMouseEvent(mouseEvent.x(), mouseEvent.y(), 0,
                                            mouseEvent.buttons(), 0);
    } else if (label == "touch" &&
               touchEvent.ParseFromArray(decoded.data(), decoded_size)) {
        mTouchEventSender.send(&touchEvent);
    } else {
        LOG(WARNING) << "Unable to handle " << label
                     << ", bad label or protobuf.";
    }
}
}  // namespace control
}  // namespace emulation
}  // namespace android