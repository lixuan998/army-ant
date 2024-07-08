#ifndef __RISCV_PROC_DEFS_H__
#define __RISCV_PROC_DEFS_H__

#include "riscv_vm_defs.h"
#include "riscv_spinlock_defs.h"
#include "riscv_interrupt_defs.h"

#include "lib/include/stdlib.h"

extern PROC_LIST proc_list;
extern addr_t ret_to_user[];

void proc_list_add(PROC_LIST *list, PROC *proc);
PROC *proc_list_get_next(PROC_LIST *list);
void proc_list_pop(PROC_LIST *list);
void proc_list_remove(PROC_LIST *list, int proc_idx);
void proc_list_remove_by_data(PROC_LIST *list, PROC *proc);

PROC *proc_create(PROC *parent, char *name, enum PROC_PRIORITY priority);
void proc_destroy(PROC *proc);

void exec(PROC *proc, unsigned char *code, int code_size);
uint64 exit();
void proc_ret();
void kernel_trap();
void user_trap();
void init_proc();

void scheduler();

void proc_yield();
void back_to_scheduler();
#endif /* __RISCV_PROC_DEFS_H__ */