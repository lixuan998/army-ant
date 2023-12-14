#include "../include/k_spinlock.h"

void k_spinlock_init(K_SPINLOCK *slk, char *name)
{
    riscv_spinlock_init(slk, name);
}

void k_spinlock_lock(K_SPINLOCK *slk)
{
    riscv_spinlock_lock(slk);
}

void k_spinlock_unlock(K_SPINLOCK *slk)
{
    riscv_spinlock_unlock(slk);
}

void k_general_spinlock_init()
{
    riscv_general_spinlock_init();
}

void k_general_spinlock_lock(int lock_id)
{
    riscv_general_spinlock_lock(lock_id);
}

void k_general_spinlock_unlock(int lock_id)
{
    riscv_general_spinlock_unlock(lock_id);
}