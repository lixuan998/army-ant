#include <arch/irq/arch_irq.h>
#include <plat/timer/plat_timer.h>
#include <lib/printk.h>

void arch_irq_vec_tbl(void);

void arch_irq_init(void)
{
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
    plat_timer_irq_handler();
}