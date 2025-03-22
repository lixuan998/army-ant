// #include "lib/include/stdio.h"
#include <lib/printk.h>
#include <mm/mmu.h>
#include <common/operation.h>
#include <arch/arch_defs.h>
// #include "bsp/driver/plic/plic.h"
// #include "arch/riscv/include/riscv_type_defs.h"
// #include "arch/riscv/include/riscv_mem_layout.h"
extern char                                 __bss_start[];
extern char                                 __bss_end[];
extern int main();

extern void interrupt_vector();
void test()
{
    printk("test\n\r");
}
void boot_cfg()
{
    //Set MPP to Supervisor mode.
    uint64_t cur_mstatus = r_mstatus();
    printf("mstatus: %x\n\r", cur_mstatus);
    cur_mstatus &= (~(MSTATUS_MPP_MASK));
    cur_mstatus |= (SUPERVISOR_MODE_CODE << MSTATUS_MPP_OFFSET);
    w_mstatus(cur_mstatus);
    w_mepc((uint64_t)main);

    //Stop paging
    sfence_vma();
    w_satp((uint64_t)SATP_BARE_MODE << (uint64_t)RV64_SATP_MODE_OFFSET);
    sfence_vma();


    w_medeleg(0xFFFF);
    w_mideleg(0xFFFF);

    w_sie(r_sie() | SIE_SEIE_MASK | SIE_STIE_MASK | SIE_SSIE_MASK);
    // w_sstatus(r_sstatus() | (1 << 18));
    // plic_s_mode_access();
    w_pmpaddr0(0xFFFFFFFFFFFFFFFFUL);
    w_pmpcfg0(0xF);

    w_tp((uint64_t)r_mhartid());

    asm volatile ("mret");
}