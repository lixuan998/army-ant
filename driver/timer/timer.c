#include "timer.h"

void timer_interrupt_enable(int n)
{
    WRITE_REG32(TMR_IRQ_EN_REG, (READ_REG32(TMR_IRQ_EN_REG) | (1 << n)));
}

void timer_pending_clear(int n)
{
    WRITE_REG32(TMR_IRQ_STA_REG, 1 << n);
}

void timer_set_prescale(int n, int prescale)
{
    WRITE_REG32(TMRn_CTRL_REG(n), READ_REG32(TMRn_CTRL_REG(n)) | prescale << TMRn_CLK_PRES_OFFSET);
}

void timer_start(int n, int tick)
{
    WRITE_REG32(TMRn_INTV_VALUE_REG(n), READ_REG32(TMRn_INTV_VALUE_REG(n)) | (TIMER_FREQUENCY / tick));
    volatile uint32 tmr_ctrl_val = READ_REG32(TMRn_CTRL_REG(n));
    timer_set_prescale(n, TMRn_CTRL_PRES_DIVEDE_1);
    tmr_ctrl_val = READ_REG32(TMRn_CTRL_REG(n));
    tmr_ctrl_val |= 1 << TMRn_CTRL_RELOAD_OFFSET;
    
    WRITE_REG32(TMRn_CTRL_REG(n), tmr_ctrl_val);

    //Wait for reload.
    tmr_ctrl_val = READ_REG32(TMRn_CTRL_REG(n));
    while(tmr_ctrl_val & (1 << TMRn_CTRL_RELOAD_OFFSET))
    {
        tmr_ctrl_val = READ_REG32(TMRn_CTRL_REG(n));
    }
    tmr_ctrl_val |= (1 << TMRn_CTRL_EN_OFFSET);
    WRITE_REG32(TMRn_CTRL_REG(n), tmr_ctrl_val);
    timer_interrupt_enable(n);
}

void timer_stop(int n)
{
    WRITE_REG32(TMR_IRQ_EN_REG, (READ_REG32(TMR_IRQ_EN_REG) & (~(1 << n))));
}