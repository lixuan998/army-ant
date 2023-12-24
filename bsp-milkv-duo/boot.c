#include "arch/riscv/include/riscv_asm_operation.h"
#include "kernel/include/k_stdio.h"
#include "arch/defs.h"
#include "kernel/include/k_vm.h"
#include "kernel/include/k_paging.h"
#include "driver/plic/plic.h"

extern int main();

extern void k_interrupt_vector();

__attribute__ ((aligned (16))) char stack0[4096 * 4];
void boot_cfg()
{
    w_sstatus((uint64)0x200000100);
    w_mstatus((uint64)0xA00000188);
    k_printf("SSTATUS1: %x\n\r", r_sstatus());
    k_printf("MSTATUS1: %x\n\r", r_mstatus());
    //Set MPP to Supervisor mode.
    volatile uint64 cur_mstatus = r_mstatus();
    k_printf("cur_mstatus1: %x\n\r", cur_mstatus);
    cur_mstatus &= (~(MSTATUS_MPP_MASK));
    cur_mstatus |= (SUPERVISOR_MODE_CODE << MSTATUS_MPP_OFFSET);
    k_printf("cur_mstatus2: %x\n\r", cur_mstatus);
    w_mstatus(cur_mstatus);
    w_mepc((uint64)main);

    k_printf("SSTATUS2: %x\n\r", r_sstatus());
    k_printf("MSTATUS2: %x\n\r", r_mstatus());

    //Stop paging
    sfence_vma();
    w_satp((uint64)SATP_BARE_MODE << (uint64)RV64_SATP_MODE_OFFSET);
    sfence_vma();


    w_medeleg(0xFFFF);
    w_mideleg(0xFFFF);

    w_sie(r_sie() | SIE_SEIE_MASK | SIE_STIE_MASK | SIE_SSIE_MASK);
    plic_s_mode_access();
    w_pmpaddr0(0xFFFFFFFFFFFFFFFFUL);
    w_pmpcfg0(0xF);

    uint64 tp_arr[50];
    k_printf("HARDID: %x\n\r", r_mhartid());
    tp_arr[0] = r_mhartid();
    w_tp((uint64)&tp_arr);

    asm volatile ("mret");
}