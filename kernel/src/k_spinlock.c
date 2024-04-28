#include "arch/defs.h"
#include "lib/include/stdio.h"

SPINLOCK general_spinlock[GENERAL_SPINLOCK_NUM];

void spinlock_init(SPINLOCK *slk, char *name)
{
    slk -> slk_name = name;
    slk -> locked = SLK_UNLOCKED;
    slk -> slk_owner = NULL;
}

void spinlock_lock(SPINLOCK *slk)
{
    int interrupt_status = s_interrupt_status();

    CPU *cur_cpu = current_cpu();
    cur_cpu -> slk_cnt ++;
    if(!cur_cpu -> interrupt_enabled)
    {
        cur_cpu -> interrupt_enabled = interrupt_status;
    }
    
    if(slk -> locked && slk -> slk_owner == cur_cpu)
    {
        panic("riscv_spinlock_lock");
    }

    while(__sync_lock_test_and_set(&slk->locked, 1) != 0);
    __sync_synchronize();

    slk -> slk_owner = cur_cpu;

    interrupt_disable();
}

void spinlock_unlock(SPINLOCK *slk)
{
    CPU *cur_cpu = current_cpu();
    if(!(slk -> locked && slk -> slk_owner == cur_cpu))
    {
        panic("riscv_spinlock_unlock");
    }

    slk -> slk_owner = NULL;
    __sync_synchronize();
    __sync_lock_release(&slk->locked);

    cur_cpu -> slk_cnt --;
    if(cur_cpu -> slk_cnt == 0 && cur_cpu -> interrupt_enabled)
    {
        interrupt_enable();
    }
}