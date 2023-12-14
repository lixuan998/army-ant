#ifndef __K_SPINLOCK_H__
#define __K_SPINLOCK_H__

#include "arch/riscv/include/riscv_spinlock.h"

typedef struct _RISCV_SPINLOCK K_SPINLOCK;

void k_spinlock_init(K_SPINLOCK *slk, char *name);
void k_spinlock_lock(K_SPINLOCK *slk);
void k_spinlock_unlock(K_SPINLOCK *slk);

void k_general_spinlock_init();
void k_general_spinlock_lock(int lock_id);
void k_general_spinlock_unlock(int lock_id);
#endif /* __K_SPINLOCK_H__ */