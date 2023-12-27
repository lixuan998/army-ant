#include "../include/k_spinlock.h"

void k_spinlock_init(SPINLOCK *slk, char *name)
{
    spinlock_init(slk, name);
}

void k_spinlock_lock(SPINLOCK *slk)
{
    spinlock_lock(slk);
}

void k_spinlock_unlock(SPINLOCK *slk)
{
    spinlock_unlock(slk);
}