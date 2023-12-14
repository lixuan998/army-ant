#include "arch/riscv/include/riscv_cpu.h"

CPU cpu_list[MAX_CPU_NUM];

int get_cpu_id()
{
    int cpu_id = ((uint64 *)r_tp())[0];
    return cpu_id;
}