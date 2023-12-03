#include "../include/k_cpu.h"

K_CPU cpu_list[MAX_CPU_NUM];

int k_get_cpu_id()
{
    int cpu_id = ((uint64 *)r_tp())[0];
    return cpu_id;
}