#include "../include/riscv_cpu_defs.h"

CPU cpu_list[CPU_NUM] = {
    {.cpu_id = 0,
     .interrupt_enabled = 0,
     .reg_list = {},
     .slk_cnt = 0
    //  .proc = NULL
    }
};

CPU * current_cpu()
{
    int cpu_id = get_cpu_id();
    return cpu_list + cpu_id;
}

// PROC * current_cpu_proc()
// {
//     return current_cpu()->proc;
// }
