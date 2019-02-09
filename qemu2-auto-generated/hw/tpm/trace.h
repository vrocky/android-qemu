/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_HW_TPM_GENERATED_TRACERS_H
#define TRACE_HW_TPM_GENERATED_TRACERS_H

#include "qemu-common.h"
#include "trace/control.h"

extern TraceEvent _TRACE_TPM_CRB_MMIO_READ_EVENT;
extern TraceEvent _TRACE_TPM_CRB_MMIO_WRITE_EVENT;
extern TraceEvent _TRACE_TPM_PASSTHROUGH_HANDLE_REQUEST_EVENT;
extern TraceEvent _TRACE_TPM_PASSTHROUGH_RESET_EVENT;
extern TraceEvent _TRACE_TPM_UTIL_GET_BUFFER_SIZE_HDR_LEN_EVENT;
extern TraceEvent _TRACE_TPM_UTIL_GET_BUFFER_SIZE_LEN_EVENT;
extern TraceEvent _TRACE_TPM_UTIL_GET_BUFFER_SIZE_HDR_LEN2_EVENT;
extern TraceEvent _TRACE_TPM_UTIL_GET_BUFFER_SIZE_LEN2_EVENT;
extern TraceEvent _TRACE_TPM_UTIL_GET_BUFFER_SIZE_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_SET_LOCALITY_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_HANDLE_REQUEST_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_PROBE_CAPS_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_SET_BUFFER_SIZE_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_STARTUP_TPM_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_GET_TPM_ESTABLISHED_FLAG_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_CANCEL_CMD_NOT_SUPT_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_TPM12_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_TPM2_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_UNSPEC_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_STARTUP_ERROR_EVENT;
extern TraceEvent _TRACE_TPM_EMULATOR_INST_INIT_EVENT;
extern TraceEvent _TRACE_TPM_TIS_RAISE_IRQ_EVENT;
extern TraceEvent _TRACE_TPM_TIS_NEW_ACTIVE_LOCALITY_EVENT;
extern TraceEvent _TRACE_TPM_TIS_ABORT_EVENT;
extern TraceEvent _TRACE_TPM_TIS_DATA_READ_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_READ_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_LOCTY4_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_RELEASE_LOCTY_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_LOCTY_REQ_USE_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_NEXT_LOCTY_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_LOCTY_SEIZED_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_INIT_ABORT_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_LOWERING_IRQ_EVENT;
extern TraceEvent _TRACE_TPM_TIS_MMIO_WRITE_DATA2SEND_EVENT;
extern uint16_t _TRACE_TPM_CRB_MMIO_READ_DSTATE;
extern uint16_t _TRACE_TPM_CRB_MMIO_WRITE_DSTATE;
extern uint16_t _TRACE_TPM_PASSTHROUGH_HANDLE_REQUEST_DSTATE;
extern uint16_t _TRACE_TPM_PASSTHROUGH_RESET_DSTATE;
extern uint16_t _TRACE_TPM_UTIL_GET_BUFFER_SIZE_HDR_LEN_DSTATE;
extern uint16_t _TRACE_TPM_UTIL_GET_BUFFER_SIZE_LEN_DSTATE;
extern uint16_t _TRACE_TPM_UTIL_GET_BUFFER_SIZE_HDR_LEN2_DSTATE;
extern uint16_t _TRACE_TPM_UTIL_GET_BUFFER_SIZE_LEN2_DSTATE;
extern uint16_t _TRACE_TPM_UTIL_GET_BUFFER_SIZE_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_SET_LOCALITY_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_HANDLE_REQUEST_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_PROBE_CAPS_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_SET_BUFFER_SIZE_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_STARTUP_TPM_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_GET_TPM_ESTABLISHED_FLAG_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_CANCEL_CMD_NOT_SUPT_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_TPM12_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_TPM2_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_UNSPEC_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_STARTUP_ERROR_DSTATE;
extern uint16_t _TRACE_TPM_EMULATOR_INST_INIT_DSTATE;
extern uint16_t _TRACE_TPM_TIS_RAISE_IRQ_DSTATE;
extern uint16_t _TRACE_TPM_TIS_NEW_ACTIVE_LOCALITY_DSTATE;
extern uint16_t _TRACE_TPM_TIS_ABORT_DSTATE;
extern uint16_t _TRACE_TPM_TIS_DATA_READ_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_READ_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_LOCTY4_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_RELEASE_LOCTY_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_LOCTY_REQ_USE_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_NEXT_LOCTY_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_LOCTY_SEIZED_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_INIT_ABORT_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_LOWERING_IRQ_DSTATE;
extern uint16_t _TRACE_TPM_TIS_MMIO_WRITE_DATA2SEND_DSTATE;
#define TRACE_TPM_CRB_MMIO_READ_ENABLED 1
#define TRACE_TPM_CRB_MMIO_WRITE_ENABLED 1
#define TRACE_TPM_PASSTHROUGH_HANDLE_REQUEST_ENABLED 1
#define TRACE_TPM_PASSTHROUGH_RESET_ENABLED 1
#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_HDR_LEN_ENABLED 1
#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_LEN_ENABLED 1
#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_HDR_LEN2_ENABLED 1
#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_LEN2_ENABLED 1
#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_ENABLED 1
#define TRACE_TPM_EMULATOR_SET_LOCALITY_ENABLED 1
#define TRACE_TPM_EMULATOR_HANDLE_REQUEST_ENABLED 1
#define TRACE_TPM_EMULATOR_PROBE_CAPS_ENABLED 1
#define TRACE_TPM_EMULATOR_SET_BUFFER_SIZE_ENABLED 1
#define TRACE_TPM_EMULATOR_STARTUP_TPM_ENABLED 1
#define TRACE_TPM_EMULATOR_GET_TPM_ESTABLISHED_FLAG_ENABLED 1
#define TRACE_TPM_EMULATOR_CANCEL_CMD_NOT_SUPT_ENABLED 1
#define TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_TPM12_ENABLED 1
#define TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_TPM2_ENABLED 1
#define TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_UNSPEC_ENABLED 1
#define TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_STARTUP_ERROR_ENABLED 1
#define TRACE_TPM_EMULATOR_INST_INIT_ENABLED 1
#define TRACE_TPM_TIS_RAISE_IRQ_ENABLED 1
#define TRACE_TPM_TIS_NEW_ACTIVE_LOCALITY_ENABLED 1
#define TRACE_TPM_TIS_ABORT_ENABLED 1
#define TRACE_TPM_TIS_DATA_READ_ENABLED 1
#define TRACE_TPM_TIS_MMIO_READ_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_LOCTY4_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_RELEASE_LOCTY_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_LOCTY_REQ_USE_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_NEXT_LOCTY_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_LOCTY_SEIZED_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_INIT_ABORT_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_LOWERING_IRQ_ENABLED 1
#define TRACE_TPM_TIS_MMIO_WRITE_DATA2SEND_ENABLED 1

