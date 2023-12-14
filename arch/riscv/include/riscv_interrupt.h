#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "arch/defs.h"
#include "riscv_asm_operation.h"
#include "bsp/driver/plic/plic.h"
#include "bsp/driver/timer/timer.h"
#include "riscv_spinlock.h"
#include "console/include/console.h"
#include "kernel/include/k_time.h"

extern void k_interrupt_vector();

void riscv_interrupt_init();
void riscv_interrupt_enable();
void riscv_interrupt_disable();
uint32 riscv_get_interrupt_source();
void riscv_interrupt_handler();
void riscv_external_interrupt_handler();
void riscv_external_interrupt_source_done(int source);
#endif /* __INTERRUPT_H__ */