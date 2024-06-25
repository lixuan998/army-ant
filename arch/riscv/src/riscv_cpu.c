#include "arch/riscv/include/riscv_cpu_defs.h"

CPU cpu_list[CPU_NUM];

CPU * current_cpu()
{
    int cpu_id = get_cpu_id();
    return cpu_list + cpu_id;
}

PROC * current_cpu_proc()
{
    PROC *cur_cpu_proc = current_cpu()->proc;
    return cur_cpu_proc;
}
