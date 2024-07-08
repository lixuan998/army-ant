#include "arch/riscv/include/riscv_proc_defs.h"

void scheduler()
{
    PROC *cur_proc = NULL;
    for (;;)
    {
        cur_proc = proc_list_get_next(&proc_list);
        // printf("cur_proc: %x\n\r", cur_proc);
        interrupt_enable();
        if (cur_proc)
        {
            // printf("prepare to launch process: %s, %x, mark: %c\n\r", cur_proc -> proc_name, cur_proc, ((char *)(cur_proc->shared_mem_addr))[0]);
            if (cur_proc->proc_state == PROC_STATE_READY)
            {
                CPU *cpu = current_cpu();
                cpu->proc = cur_proc;
                cur_proc->proc_state = PROC_STATE_RUNNING;
                context_switch(&current_cpu()->context, &cur_proc->proc_context);
                // printf("process %s's epc: %x\n\r", cur_proc->proc_name, cur_proc->trapframe->epc);
                cpu->proc = NULL;
                interrupt_enable();
            }
            else if(cur_proc->proc_state == PROC_STATE_ZOMBIE)
            {
                printf("Remove Zombine Process: %s\n\r", cur_proc->proc_name);
                proc_list_remove_by_data(&proc_list, cur_proc);
            }
        }
    }
}

void proc_yield()
{
    PROC *cur_proc = current_cpu_proc();
    if (cur_proc->proc_state == PROC_STATE_RUNNING)
    {
        cur_proc->proc_state = PROC_STATE_READY;
        back_to_scheduler();
    }
}

void back_to_scheduler()
{
    PROC *cur_proc = current_cpu_proc();
    context_switch(&cur_proc->proc_context, &(current_cpu()->context));
}