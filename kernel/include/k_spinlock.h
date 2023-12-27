#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include "bsp/init.h"

void k_spinlock_init(SPINLOCK *slk, char *name);
void k_spinlock_lock(SPINLOCK *slk);
void k_spinlock_unlock(SPINLOCK *slk);

#endif /* __SPINLOCK_H__ */