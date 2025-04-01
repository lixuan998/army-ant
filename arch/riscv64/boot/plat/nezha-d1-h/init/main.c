// #include "arch/riscv/include/riscv_spinlock_defs.h"
// #include "arch/riscv/include/riscv_mem_layout.h"
// #include "arch/riscv/include/riscv_interrupt_defs.h"
// #include "arch/riscv/include/riscv_mem_defs.h"
// #include "arch/riscv/include/riscv_type_defs.h"
// #include "arch/riscv/include/riscv_proc_defs.h"
// #include "lib/include/stdio.h"
// #include "lib/include/time.h"
// #include "lib/include/time.h"
// #include "driver/uart/uart.h"
// #include "driver/timer/timer.h"
// #include "driver/smhc/sd.h"
// #include "driver/gpio/gpio.h"
// #include "console/include/console.h"
// #include "fs/include/diskio.h"

// void kernel_mem_init();
// void kernel_vm_init();
// void kernel_interrupt_init();

// void general_spinlock_init();

// void kernel_init();
// void peripheral_init();
#include <lib/printk.h>
#include <common/logo.h>

#include <mm/mem_paging.h>
#include <mm/mmu.h>

#include <arch/mm/arch_mem_layout.h>

int main()
{        
    kernel_mmu_init();
    // general_spinlock_init();
    // kernel_init();
    
    // peripheral_init();
    // smhcn_init(0);
    // console_init();

    // register_virtual_disk();

    // init_proc();
    // scheduler();
    mem_paging_init();
    print_logo();
    while(1);
    return 0;
}

// void kernel_mem_init()
// {
//     printf("memory page size:                     %d bytes\n\r", PAGE_SIZE);
//     printf("memory start address:                 %x\n\r", kernel_end_addr);
//     printf("memory end address:                   %x\n\r", MEM_TOP_ADDR);
//     printf("trampoline address:                   %x\n\r", __trampoline_start);
//     printf("kernel text end address:                   %x\n\r", kernel_text_end_addr);
//     printf("memory size:                          %d bytes\n\r", MEM_TOP_ADDR);
//     mem_paging_init();
//     printf("memory management system              [OK]\n\r");
// }

// void kernel_interrupt_init()
// {
//     interrupt_init(kernel_interrupt_vector);
//     interrupt_enable();
//     printf("interrupt system initialize           [OK]\n\r");
// }

// void general_spinlock_init()
// {
//     for(int i = 0; i < GENERAL_SPINLOCK_NUM; ++i)
//     {
//         spinlock_init(general_spinlock + i, "general_spinlock");
//     }
// }

// void kernel_init()
// {

//     kernel_interrupt_init();
//     printf("\n\r");
//     printf("kernel initialize                     [OK]\n\r");
//     printf("ready to start...\n\r");
// }

// void peripheral_init()
// {
//     uart_init();
//     timer_start(0, 1000);
// }
