// #include "lib/include/stdio.h"
#include <lib/printk.h>
#include <mm/mmu.h>
#include <common/operation.h>
#include <common/log.h>
#include <arch/arch_defs.h>
#include <plat/timer/plat_timer.h>

extern int main();
extern void mmu_map_init(void);
extern void interrupt_vector();

void boot_cfg()
{
    //Set MPP to Supervisor mode.
    uint64_t cur_mstatus = READ_CSR(mstatus);
    KLOG_DEBUG("BOOTCFG", "mstatus: %x", cur_mstatus); 
    cur_mstatus &= (~(MSTATUS_MPP_MSK));
    cur_mstatus |= (SUPERVISOR_MODE_CODE << MSTATUS_MPP_OFFSET);
    WRITE_CSR(mstatus, cur_mstatus);
    WRITE_CSR(mepc, (uintptr_t)main);

    //Set MMU Mapping.
    mmu_map_init();

    WRITE_CSR(medeleg, 0xFFFF);
    WRITE_CSR(mideleg, 0xFFFF);

    WRITE_CSR(sie, READ_CSR(sie) | xIE_SEIE_MSK | xIE_STIE_MSK | xIE_SSIE_MSK);
    // w_sstatus(r_sstatus() | (1 << 18));
    // plic_s_mode_access();
    WRITE_CSR(pmpaddr0, 0xFFFFFFFFFFFFFFFFUL);
    WRITE_CSR(pmpcfg0, 0xF);

    plat_timer_init();
    // Store hartid in tp register.
    WRITE_GPR(tp, (uintptr_t)READ_CSR(mhartid));

    asm volatile ("mret");
}