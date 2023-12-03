#include "../include/k_spinlock.h"

void k_spinlock_init(K_SPINLOCK *slk, char *name)
{
    slk -> slk_name = name;
    slk -> locked = K_SLK_UNLOCKED;
    slk -> slk_owner = NULL;
}

void k_spinlock_lock(K_SPINLOCK *slk)
{
    int interrupt_status = s_interrupt_status();
    int cpu_id = k_get_cpu_id();
    cpu_list[cpu_id].slk_cnt ++;
    cpu_list[cpu_id].intrrupt_enabled = interrupt_status;
    
    if(slk -> locked && slk -> slk_owner == cpu_list + cpu_id)
    {
        k_panic("k_spinlock_lock");
    }

    while(__sync_lock_test_and_set(&slk->locked, 1) != 0);
    __sync_synchronize();

    slk -> slk_owner = cpu_list + cpu_id;
}

void k_spinlock_unlock(K_SPINLOCK *slk)
{
    int cpu_id = k_get_cpu_id();
    if(!(slk -> locked && slk -> slk_owner == cpu_list + cpu_id))
    {
        k_panic("k_spinlock_unlock");
    }

    slk -> slk_owner = NULL;
    __sync_synchronize();
    __sync_lock_release(&slk->locked);

    cpu_list[cpu_id].slk_cnt --;
    if(cpu_list[cpu_id].slk_cnt == 0 && cpu_list[cpu_id].intrrupt_enabled)
    {
        turn_on_s_interrupt();
    }
}