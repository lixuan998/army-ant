#include "timer.h"

void timer_interrupt_enable(int n)
{
    write32(TMR_IRQ_EN_REG, (read32(TMR_IRQ_EN_REG) | (1 << n)));
}

void timer_pending_clear(int n)
{
    write32(TMR_IRQ_STA_REG, 1 << n);
}

void timer_set_prescale(int n, int prescale)
{
    write32(TMRn_CTRL_REG(n), read32(TMRn_CTRL_REG(n)) | prescale << TMRn_CLK_PRES_OFFSET);
}

void timer_start(int n, int tick)
{
    write32(TMRn_INTV_VALUE_REG(n), read32(TMRn_INTV_VALUE_REG(n)) | (TIMER_FREQUENCY / tick));
    volatile uint32 tmr_ctrl_val = read32(TMRn_CTRL_REG(n));
    timer_set_prescale(n, TMRn_CTRL_PRES_DIVEDE_1);
    tmr_ctrl_val = read32(TMRn_CTRL_REG(n));
    tmr_ctrl_val |= 1 << TMRn_CTRL_RELOAD_OFFSET;
    
    write32(TMRn_CTRL_REG(n), tmr_ctrl_val);

    //Wait for reload.
    tmr_ctrl_val = read32(TMRn_CTRL_REG(n));
    while(tmr_ctrl_val & (1 << TMRn_CTRL_RELOAD_OFFSET))
    {
        tmr_ctrl_val = read32(TMRn_CTRL_REG(n));
    }
    tmr_ctrl_val |= (1 << TMRn_CTRL_EN_OFFSET);
    write32(TMRn_CTRL_REG(n), tmr_ctrl_val);
    timer_interrupt_enable(n);
}

void timer_stop(int n)
{
    write32(TMR_IRQ_EN_REG, (read32(TMR_IRQ_EN_REG) & (~(1 << n))));
}