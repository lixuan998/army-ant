// #include "lib/include/stdio.h"
#include <lib/printk.h>
#include <io/uart.h>
#include <mm/mm.h>
#include <common/operation.h>
#include <common/logo.h>
#include <common/log.h>

#include <irq/irq.h>
#include <arch/arch_defs.h>
#include <arch/atomic.h>
#include <plat/timer/plat_timer.h>
#include <plat/plic/plat_plic.h>

extern void mmu_map_init(void);
extern void interrupt_vector();

static volatile uint8_t smp_ready = 0;

void boot_core_init(void)
{
    uart_init();
    KLOG_INFO("INITIALIZING", "Initializing system...");
    KLOG_DEBUG("HARTID", "hartid: %d", READ_HARTID());
    mm_init();
    // plic_interrupt_enable();
    irq_init();
    print_logo();
    irq_enable();
    smp_ready = 1;
    while(1);
}

void smp_core_init(void)
{
    while (smp_ready == 0) {
        asm volatile ("nop");
    }
    KLOG_DEBUG("HARTID", "HARTID: %d", READ_HARTID());
    secondary_mm_init();
    irq_init();
    irq_enable();
    while(1);
}

void boot_cfg()
{
    //Set MPP to Supervisor mode.
    uintptr_t cur_mstatus = READ_CSR(mstatus);
    cur_mstatus &= (~(MSTATUS_MPP_MSK));
    cur_mstatus |= (SUPERVISOR_MODE_CODE << MSTATUS_MPP_OFFSET);
    WRITE_CSR(mstatus, cur_mstatus);

    if (READ_CSR(mhartid) == BOOT_CORE) {
        WRITE_CSR(mepc, (uintptr_t)boot_core_init);
    } else {
        WRITE_CSR(mepc, (uintptr_t)smp_core_init);
    }

    //Set MMU Mapping.
    mmu_disable();
    mmu_map_init();

    WRITE_CSR(medeleg, 0xFFFF);
    WRITE_CSR(mideleg, 0xFFFF);
    WRITE_CSR(sie, READ_CSR(sie) | xIE_SEIE_MSK | xIE_STIE_MSK | xIE_SSIE_MSK);
    
    // plic_s_mode_access();
    WRITE_CSR(pmpaddr0, 0xFFFFFFFFFFFFFFFFUL);
    WRITE_CSR(pmpcfg0, 0xF);

    plat_timer_init();
    // Store hartid in tp register.
    WRITE_GPR(tp, (uintptr_t)READ_CSR(mhartid));

    asm volatile ("mret");
}