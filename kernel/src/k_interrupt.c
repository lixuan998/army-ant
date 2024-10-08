#include "arch/riscv/include/riscv_interrupt_defs.h"
#include "bsp/driver/plic/plic.h"
#include "bsp/driver/timer/timer.h"
#include "bsp/driver/sdio/sd.h"
#include "console/include/console.h"
#include "tools/uart_file_receiver/uart_file_receiver.h"

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
    interrupt_handler();
}

void user_interrupt_handler()
{
    uint32 interrupt_type = interrupt_handler();

    // Distinguish interrupt type, Interrupt or Exception.
    // If interrupt_type & (1 << 31) == 1, it's an interrupt, 0 otherwise.
    if (interrupt_type & (1 << 31))
    {
        int scause_val = ((interrupt_type >> 16) & 0xFF);
        if (scause_val == SCAUSE_SOFTWARE_INTERRUPT)
        {
        }
        else if (scause_val == SCAUSE_TIMER_INTERRUPT)
        {
        }
        else if (scause_val == SCAUSE_EXTERNAL_INTERRUPT)
        {
            int plic_val = ((interrupt_type & 0xFF));
            if (plic_val == TIMER0_SOURCE)
            {
                PROC *cur_proc = current_cpu_proc();
                if (cur_proc != NULL)
                {
                    if (cur_proc->proc_state == PROC_STATE_RUNNING)
                    {
                        proc_yield();
                    }
                }
            }
        }
    }
    else
    {
        int scause_val = ((interrupt_type >> 16) & 0xFF);
        if (scause_val == SCAUSE_ECALL_U)
        {
            PROC *cur_proc = current_cpu_proc();
            cur_proc->trapframe->epc += 4;
            interrupt_enable();
            syscall();
        }
    }
}

uint32 interrupt_handler()
{
    uint32 interrupt_type = 0;
    isa_reg_t scause = r_scause();

    isa_reg_t scause_code = scause & SCAUSE_EXCEPTION_CODE_MASK;
    if (scause & SCAUSE_INTERRUPT)
    {
        interrupt_type = 1 << 31;
        switch (scause_code)
        {
        case SCAUSE_SOFTWARE_INTERRUPT:
        {
            interrupt_type |= (SCAUSE_SOFTWARE_INTERRUPT) << 16;
            break;
        }
        case SCAUSE_TIMER_INTERRUPT:
        {
            interrupt_type |= (SCAUSE_TIMER_INTERRUPT) << 16;
            break;
        }
        case SCAUSE_EXTERNAL_INTERRUPT:
        {
            interrupt_type |= (SCAUSE_EXTERNAL_INTERRUPT) << 16;
            enum PLIC_EXTERNAL_INTERRUPT_SOURCE source;
            source = plic_interrupt_source();
            switch (source)
            {
            case UART0_SOURCE:
            {
                interrupt_type |= UART0_SOURCE;
                char c;
                int ret = getc(&c);
                while (ret >= 0)
                {
                    file_byte_receive(c);
                    console_get_char(c);
                    ret = getc(&c);
                }
                break;
            }
            case TIMER0_SOURCE:
            {
                interrupt_type |= TIMER0_SOURCE;
                timestamp_update();
                timer_pending_clear(0);
                break;
            }
            case SMHC0_SOURCE:
            {
                interrupt_type |= SMHC0_SOURCE;
                volatile soc_reg_t reg_val = read32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET);
                volatile soc_reg_t status = read32(SMHCn_BASE_ADDR(0) + SMHC_STATUS_OFFSET);
                write32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET, 0xFFFFFFFF);
                while (reg_val)
                {
                    reg_val = read32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET);
                }
                reg_val = read32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET);
                write32(SMHCn_BASE_ADDR(0) + SMHC_RINTSTS_OFFSET, reg_val);
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
    else
    {
        switch (scause_code)
        {
        case SCAUSE_ECALL_U:
        {
            interrupt_type |= (SCAUSE_ECALL_U) << 16;
            break;
        }
        case SCAUSE_ECALL_S:
        {
            interrupt_type |= (SCAUSE_ECALL_S) << 16;
            printf("ECALL_S\n\r");
        }
        default:
            break;
        }
    }
    return interrupt_type;
}