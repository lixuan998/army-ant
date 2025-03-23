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
    uint64_t cur_mstatus = READ_CSR(mstatus);
    printf("mstatus: %x\n\r", cur_mstatus);
    cur_mstatus &= (~(MSTATUS_MPP_MASK));
    cur_mstatus |= (SUPERVISOR_MODE_CODE << MSTATUS_MPP_OFFSET);
    WRITE_CSR(mstatus, cur_mstatus);
    WRITE_CSR(mepc, (uintptr_t)main);

    //Stop paging
    WRITE_CSR(satp, (uintptr_t)SATP_BARE_MODE << RV64_SATP_MODE_OFFSET);
    INVALIDATE_TLB();


    WRITE_CSR(medeleg, 0xFFFF);
    WRITE_CSR(mideleg, 0xFFFF);

    WRITE_CSR(sie, READ_CSR(sie) | SIE_SEIE_MASK | SIE_STIE_MASK | SIE_SSIE_MASK);
    // w_sstatus(r_sstatus() | (1 << 18));
    // plic_s_mode_access();
    WRITE_CSR(pmpaddr0, 0xFFFFFFFFFFFFFFFFUL);
    WRITE_CSR(pmpcfg0, 0xF);

    WRITE_GPR(tp, (uintptr_t)READ_CSR(mhartid));

    asm volatile ("mret");
}