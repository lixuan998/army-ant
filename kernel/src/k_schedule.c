#include "arch/riscv/include/riscv_proc_defs.h"

void scheduler()
{
    PROC *cur_proc = NULL;
    for (;;)
    {
        cur_proc = proc_list_get_next(&proc_list);  // 选择当前优先级最高的进程
        interrupt_enable();                         // 开启中断
        if (cur_proc)
        {
            if (cur_proc->proc_state == PROC_STATE_READY) // 判断进程是否处于就绪状态
            {
                CPU *cpu = current_cpu(); // 获取当前CPU结构体
                cpu->proc = cur_proc;
                cur_proc->proc_state = PROC_STATE_RUNNING; // 将进程状态设置为运行态
                context_switch(&current_cpu()->context, &cur_proc->proc_context); // 切换上下文
                cpu->proc = NULL;
                interrupt_enable(); // 开启中断
            }
            else if(cur_proc->proc_state == PROC_STATE_ZOMBIE) // 判断进程是否处于僵尸态
            {
                printf("Remove Zombine Process: %s\n\r", cur_proc->proc_name);
                proc_list_remove_by_data(&proc_list, cur_proc); // 从进程列表中移除僵尸进程
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