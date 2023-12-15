#include "kernel/include/k_logo.h"
#include "kernel/include/k_stdio.h"
#include "kernel/include/k_paging.h"
#include "kernel/include/k_vm.h"
#include "kernel/include/k_time.h"
#include "arch/riscv/include/riscv_asm_operation.h"
#include "kernel/include/k_interrupt.h"
#include "kernel/include/k_time.h"
#include "driver/uart/uart.h"
#include "driver/timer/timer.h"

int test_val = 0;

void kernel_init();
void peripheral_init();

int main()
{    
    
    
    // k_printf("kernel start: %x\n\r", (addr_t)kernel_start);
    // k_printf("kernel end: %x\n\r", (addr_t)kernel_end);
    // k_printf("kernel text end: %x\n\r", (addr_t)kernel_text_end);
    // k_printf("ram size: %x\n\r", RAM_SIZE);
    // k_printf("ram top: %x\n\r", (addr_t)RAM_TOP);
    
    // k_printf("kernel memory paging initializing...\n\r");
    
    // k_printf("kernel memory paging initialized!\n\r");
    // k_printf("initializing virtual memory...\n\r");
    
    // k_printf("virtual memory initialized!\n\r");
    // k_printf("initializing interrupt...\n\r");
    
    
    // k_printf("interrupt initialized!\n\r");
    kernel_init();
    peripheral_init();
    console_init();
    
    
    
    while(1)
    {
        // uint32 duration = 0;
        // k_start_timing();
        // k_msleep(876);
        // k_stop_timing(&duration);
        // k_printf("duration: %d\n\r", duration);
        
    }
    return 0;
}

void kernel_init()
{
    k_time_stamp_init();
    k_general_spinlock_init();
    k_mem_paging_init();
    k_pagetable_init();
    k_vm_enable();
    k_interrupt_init();
}

void peripheral_init()
{
    uart_init();
    timer_start(0, 1000);
}
