#pragma once

#include <common/operation.h>
#include <lib/stddef.h>

#define IRQ_HANDLER_SET_NUM       (10)
#define EXCEPTION_HANDLER_SET_NUM (16)
enum irq_type {
    SOFTWARE_IRQ = 1,
    TIMER_IRQ    = 5,
    EXTERNAL_IRQ = 9
};

enum exception_type {
    INSTRUCTION_ADDRESS_MISALIGNED = 0,
    INSTRUCTION_ACCESS_FAULT      = 1,
    ILLEGAL_INSTRUCTION           = 2,
    BREAKPOINT                    = 3,
    LOAD_ADDRESS_MISALIGNED       = 4,
    LOAD_ACCESS_FAULT             = 5,
    STORE_ADDRESS_MISALIGNED      = 6,
    STORE_ACCESS_FAULT            = 7,
    USER_ECALL                    = 8,
    SUPERVISOR_ECALL              = 9,
    INSTRUCTION_PAGE_FAULT        = 12,
    LOAD_PAGE_FAULT               = 13,
    STORE_PAGE_FAULT              = 15
};

typedef void (*irq_handler_t)(void);
typedef void (*exception_handler_t)(void);

void arch_irq_init(void);
void arch_irq_enable(void);
void arch_irq_disable(void);
uint8_t arch_irq_lock(void);
void arch_irq_unlock(uint8_t prev);