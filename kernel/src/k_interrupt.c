#include "arch/defs.h"
#include "bsp/driver/plic/plic.h"
#include "bsp/driver/timer/timer.h"
#include "console/include/console.h"

void interrupt_init(void *interrupt_vector)
{
    w_stvec((isa_reg_t)interrupt_vector);
    plic_interrupt_enable();
}

void interrupt_enable()
{
    turn_on_s_interrupt();
}

void interrupt_disable()
{
    turn_off_s_interrupt();
}

void interrupt_handler()
{
    isa_reg_t scause = r_scause();
    isa_reg_t scause_code = r_scause() & SCAUSE_EXCEPTION_CODE_MASK;
    if(scause & SCAUSE_INTERRUPT)
    {
        switch(scause_code)
        {
            case SCAUSE_SOFTWARE_INTERRUPT:
            {
                software_interrupt_handler();
                break;
            }
            case SCAUSE_TIMER_INTERRUPT:
            {
                timer_interrupt_handler();
                break;
            } 
            case SCAUSE_EXTERNAL_INTERRUPT:
            {
                external_interrupt_handler();
                break;
            }
            default:
                break;
        }
        
    }
}

void external_interrupt_handler()
{
    enum EXTERNAL_INTERRUPT_SOURCE source;
    source = read_reg32(PLIC_SCLAIM_REG);
    switch (source)
    {
        case UART0_SOURCE:
        {
            char c;
            int ret = getc(&c);
            while(ret >= 0)
            {
                console_get_char(c);
                ret = getc(&c);
            }
            break;
        }
        case TIMER0_SOURCE:
        {
            timestamp_update();
            timer_pending_clear(0);
            break;
        }
        
        default:
            break;
        }
    external_interrupt_handled(source);
}

void external_interrupt_handled(int source)
{
    write_reg32(PLIC_SCLAIM_REG, source);
}

void timer_interrupt_handler()
{

}

void software_interrupt_handler()
{

}