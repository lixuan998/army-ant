#include "../arch/riscv/include/asm_operation.h"
#include "../kernel/include/k_stdio.h"
#include "../arch/defs.h"
#include "../kernel/include/k_vm.h"
#include "../kernel/include/k_paging.h"
extern int main();

__attribute__ ((aligned (16))) char stack0[4096 * 4];
void boot_cfg()
{
    //Set MPP to Supervisor mode.
    uint64 cur_mstatus = r_mstatus();
    cur_mstatus &= (~(MSTATUS_MPP_MASK));
    cur_mstatus |= (SUPERVISOR_MODE_CODE << MSTATUS_MPP_OFFSET);
    w_mstatus(cur_mstatus);
    w_mepc((uint64)main);

    //Stop paging
    sfence_vma();
    w_satp((uint64)SATP_BARE_MODE << (uint64)RV64_SATP_MODE_OFFSET);
    sfence_vma();


    w_medeleg(0xFFFF);
    w_mideleg(0xFFFF);

    w_sie(r_sie() | SIE_SEIE_MASK | SIE_STIE_MASK | SIE_SSIE_MASK);
    
    w_pmpaddr0(0xFFFFFFFFFFFFFFFFUL);
    w_pmpcfg0(0xF);

    uint64 tp_arr[50];
    tp_arr[0] = r_mhartid();
    w_tp((uint64)&tp_arr);

    asm volatile ("mret");
}