#ifndef __RISCV_SPINLOCK_DEFS_H__
#define __RISCV_SPINLOCK_DEFS_H__

#include "riscv_type_defs.h"

#define GENERAL_SPINLOCK_NUM        5
// forward declaration of struct _CPU.
typedef struct _CPU CPU;
#define SLK_LOCKED                            1
#define SLK_UNLOCKED                          0

typedef struct SPINLOCK {
    char *slk_name;
    int locked;
    CPU *slk_owner;
} SPINLOCK;

void spinlock_init(SPINLOCK *slk, char *name);
void spinlock_lock(SPINLOCK *slk);
void spinlock_unlock(SPINLOCK *slk);

extern SPINLOCK general_spinlock[GENERAL_SPINLOCK_NUM];

#endif  /* __RISCV_SPINLOCK_DEFS_H__ */