#define TRACE_TPM_CRB_MMIO_READ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_crb_mmio_read(uint64_t addr, unsigned size, uint32_t val)
{
}

static inline void trace_tpm_crb_mmio_read(uint64_t addr, unsigned size, uint32_t val)
{
    if (true) {
        _nocheck__trace_tpm_crb_mmio_read(addr, size, val);
    }
}

#define TRACE_TPM_CRB_MMIO_WRITE_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_crb_mmio_write(uint64_t addr, unsigned size, uint32_t val)
{
}

static inline void trace_tpm_crb_mmio_write(uint64_t addr, unsigned size, uint32_t val)
{
    if (true) {
        _nocheck__trace_tpm_crb_mmio_write(addr, size, val);
    }
}

#define TRACE_TPM_PASSTHROUGH_HANDLE_REQUEST_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_passthrough_handle_request(void * cmd)
{
}

static inline void trace_tpm_passthrough_handle_request(void * cmd)
{
    if (true) {
        _nocheck__trace_tpm_passthrough_handle_request(cmd);
    }
}

#define TRACE_TPM_PASSTHROUGH_RESET_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_passthrough_reset(void)
{
}

static inline void trace_tpm_passthrough_reset(void)
{
    if (true) {
        _nocheck__trace_tpm_passthrough_reset();
    }
}

#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_HDR_LEN_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_util_get_buffer_size_hdr_len(uint32_t len, size_t expected)
{
}

static inline void trace_tpm_util_get_buffer_size_hdr_len(uint32_t len, size_t expected)
{
    if (true) {
        _nocheck__trace_tpm_util_get_buffer_size_hdr_len(len, expected);
    }
}

