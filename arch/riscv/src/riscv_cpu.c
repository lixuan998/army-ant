#include "../include/riscv_cpu_defs.h"

CPU cpu_list[CPU_NUM] = {
    {.cpu_id = 0,
     .interrupt_enabled = 0,
     .reg_list = {},
     .slk_cnt = 0,
     .proc = NULL
    }
};

CPU * current_cpu()
{
    int cpu_id = get_cpu_id();
    return cpu_list + cpu_id;
}

PROC * current_cpu_proc()
{
    turn_off_s_interrupt();
    PROC *cur_cpu_proc = current_cpu()->proc;
    turn_on_s_interrupt();
    return cur_cpu_proc;
}
