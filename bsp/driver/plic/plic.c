#include "plic.h"


void plic_interrupt_enable()
{
    for(int i = 1; i < PLIC_INTERRUPT_MAX; ++ i)
    {
        write_reg32(PLIC_PRIO_REG(i), PLIC_ENABLE);
    }
    for(int i = 0; i < 10; ++ i)
    {
        write_reg32(PLIC_SIE_REG(i), 0xFFFFFFFF);
    }
}

void plic_interrupt_disable()
{
    for(int i = 1; i < PLIC_INTERRUPT_MAX; ++ i)
    {
        write_reg32(PLIC_PRIO_REG(i), PLIC_DISABLE);
    }
    for(int i = 0; i < 10; ++ i)
    {
        write_reg32(PLIC_SIE_REG(i), PLIC_DISABLE);
    }
}

void plic_s_mode_access()
{
    //Give access to supervisor mode to access PLIC registers.
    write_reg32(PLIC_CTRL_REG, PLIC_CTRL_REG_S_MODE_ACCESS);
}