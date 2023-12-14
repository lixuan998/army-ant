#ifndef __RISCV_SPINLOCK_H__
#define __RISCV_SPINLOCK_H__

#include "riscv_cpu.h"
#include "riscv_interrupt.h"
#include "kernel/include/k_stdio.h"

#define SLK_LOCKED    1
#define SLK_UNLOCKED  0

#define GENERAL_SPINLOCK_NUM 10
extern CPU cpu_list[MAX_CPU_NUM];

typedef struct _RISCV_SPINLOCK {
    char *slk_name;
    int locked;
    CPU *slk_owner;
} RISCV_SPINLOCK;

void riscv_spinlock_init(RISCV_SPINLOCK *slk, char *name);
void riscv_spinlock_lock(RISCV_SPINLOCK *slk);
void riscv_spinlock_unlock(RISCV_SPINLOCK *slk);

void riscv_general_spinlock_init();
void riscv_general_spinlock_lock(int lock_id);
void riscv_general_spinlock_unlock(int lock_id);

#endif /* __RISCV_SPINLOCriscv_H__ */