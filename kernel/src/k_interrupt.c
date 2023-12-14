#include "kernel/include/k_interrupt.h"

void k_interrupt_init()
{
    riscv_interrupt_init();
}

void k_interrupt_handler()
{
    riscv_interrupt_handler();
}