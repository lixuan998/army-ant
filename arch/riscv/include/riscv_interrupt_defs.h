#ifndef __RISCV_INTERRUPT_DEFS_H__
#define __RISCV_INTERRUPT_DEFS_H__

#include "riscv_cpu_defs.h"
#include "riscv_proc_defs.h"
#include "riscv_syscall_defs.h"

extern void kernel_interrupt_vector();

void interrupt_init(void *interrupt_vector);
void interrupt_enable();
void interrupt_disable();
void kernel_interrupt_handler();
void user_interrupt_handler();
uint32 interrupt_handler();

#endif  /* __RISCV_INTERRUPT_DEFS_H__ */