#include "timer.h"

void timer_interrupt_enable(int n)
{
    write_reg32(TMR_IRQ_EN_REG, (read_reg32(TMR_IRQ_EN_REG) | (1 << n)));
}

void timer_pending_clear(int n)
{
    write_reg32(TMR_IRQ_STA_REG, 1 << n);
}

void timer_set_prescale(int n, int prescale)
{
    write_reg32(TMRn_CTRL_REG(n), read_reg32(TMRn_CTRL_REG(n)) | prescale << TMRn_CLK_PRES_OFFSET);
}

volatile void timer_start(int n, int tick)
{
    k_printf("\n\r");
    write_reg32(TMRn_INTV_VALUE_REG(n), read_reg32(TMRn_INTV_VALUE_REG(n)) | TIMER_FREQUENCY / tick);
    volatile uint32 tmr_ctrl_val = read_reg32(TMRn_CTRL_REG(n));
    timer_set_prescale(n, TMRn_CTRL_PRES_DIVEDE_2);
    tmr_ctrl_val |= 1 << TMRn_CTRL_RELOAD_OFFSET;
    
    write_reg32(TMRn_CTRL_REG(n), tmr_ctrl_val);

    //Wait for reload.
    tmr_ctrl_val = read_reg32(TMRn_CTRL_REG(n));
    while(tmr_ctrl_val & (1 << TMRn_CTRL_RELOAD_OFFSET))
    {
        tmr_ctrl_val = read_reg32(TMRn_CTRL_REG(n));
    }
    
    tmr_ctrl_val |= (1 << TMRn_CTRL_EN_OFFSET);
    write_reg32(TMRn_CTRL_REG(n), tmr_ctrl_val);
    timer_interrupt_enable(n);
}

volatile void timer_stop(int n)
{
    __sync_synchronize();
    k_printf("\n\r");

    write_reg32(TMR_IRQ_EN_REG, (read_reg32(TMR_IRQ_EN_REG) & (~(1 << n))));

    k_printf("\n\r");
}