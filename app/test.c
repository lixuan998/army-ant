#define UART_BASE_ADDR 0x02500000
#define UART_THR 0x0000
#define UART_RBR 0x0000
#define UART_USR 0x007C
#define UART_IER 0x0004
#define UART_FCR 0x0008
#define UART_LCR 0x000C
#define UART_LSR 0x0014
#define UART_IIR 0x0008
#define VM_PROC_SHARED_MEM_ADDR (0x40000000)

#define UART_LSR_DATA_READY 1 << 0
#define UART_LSR_THR_EMPTY 1 << 5

#define UART_LCR_DLS_8 3 << 0
#define UART_LCR_STOP 1 << 1
#define UART_LCR_PARITY 1 << 2

#define UART_FCR_ENABLE_FIFO 1 << 0

#define UART_IER_ERBFI 1 << 0 // Enable Received Data Available Interrupt.

#define uint32 unsigned int

inline void write32(volatile uint32 reg, uint32 val)
{
    volatile uint32 *reg_ptr = (uint32 *)((long)reg);
    *reg_ptr = val;
}

inline uint32 read32(volatile uint32 reg)
{
    uint32 val = *((uint32 *)((long)reg));
    return val;
}

void syscall_exit();

void putc(char c);
int main()
{
    char hello[] = "hello\n\r";
    for (int i = 0; i < 5; ++i)
    {
        while (((((volatile char *)(VM_PROC_SHARED_MEM_ADDR))[0]) != 'R'));
        (((volatile char *)(VM_PROC_SHARED_MEM_ADDR))[0]) = 'W';
        for (int i = 0; i < 7; ++i)
        {
            while (1)
            {
                uint32 lsr_val = read32(UART_BASE_ADDR + UART_LSR);
                if (lsr_val & (UART_LSR_THR_EMPTY))
                {
                    break;
                }
            }
            write32(UART_BASE_ADDR + UART_THR, hello[i]);
        }
        (((volatile char *)(VM_PROC_SHARED_MEM_ADDR))[0]) = 'R';
    }
    syscall_exit();
    return 0;
}

// int main()
// {
//     while (1)
//     {
//         while (1)
//         {
//             uint32 lsr_val = read32(UART_BASE_ADDR + UART_LSR);
//             if (lsr_val & (UART_LSR_THR_EMPTY))
//             {
//                 break;
//             }
//         }
//         write32(UART_BASE_ADDR + UART_THR, 'W');
//     }
// }

// int main()
// {
//     while(1)
//     {
//         putc('K');
//     }
// }

void putc(char c)
{
    while (1)
    {
        uint32 lsr_val = read32(UART_BASE_ADDR + UART_LSR);
        if (lsr_val & (UART_LSR_THR_EMPTY))
        {
            break;
        }
    }
    write32(UART_BASE_ADDR + UART_THR, c);
}