#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_LEN_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_util_get_buffer_size_len(uint32_t len, size_t expected)
{
}

static inline void trace_tpm_util_get_buffer_size_len(uint32_t len, size_t expected)
{
    if (true) {
        _nocheck__trace_tpm_util_get_buffer_size_len(len, expected);
    }
}

#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_HDR_LEN2_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_util_get_buffer_size_hdr_len2(uint32_t len, size_t expected)
{
}

static inline void trace_tpm_util_get_buffer_size_hdr_len2(uint32_t len, size_t expected)
{
    if (true) {
        _nocheck__trace_tpm_util_get_buffer_size_hdr_len2(len, expected);
    }
}

#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_LEN2_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_util_get_buffer_size_len2(uint32_t len, size_t expected)
{
}

static inline void trace_tpm_util_get_buffer_size_len2(uint32_t len, size_t expected)
{
    if (true) {
        _nocheck__trace_tpm_util_get_buffer_size_len2(len, expected);
    }
}

#define TRACE_TPM_UTIL_GET_BUFFER_SIZE_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_util_get_buffer_size(size_t len)
{
}

static inline void trace_tpm_util_get_buffer_size(size_t len)
{
    if (true) {
        _nocheck__trace_tpm_util_get_buffer_size(len);
    }
}

#define TRACE_TPM_EMULATOR_SET_LOCALITY_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_set_locality(uint8_t locty)
{
}

static inline void trace_tpm_emulator_set_locality(uint8_t locty)
{
    if (true) {
        _nocheck__trace_tpm_emulator_set_locality(locty);
    }
}

#define TRACE_TPM_EMULATOR_HANDLE_REQUEST_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_handle_request(void)
{
}

static inline void trace_tpm_emulator_handle_request(void)
{
    if (true) {
        _nocheck__trace_tpm_emulator_handle_request();
    }
}

#define TRACE_TPM_EMULATOR_PROBE_CAPS_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_probe_caps(uint64_t caps)
{
}

static inline void trace_tpm_emulator_probe_caps(uint64_t caps)
{
    if (true) {
        _nocheck__trace_tpm_emulator_probe_caps(caps);
    }
}

#define TRACE_TPM_EMULATOR_SET_BUFFER_SIZE_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_set_buffer_size(uint32_t buffersize, uint32_t minsize, uint32_t maxsize)
{
}

static inline void trace_tpm_emulator_set_buffer_size(uint32_t buffersize, uint32_t minsize, uint32_t maxsize)
{
    if (true) {
        _nocheck__trace_tpm_emulator_set_buffer_size(buffersize, minsize, maxsize);
    }
}

#define TRACE_TPM_EMULATOR_STARTUP_TPM_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_startup_tpm(void)
{
}

static inline void trace_tpm_emulator_startup_tpm(void)
{
    if (true) {
        _nocheck__trace_tpm_emulator_startup_tpm();
    }
}

#define TRACE_TPM_EMULATOR_GET_TPM_ESTABLISHED_FLAG_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_get_tpm_established_flag(uint8_t flag)
{
}

static inline void trace_tpm_emulator_get_tpm_established_flag(uint8_t flag)
{
    if (true) {
        _nocheck__trace_tpm_emulator_get_tpm_established_flag(flag);
    }
}

#define TRACE_TPM_EMULATOR_CANCEL_CMD_NOT_SUPT_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_cancel_cmd_not_supt(void)
{
}

static inline void trace_tpm_emulator_cancel_cmd_not_supt(void)
{
    if (true) {
        _nocheck__trace_tpm_emulator_cancel_cmd_not_supt();
    }
}

#define TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_TPM12_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_handle_device_opts_tpm12(void)
{
}

static inline void trace_tpm_emulator_handle_device_opts_tpm12(void)
{
    if (true) {
        _nocheck__trace_tpm_emulator_handle_device_opts_tpm12();
    }
}

#define TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_TPM2_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_handle_device_opts_tpm2(void)
{
}

static inline void trace_tpm_emulator_handle_device_opts_tpm2(void)
{
    if (true) {
        _nocheck__trace_tpm_emulator_handle_device_opts_tpm2();
    }
}

#define TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_UNSPEC_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_handle_device_opts_unspec(void)
{
}

