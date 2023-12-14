#include "../include/riscv_spinlock.h"

RISCV_SPINLOCK general_spinlocks[10];

void riscv_spinlock_init(RISCV_SPINLOCK *slk, char *name)
{
    slk -> slk_name = name;
    slk -> locked = SLK_UNLOCKED;
    slk -> slk_owner = NULL;
}

void riscv_spinlock_lock(RISCV_SPINLOCK *slk)
{
    int interrupt_status = s_interrupt_status();
    int cpu_id = get_cpu_id();
    cpu_list[cpu_id].slk_cnt ++;
    cpu_list[cpu_id].intrrupt_enabled = interrupt_status;
    
    // if(slk -> locked && slk -> slk_owner == cpu_list + cpu_id)
    // {
    //     k_panic(__FILE__, __LINE__, "riscv_spinlock_lock");
    // }

    while(__sync_lock_test_and_set(&slk->locked, 1) != 0);
    __sync_synchronize();

    slk -> slk_owner = cpu_list + cpu_id;

    riscv_interrupt_disable();
}

void riscv_spinlock_unlock(RISCV_SPINLOCK *slk)
{
    int cpu_id = get_cpu_id();
    // if(!(slk -> locked && slk -> slk_owner == cpu_list + cpu_id))
    // {
    //     k_panic(__FILE__, __LINE__, "riscv_spinlock_unlock");
    // }

    slk -> slk_owner = NULL;
    __sync_synchronize();
    __sync_lock_release(&slk->locked);

    cpu_list[cpu_id].slk_cnt --;
    if(cpu_list[cpu_id].slk_cnt == 0 && cpu_list[cpu_id].intrrupt_enabled)
    {
        riscv_interrupt_enable();
    }
}

void riscv_general_spinlock_init()
{
    for(int i = 0; i < GENERAL_SPINLOCK_NUM; ++ i)
    {
        char name[30];
        k_sprintf(name, "general_spinlock_%d", i);
        riscv_spinlock_init(general_spinlocks + i, name);
    }
}

void riscv_general_spinlock_lock(int lock_id)
{
    riscv_spinlock_lock(general_spinlocks + lock_id);
}

void riscv_general_spinlock_unlock(int lock_id)
{
    riscv_spinlock_unlock(general_spinlocks + lock_id);
}