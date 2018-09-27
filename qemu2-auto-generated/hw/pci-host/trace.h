/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_HW_PCI_HOST_GENERATED_TRACERS_H
#define TRACE_HW_PCI_HOST_GENERATED_TRACERS_H

#include "qemu-common.h"
#include "trace/control.h"

extern TraceEvent _TRACE_GRACKLE_SET_IRQ_EVENT;
extern TraceEvent _TRACE_SABRE_SET_REQUEST_EVENT;
extern TraceEvent _TRACE_SABRE_CLEAR_REQUEST_EVENT;
extern TraceEvent _TRACE_SABRE_CONFIG_WRITE_EVENT;
extern TraceEvent _TRACE_SABRE_CONFIG_READ_EVENT;
extern TraceEvent _TRACE_SABRE_PCI_CONFIG_WRITE_EVENT;
extern TraceEvent _TRACE_SABRE_PCI_CONFIG_READ_EVENT;
extern TraceEvent _TRACE_SABRE_PCI_SET_IRQ_EVENT;
extern TraceEvent _TRACE_SABRE_PCI_SET_OBIO_IRQ_EVENT;
extern TraceEvent _TRACE_UNIN_SET_IRQ_EVENT;
extern TraceEvent _TRACE_UNIN_GET_CONFIG_REG_EVENT;
extern TraceEvent _TRACE_UNIN_DATA_WRITE_EVENT;
extern TraceEvent _TRACE_UNIN_DATA_READ_EVENT;
extern uint16_t _TRACE_GRACKLE_SET_IRQ_DSTATE;
extern uint16_t _TRACE_SABRE_SET_REQUEST_DSTATE;
extern uint16_t _TRACE_SABRE_CLEAR_REQUEST_DSTATE;
extern uint16_t _TRACE_SABRE_CONFIG_WRITE_DSTATE;
extern uint16_t _TRACE_SABRE_CONFIG_READ_DSTATE;
extern uint16_t _TRACE_SABRE_PCI_CONFIG_WRITE_DSTATE;
extern uint16_t _TRACE_SABRE_PCI_CONFIG_READ_DSTATE;
extern uint16_t _TRACE_SABRE_PCI_SET_IRQ_DSTATE;
extern uint16_t _TRACE_SABRE_PCI_SET_OBIO_IRQ_DSTATE;
extern uint16_t _TRACE_UNIN_SET_IRQ_DSTATE;
extern uint16_t _TRACE_UNIN_GET_CONFIG_REG_DSTATE;
extern uint16_t _TRACE_UNIN_DATA_WRITE_DSTATE;
extern uint16_t _TRACE_UNIN_DATA_READ_DSTATE;
#define TRACE_GRACKLE_SET_IRQ_ENABLED 1
#define TRACE_SABRE_SET_REQUEST_ENABLED 1
#define TRACE_SABRE_CLEAR_REQUEST_ENABLED 1
#define TRACE_SABRE_CONFIG_WRITE_ENABLED 1
#define TRACE_SABRE_CONFIG_READ_ENABLED 1
#define TRACE_SABRE_PCI_CONFIG_WRITE_ENABLED 1
#define TRACE_SABRE_PCI_CONFIG_READ_ENABLED 1
#define TRACE_SABRE_PCI_SET_IRQ_ENABLED 1
#define TRACE_SABRE_PCI_SET_OBIO_IRQ_ENABLED 1
#define TRACE_UNIN_SET_IRQ_ENABLED 1
#define TRACE_UNIN_GET_CONFIG_REG_ENABLED 1
#define TRACE_UNIN_DATA_WRITE_ENABLED 1
#define TRACE_UNIN_DATA_READ_ENABLED 1

#define TRACE_GRACKLE_SET_IRQ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_grackle_set_irq(int irq_num, int level)
{
}

static inline void trace_grackle_set_irq(int irq_num, int level)
{
    if (true) {
        _nocheck__trace_grackle_set_irq(irq_num, level);
    }
}

