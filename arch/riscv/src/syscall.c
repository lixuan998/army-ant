#include "../include/riscv_syscall_defs.h"

static int syscall_func_count = 1;

static uint64 (*syscall_func[])(void) = {
    [SYSCALL_EXIT] exit};

void syscall()
{
    int syscall_num;
    PROC *cur_proc = current_cpu_proc();

    syscall_num = cur_proc->trapframe->a7;
    if (syscall_num < syscall_func_count && syscall_func[syscall_num])
    {
        cur_proc->trapframe->a0 = syscall_func[syscall_num]();
    }
    else
    {
        printf("%d %s: unknown sys call %d\n",
               cur_proc->pid, cur_proc->proc_name, syscall_num);
        cur_proc->trapframe->a0 = -1;
    }
}
