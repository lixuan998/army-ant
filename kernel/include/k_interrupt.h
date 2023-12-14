#ifndef __K_INTERRUPT_H__
#define __K_INTERRUPT_H__

#include "arch/defs.h"
#include "arch/riscv/include/riscv_interrupt.h"
#include "kernel/include/k_stdio.h"

void k_interrupt_init();
void k_interrupt_handler();

#endif /* __K_INTERRUPT_H__ */