static inline void trace_tpm_emulator_handle_device_opts_unspec(void)
{
    if (true) {
        _nocheck__trace_tpm_emulator_handle_device_opts_unspec();
    }
}

#define TRACE_TPM_EMULATOR_HANDLE_DEVICE_OPTS_STARTUP_ERROR_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_handle_device_opts_startup_error(void)
{
}

static inline void trace_tpm_emulator_handle_device_opts_startup_error(void)
{
    if (true) {
        _nocheck__trace_tpm_emulator_handle_device_opts_startup_error();
    }
}

#define TRACE_TPM_EMULATOR_INST_INIT_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_emulator_inst_init(void)
{
}

static inline void trace_tpm_emulator_inst_init(void)
{
    if (true) {
        _nocheck__trace_tpm_emulator_inst_init();
    }
}

#define TRACE_TPM_TIS_RAISE_IRQ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_raise_irq(uint32_t irqmask)
{
}

static inline void trace_tpm_tis_raise_irq(uint32_t irqmask)
{
    if (true) {
        _nocheck__trace_tpm_tis_raise_irq(irqmask);
    }
}

#define TRACE_TPM_TIS_NEW_ACTIVE_LOCALITY_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_new_active_locality(uint8_t locty)
{
}

static inline void trace_tpm_tis_new_active_locality(uint8_t locty)
{
    if (true) {
        _nocheck__trace_tpm_tis_new_active_locality(locty);
    }
}

#define TRACE_TPM_TIS_ABORT_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_abort(uint8_t locty)
{
}

static inline void trace_tpm_tis_abort(uint8_t locty)
{
    if (true) {
        _nocheck__trace_tpm_tis_abort(locty);
    }
}

#define TRACE_TPM_TIS_DATA_READ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_data_read(uint32_t value, uint32_t off)
{
}

static inline void trace_tpm_tis_data_read(uint32_t value, uint32_t off)
{
    if (true) {
        _nocheck__trace_tpm_tis_data_read(value, off);
    }
}

#define TRACE_TPM_TIS_MMIO_READ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_read(unsigned size, uint32_t addr, uint32_t val)
{
}

static inline void trace_tpm_tis_mmio_read(unsigned size, uint32_t addr, uint32_t val)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_read(size, addr, val);
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write(unsigned size, uint32_t addr, uint32_t val)
{
}

static inline void trace_tpm_tis_mmio_write(unsigned size, uint32_t addr, uint32_t val)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write(size, addr, val);
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_LOCTY4_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write_locty4(void)
{
}

static inline void trace_tpm_tis_mmio_write_locty4(void)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write_locty4();
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_RELEASE_LOCTY_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write_release_locty(uint8_t locty)
{
}

static inline void trace_tpm_tis_mmio_write_release_locty(uint8_t locty)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write_release_locty(locty);
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_LOCTY_REQ_USE_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write_locty_req_use(uint8_t locty)
{
}

static inline void trace_tpm_tis_mmio_write_locty_req_use(uint8_t locty)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write_locty_req_use(locty);
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_NEXT_LOCTY_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write_next_locty(uint8_t locty)
{
}

static inline void trace_tpm_tis_mmio_write_next_locty(uint8_t locty)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write_next_locty(locty);
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_LOCTY_SEIZED_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write_locty_seized(uint8_t locty, uint8_t active)
{
}

static inline void trace_tpm_tis_mmio_write_locty_seized(uint8_t locty, uint8_t active)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write_locty_seized(locty, active);
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_INIT_ABORT_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write_init_abort(void)
{
}

static inline void trace_tpm_tis_mmio_write_init_abort(void)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write_init_abort();
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_LOWERING_IRQ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write_lowering_irq(void)
{
}

static inline void trace_tpm_tis_mmio_write_lowering_irq(void)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write_lowering_irq();
    }
}

#define TRACE_TPM_TIS_MMIO_WRITE_DATA2SEND_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_tpm_tis_mmio_write_data2send(uint32_t value, unsigned size)
{
}

static inline void trace_tpm_tis_mmio_write_data2send(uint32_t value, unsigned size)
{
    if (true) {
        _nocheck__trace_tpm_tis_mmio_write_data2send(value, size);
    }
}
#endif /* TRACE_HW_TPM_GENERATED_TRACERS_H */