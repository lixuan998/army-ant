#include "arch/defs.h"
#include "bsp/driver/plic/plic.h"
#include "bsp/driver/timer/timer.h"
#include "bsp/driver/smhc/sd.h"
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
    else
    {
        printf("scause code : %x\n\r", scause_code);
        switch (scause_code)
        {
            case SCAUSE_ECALL_U :
            {
                printf("ECALL_U\n\r");
                break;
            }
            case SCAUSE_ECALL_S :
            {
                printf("ECALL_S\n\r");
            }
        default:
            break;
        }
    }
}

void external_interrupt_handler()
{
    enum EXTERNAL_INTERRUPT_SOURCE source;
    source = read32(PLIC_SCLAIM_REG);
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
        case SMHC0_SOURCE:
        {
            volatile soc_reg_t reg_val = read32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET);
            // volatile soc_reg_t status = read32(SMHCn_BASE_ADDR(0) + SMHC_STATUS_OFFSET);
            // printf("status: %x, %b\n\r", status, status);
            write32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET, 0xFFFFFFFF);
            while(reg_val)
            {
                reg_val = read32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET);
            }
            reg_val = read32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET);
            // write32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET, reg_val);
            // if(reg_val & (1 << 30)) printf("Card Insert, reg_val: %b, or res: %d\n\r", reg_val, (reg_val & (1 << 30)));
            // else if(reg_val & (1 << 31)) printf("Card Removed\n\r");
            break;
        }
        default:
            break;
        }
    external_interrupt_handled(source);
}

void external_interrupt_handled(int source)
{
    write32(PLIC_SCLAIM_REG, source);
}

void timer_interrupt_handler()
{

}

void software_interrupt_handler()
{

}