#define TRACE_SABRE_SET_REQUEST_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_sabre_set_request(int irq_num)
{
}

static inline void trace_sabre_set_request(int irq_num)
{
    if (true) {
        _nocheck__trace_sabre_set_request(irq_num);
    }
}

#define TRACE_SABRE_CLEAR_REQUEST_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_sabre_clear_request(int irq_num)
{
}

static inline void trace_sabre_clear_request(int irq_num)
{
    if (true) {
        _nocheck__trace_sabre_clear_request(irq_num);
    }
}

#define TRACE_SABRE_CONFIG_WRITE_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_sabre_config_write(uint64_t addr, uint64_t val)
{
}

static inline void trace_sabre_config_write(uint64_t addr, uint64_t val)
{
    if (true) {
        _nocheck__trace_sabre_config_write(addr, val);
    }
}

#define TRACE_SABRE_CONFIG_READ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_sabre_config_read(uint64_t addr, uint64_t val)
{
}

static inline void trace_sabre_config_read(uint64_t addr, uint64_t val)
{
    if (true) {
        _nocheck__trace_sabre_config_read(addr, val);
    }
}

#define TRACE_SABRE_PCI_CONFIG_WRITE_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_sabre_pci_config_write(uint64_t addr, uint64_t val)
{
}

static inline void trace_sabre_pci_config_write(uint64_t addr, uint64_t val)
{
    if (true) {
        _nocheck__trace_sabre_pci_config_write(addr, val);
    }
}

#define TRACE_SABRE_PCI_CONFIG_READ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_sabre_pci_config_read(uint64_t addr, uint64_t val)
{
}

static inline void trace_sabre_pci_config_read(uint64_t addr, uint64_t val)
{
    if (true) {
        _nocheck__trace_sabre_pci_config_read(addr, val);
    }
}

#define TRACE_SABRE_PCI_SET_IRQ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_sabre_pci_set_irq(int irq_num, int level)
{
}

static inline void trace_sabre_pci_set_irq(int irq_num, int level)
{
    if (true) {
        _nocheck__trace_sabre_pci_set_irq(irq_num, level);
    }
}

#define TRACE_SABRE_PCI_SET_OBIO_IRQ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_sabre_pci_set_obio_irq(int irq_num, int level)
{
}

static inline void trace_sabre_pci_set_obio_irq(int irq_num, int level)
{
    if (true) {
        _nocheck__trace_sabre_pci_set_obio_irq(irq_num, level);
    }
}

#define TRACE_UNIN_SET_IRQ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_unin_set_irq(int irq_num, int level)
{
}

static inline void trace_unin_set_irq(int irq_num, int level)
{
    if (true) {
        _nocheck__trace_unin_set_irq(irq_num, level);
    }
}

#define TRACE_UNIN_GET_CONFIG_REG_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_unin_get_config_reg(uint32_t reg, uint32_t addr, uint32_t retval)
{
}

static inline void trace_unin_get_config_reg(uint32_t reg, uint32_t addr, uint32_t retval)
{
    if (true) {
        _nocheck__trace_unin_get_config_reg(reg, addr, retval);
    }
}

#define TRACE_UNIN_DATA_WRITE_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_unin_data_write(uint64_t addr, unsigned len, uint64_t val)
{
}

static inline void trace_unin_data_write(uint64_t addr, unsigned len, uint64_t val)
{
    if (true) {
        _nocheck__trace_unin_data_write(addr, len, val);
    }
}

#define TRACE_UNIN_DATA_READ_BACKEND_DSTATE() ( \
    false)

static inline void _nocheck__trace_unin_data_read(uint64_t addr, unsigned len, uint64_t val)
{
}

static inline void trace_unin_data_read(uint64_t addr, unsigned len, uint64_t val)
{
    if (true) {
        _nocheck__trace_unin_data_read(addr, len, val);
    }
}
#endif /* TRACE_HW_PCI_HOST_GENERATED_TRACERS_H */
