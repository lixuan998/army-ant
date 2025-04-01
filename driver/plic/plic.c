#include "plic.h"

void plic_interrupt_enable()
{
    for(int i = 1; i < PLIC_INTERRUPT_MAX; ++i)
    {
        WRITE_REG32(PLIC_PRIO_REG(i), PLIC_ENABLE);
    }
    for(int i = 0; i < 10; ++i)
    {
        WRITE_REG32(PLIC_SIE_REG(i), 0xFFFFFFFF);
    }
}

void plic_interrupt_disable()
{
    for(int i = 1; i < PLIC_INTERRUPT_MAX; ++i)
    {
        WRITE_REG32(PLIC_PRIO_REG(i), PLIC_DISABLE);
    }
    for(int i = 0; i < 10; ++i)
    {
        WRITE_REG32(PLIC_SIE_REG(i), PLIC_DISABLE);
    }
}

int plic_interrupt_source()
{
    return READ_REG32(PLIC_SCLAIM_REG);
}

void plic_interrupt_handled(int source)
{
    WRITE_REG32(PLIC_SCLAIM_REG, source);
}