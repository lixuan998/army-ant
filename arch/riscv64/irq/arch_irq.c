#include <arch/irq/arch_irq.h>
#include <plat/timer/plat_timer.h>
#include <lib/printk.h>
#include <common/log.h>

static irq_handler_t irq_handler_set[IRQ_HANDLER_SET_NUM] = {0};
static exception_handler_t exception_handler_set[EXCEPTION_HANDLER_SET_NUM] = {0};

static void default_irq_handler(void)
{
    PANIC("Unrecognized IRQ\r\n");
}

static void default_exception_handler(void)
{
    PANIC("Unrecognized Exception\r\n");
}

static void software_irq_handler(void)
{
    KLOG_DEBUG("IRQ_HANDLER", "Software IRQ Not Implemented\r\n");
    while(1);
}

static void timer_irq_handler(void)
{
    plat_timer_irq_handler();
}

static void external_irq_handler(void)
{
    KLOG_DEBUG("IRQ_HANDLER", "External IRQ Not Implemented\r\n");
    while(1);
}
static void instruction_address_misaligned_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Instruction Address Misaligned\r\n");
    while(1);
}
static void instruction_access_fault_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Instruction Access Fault\r\n");
    while(1);
}
static void illegal_instruction_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Illegal Instruction\r\n");
    while(1);
}
static void breakpoint_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Breakpoint\r\n");
    while(1);
}
static void load_address_misaligned_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Load Address Misaligned\r\n");
    while(1);
}

static void load_access_fault_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Load Access Fault\r\n");
    while(1);
}

static void store_address_misaligned_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Store Address Misaligned\r\n");
    while(1);
}

static void store_access_fault_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Store Access Fault\r\n");
    while(1);
}

static void user_ecall_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "User Environment Call\r\n");
    while(1);
}

static void supervisor_ecall_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Supervisor Environment Call\r\n");
    while(1);
}

static void instruction_page_fault_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Instruction Page Fault\r\n");
    while(1);
}

static void load_page_fault_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Load Page Fault\r\n");
    while(1);
}

static void store_page_fault_handler(void)
{
    KLOG_DEBUG("EXCEPTION_HANDLER", "Store Page Fault\r\n");
    while(1);
}

void arch_irq_vec_tbl(void);

void arch_irq_init(void)
{
    if (irq_handler_set[0] == NULL) {
        // Set default handlers
        for (int i = 0; i < IRQ_HANDLER_SET_NUM; i++) {
            irq_handler_set[i] = default_irq_handler;
        }
        // Set IRQ handlers
        irq_handler_set[SOFTWARE_IRQ]   = software_irq_handler;
        irq_handler_set[TIMER_IRQ]      = timer_irq_handler;
        irq_handler_set[EXTERNAL_IRQ]   = external_irq_handler;
    }
    if (exception_handler_set[0] == NULL) {
        // Set default handlers
        for (int i = 0; i < EXCEPTION_HANDLER_SET_NUM; i++) {
            exception_handler_set[i] = default_exception_handler;
        }
        // Set Exception handlers
        exception_handler_set[INSTRUCTION_ADDRESS_MISALIGNED]   = instruction_address_misaligned_handler;
        exception_handler_set[INSTRUCTION_ACCESS_FAULT]         = instruction_access_fault_handler;
        exception_handler_set[ILLEGAL_INSTRUCTION]              = illegal_instruction_handler;
        exception_handler_set[BREAKPOINT]                       = breakpoint_handler;
        exception_handler_set[LOAD_ADDRESS_MISALIGNED]          = load_address_misaligned_handler;
        exception_handler_set[LOAD_ACCESS_FAULT]                = load_access_fault_handler;
        exception_handler_set[STORE_ADDRESS_MISALIGNED]         = store_address_misaligned_handler;
        exception_handler_set[STORE_ACCESS_FAULT]               = store_access_fault_handler;
        exception_handler_set[USER_ECALL]                       = user_ecall_handler;
        exception_handler_set[SUPERVISOR_ECALL]                 = supervisor_ecall_handler;
        exception_handler_set[INSTRUCTION_PAGE_FAULT]           = instruction_page_fault_handler;
        exception_handler_set[LOAD_PAGE_FAULT]                  = load_page_fault_handler;
        exception_handler_set[STORE_PAGE_FAULT]                 = store_page_fault_handler;
    }
    // Set the exception vector table
    WRITE_CSR(stvec, (uint64_t)arch_irq_vec_tbl);
}

void arch_irq_enable(void)
{
    WRITE_CSR(sstatus, READ_CSR(sstatus) | SSTATUS_SIE_MSK);
}

void arch_irq_disable(void)
{
    WRITE_CSR(sstatus, READ_CSR(sstatus) & ~SSTATUS_SIE_MSK);
}

uint8_t arch_irq_lock(void)
{
    uint8_t prev = (READ_CSR(sstatus) & SSTATUS_SIE_MSK) >> SSTATUS_SIE_OFFSET;
    arch_irq_disable();
    return prev;
}

void arch_irq_unlock(uint8_t prev)
{
    if (prev) {
        arch_irq_enable();
    }
}

void arch_irq_handler()
{
    uintptr_t scause_val = READ_CSR(scause);
    uint8_t is_irq = (scause_val & SCAUSE_INTERRUPT_MSK) >> SCAUSE_INTERRUPT_OFFSET;
    if (is_irq) {
        enum irq_type type = (scause_val & SCAUSE_EXCEPTION_CODE_MSK);
        irq_handler_set[type]();
    } else {
        enum exception_type type = (scause_val & SCAUSE_EXCEPTION_CODE_MSK);
        exception_handler_set[type]();
    }
}