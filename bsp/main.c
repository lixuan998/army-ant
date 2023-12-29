#include "kernel/include/k_logo.h"
#include "lib/include/stdio.h"
#include "kernel/include/k_paging.h"
#include "kernel/include/k_vm.h"
#include "lib/include/time.h"
#include "arch/asm_operation.h"
#include "kernel/include/k_interrupt.h"
#include "lib/include/time.h"
#include "driver/uart/uart.h"
#include "driver/timer/timer.h"
#include "console/include/console.h"

int test_val = 0;

void kernel_init();
void peripheral_init();

int main()
{    
    
    
    // printf("kernel start: %x\n\r", (addr_t)kernel_start);
    // printf("kernel end: %x\n\r", (addr_t)kernel_end);
    // printf("kernel text end: %x\n\r", (addr_t)kernel_text_end);
    // printf("ram size: %x\n\r", RAM_SIZE);
    // printf("ram top: %x\n\r", (addr_t)RAM_TOP);
    
    // printf("kernel memory paging initializing...\n\r");
    
    // printf("kernel memory paging initialized!\n\r");
    // printf("initializing virtual memory...\n\r");
    
    // printf("virtual memory initialized!\n\r");
    // printf("initializing interrupt...\n\r");
    
    
    // printf("interrupt initialized!\n\r");
    printf("AAA\n\r");
    kernel_init();
    printf("BBB\n\r");
    peripheral_init();
    printf("CCC\n\r");
    console_init();
    
    
    
    while(1)
    {
        // uint32 duration = 0;
        // start_timing();
        // sleep(1);
        // stop_timing(&duration);
        // printf("duration: %d\n\r", duration);
        
    }
    return 0;
}

void kernel_init()
{
    k_mem_paging_init();
    k_pagetable_init();
    k_vm_enable();
    k_interrupt_init();
}

void peripheral_init()
{
    uart_init();
    // timer_start(0, 1000);
}
