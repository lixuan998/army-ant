#ifndef __RISCV_SPINLOCK_DEFS_H__
#define __RISCV_SPINLOCK_DEFS_H__

#include "riscv_cpu_defs.h"

void spinlock_init(SPINLOCK *slk, char *name);
void spinlock_lock(SPINLOCK *slk);
void spinlock_unlock(SPINLOCK *slk);

extern SPINLOCK general_spinlock[GENERAL_SPINLOCK_NUM];

#endif  /* __RISCV_SPINLOCK_DEFS_H__ */