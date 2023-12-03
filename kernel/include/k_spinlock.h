#ifndef __K_SPINLOCK_H__
#define __K_SPINLOCK_H__

#include "k_cpu.h"
#include "k_stdio.h"

#define K_SLK_LOCKED    1
#define K_SLK_UNLOCKED  0

extern K_CPU cpu_list[MAX_CPU_NUM];

typedef struct _K_SPINLOCK {
    char *slk_name;
    int locked;
    K_CPU *slk_owner;
} K_SPINLOCK;

void k_spinlock_init(K_SPINLOCK *slk, char *name);
void k_spinlock_lock(K_SPINLOCK *slk);
void k_spinlock_unlock(K_SPINLOCK *slk);
#endif /* __K_SPINLOCK_H__ */