#include "kernel/include/k_interrupt.h"

void k_interrupt_init()
{
    interrupt_init();
}

void k_interrupt_handler()
{
    interrupt_handler();
}