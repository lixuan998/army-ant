#include "arch/riscv/include/riscv_cpu_defs.h"

CPU cpu_list[CPU_NUM];

CPU * current_cpu()
{
    int cpu_id = get_cpu_id();
    return cpu_list + cpu_id;
}

PROC * current_cpu_proc()
{
    CPU *cur_cpu = current_cpu();
    interrupt_disable();
    cur_cpu->interrupt_enabled --;
    PROC *cur_cpu_proc = current_cpu()->proc;
    cur_cpu->interrupt_enabled ++;
    return cur_cpu_proc;
}
