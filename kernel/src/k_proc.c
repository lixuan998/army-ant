#include "arch/defs.h"

SPINLOCK *pid_lock = general_spinlock;
int next_pid = 1;

PROC *proc_create(PROC *parent, char *name, int priority)
{
    PROC *new_proc = (PROC *)malloc(sizeof(PROC));
    spinlock_init(&(new_proc -> lock), "proc_spinlock");
    spinlock_lock(&(new_proc -> lock));
    
    //  allocate pid for new process.
    spinlock_lock(pid_lock);
    new_proc -> pid = next_pid ++;
    spinlock_unlock(pid_lock);

    
}