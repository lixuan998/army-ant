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

void kernel_interrupt_handler()
{
    isa_reg_t scause = r_scause();

    isa_reg_t scause_code = scause & SCAUSE_EXCEPTION_CODE_MASK;
    if (scause & SCAUSE_INTERRUPT)
    {
        switch (scause_code)
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
            enum PLIC_EXTERNAL_INTERRUPT_SOURCE source;
            source = plic_interrupt_source();
            // printf("source: %d\n\r", source);
            switch (source)
            {
            case UART0_SOURCE:
            {
                char c;
                int ret = getc(&c);
                while (ret >= 0)
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
                while (reg_val)
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
            plic_interrupt_handled(source);
            break;
        }
        default:
            break;
        }
    }
    else if (scause & SCAUSE_EXCEPTION)
    {
        switch (scause_code)
        {
        case SCAUSE_ECALL_U:
        {
            printf("ECALL_U\n\r");
            break;
        }
        case SCAUSE_ECALL_S:
        {
            printf("ECALL_S\n\r");
        }
        default:
            break;
        }
    }
}

void user_interrupt_handler()
{
    isa_reg_t scause = r_scause();

    isa_reg_t scause_code = scause & SCAUSE_EXCEPTION_CODE_MASK;
    if (scause & SCAUSE_INTERRUPT)
    {
        switch (scause_code)
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
            enum PLIC_EXTERNAL_INTERRUPT_SOURCE source;
            source = plic_interrupt_source();
            // printf("source: %d\n\r", source);
            switch (source)
            {
            case UART0_SOURCE:
            {
                char c;
                int ret = getc(&c);
                while (ret >= 0)
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
                PROC *cur_proc = current_cpu_proc();
                if (cur_proc != NULL)
                {
                    if (cur_proc->proc_state == PROC_STATE_RUNNING)
                    {
                        plic_interrupt_handled(source);
                        proc_yield();
                    }
                }
                break;
            }
            case SMHC0_SOURCE:
            {
                volatile soc_reg_t reg_val = read32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET);
                // volatile soc_reg_t status = read32(SMHCn_BASE_ADDR(0) + SMHC_STATUS_OFFSET);
                // printf("status: %x, %b\n\r", status, status);
                write32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET, 0xFFFFFFFF);
                while (reg_val)
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
            plic_interrupt_handled(source);
            break;
        }
        default:
            break;
        }
    }
    else if (scause & SCAUSE_EXCEPTION)
    {
        switch (scause_code)
        {
        case SCAUSE_ECALL_U:
        {
            printf("ECALL_U\n\r");
            break;
        }
        case SCAUSE_ECALL_S:
        {
            printf("ECALL_S\n\r");
        }
        default:
            break;
        }
    }
}

void external_interrupt_handled(int source)
{
    write32(PLIC_SCLAIM_REG, read32(PLIC_SCLAIM_REG));
}

void timer_interrupt_handler()
{
}

void software_interrupt_handler()
{
}