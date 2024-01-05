#include "lib/include/stdio.h"
#include "driver/plic/plic.h"
#include "arch/defs.h"

extern int main();

extern void interrupt_vector();

__attribute__ ((aligned (16))) char kernel_stack[4096 * CPU_NUM];
void boot_cfg()
{
    //clean .bss.
    char *bss_ptr = (char *)bss_start;
    while(bss_ptr < (char *)bss_end)
    {
        *bss_ptr = NULL;
        bss_ptr ++;
    }
    //Set MPP to Supervisor mode.
    isa_reg_t cur_mstatus = r_mstatus();
    cur_mstatus &= (~(MSTATUS_MPP_MASK));
    cur_mstatus |= (SUPERVISOR_MODE_CODE << MSTATUS_MPP_OFFSET);
    w_mstatus(cur_mstatus);
    w_mepc((isa_reg_t)main);

    //Stop paging
    sfence_vma();
    w_satp((isa_reg_t)SATP_BARE_MODE << (isa_reg_t)RV64_SATP_MODE_OFFSET);
    sfence_vma();


    w_medeleg(0xFFFF);
    w_mideleg(0xFFFF);

    w_sie(r_sie() | SIE_SEIE_MASK | SIE_STIE_MASK | SIE_SSIE_MASK);
    plic_s_mode_access();
    w_pmpaddr0(0xFFFFFFFFFFFFFFFFUL);
    w_pmpcfg0(0xF);

    isa_reg_t tp_arr[50];
    tp_arr[0] = r_mhartid();
    w_tp((isa_reg_t)&tp_arr);

    asm volatile ("mret");
}