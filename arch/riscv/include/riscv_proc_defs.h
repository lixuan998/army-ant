#ifndef __RISCV_PROC_DEFS_H__
#define __RISCV_PROC_DEFS_H__

#include "riscv_type_defs.h"
#include "riscv_vm_defs.h"
#include "riscv_spinlock_defs.h"

#include "lib/include/stdlib.h"

enum PROC_STATE{
    PROC_STATE_RUNNING,
    PROC_STATE_READY,
    PROC_STATE_WAITING,
    PROC_STATE_EXIT,
    PROC_STATE_SLEEPING,
    PROC_STATE_ZOMBIE,
};

enum PROC_PRIORITY{
    PROC_PRIORITY_LOW,
    PROC_PRIORITY_MEDIUM,
    PROC_PRIORITY_HIGH,
};

typedef struct _SYS_REGS{
    isa_reg_t k_satp;  // pagetable in kernel mode.
    isa_reg_t k_sp;    // used in kernel mode for processes when they trap to the kernel mode.
    isa_reg_t epc;     // saved user program counter.
    isa_reg_t ra;
    isa_reg_t sp;
    isa_reg_t gp;
    isa_reg_t tp;
    isa_reg_t t0;
    isa_reg_t t1;
    isa_reg_t t2;
    isa_reg_t s0;
    isa_reg_t s1;
    isa_reg_t a0;
    isa_reg_t a1;
    isa_reg_t a2;
    isa_reg_t a3;
    isa_reg_t a4;
    isa_reg_t a5;
    isa_reg_t a6;
    isa_reg_t a7;
    isa_reg_t s2;
    isa_reg_t s3;
    isa_reg_t s4;
    isa_reg_t s5;
    isa_reg_t s6;
    isa_reg_t s7;
    isa_reg_t s8;
    isa_reg_t s9;
    isa_reg_t s10;
    isa_reg_t s11;
    isa_reg_t t3;
    isa_reg_t t4;
    isa_reg_t t5;
    isa_reg_t t6;
} SYS_REGS;

typedef struct _CONTEXT{
    isa_reg_t ra;
    isa_reg_t sp;
    isa_reg_t s0;
    isa_reg_t s1;
    isa_reg_t s2;
    isa_reg_t s3;
    isa_reg_t s4;
    isa_reg_t s5;
    isa_reg_t s6;
    isa_reg_t s7;
    isa_reg_t s8;
    isa_reg_t s9;
    isa_reg_t s10;
    isa_reg_t s11;
} CONTEXT;

enum proc_state{
    PROC_SLEEP,
    PROC_RUNNABLE,
    PROC_RUNNING,
    PROC_ZOMBIE,
};

typedef struct _PROC{
    SPINLOCK lock;

    void *sleep_for;
    int child_exit_ret;
    int pid;
    enum PROC_STATE proc_state;

    struct _PROC *parent;

    pagetable_t pagetable;
    addr_t kernel_stack;
    addr_t user_stack;
    int proc_mem_size;
    SYS_REGS *system_regs;
    CONTEXT proc_context;
    MEM_PAGE *pages[100];
    MEM_BLK *mem_blk_head;

    enum PROC_PRIORITY priority;

    char proc_name[30];

} PROC;

PROC *proc_create(PROC *parent, char *name, enum PROC_PRIORITY priority);
void exec(PROC *proc, char *code, int code_size);
void proc_ret();
void init_proc();

void scheduler();
void back_to_scheduler();
#endif /* __RISCV_PROC_DEFS_H__ */