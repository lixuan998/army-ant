#include "arch/riscv/include/riscv_spinlock_defs.h"
#include "arch/riscv/include/riscv_mem_layout.h"
#include "arch/riscv/include/riscv_interrupt_defs.h"
#include "arch/riscv/include/riscv_mem_defs.h"
#include "arch/riscv/include/riscv_type_defs.h"
#include "arch/riscv/include/riscv_proc_defs.h"
#include "lib/include/stdio.h"
#include "lib/include/time.h"
#include "lib/include/time.h"
#include "driver/uart/uart.h"
#include "driver/timer/timer.h"
#include "driver/smhc/sd.h"
#include "driver/gpio/gpio.h"
#include "console/include/console.h"

void kernel_mem_init();
void kernel_vm_init();
void kernel_interrupt_init();

void general_spinlock_init();

void kernel_init();
void peripheral_init();

int main()
{        
    general_spinlock_init();
    kernel_init();
    
    peripheral_init();
    smhcn_init(0);
    console_init();

    init_proc();
    scheduler();
    
    while(1);
    return 0;
}

void kernel_mem_init()
{
    printf("memory page size:                     %d bytes\n\r", PAGE_SIZE);
    printf("memory start address:                 %x\n\r", kernel_end_addr);
    printf("memory end address:                   %x\n\r", MEM_TOP_ADDR);
    printf("trampoline address:                   %x\n\r", trampoline_start_addr);
    printf("kernel text end address:                   %x\n\r", kernel_text_end_addr);
    printf("memory size:                          %d bytes\n\r", MEM_TOP_ADDR);
    mem_paging_init();
    printf("memory management system              [OK]\n\r");
}

void kernel_vm_init()
{
    printf("virtual memory rage:                  %x ~ %x\n\r", 0, kernel_text_end_addr);
    printf("physical memory rage:                 %x ~ %x\n\r", 0, kernel_text_end_addr);
    extern pagetable_t kernel_pagetable;
    VM_MAP_INFO vm_map_info[] = {
        {
            //Map for peripherals.
            .virt_addr_start = 0,
            .phys_addr_start = 0,
            .size = (addr_t)kernel_start_addr,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_SO_STRONG_ORDER
        },
        {
            //Map for kernel text segment
            .virt_addr_start = (addr_t)kernel_start_addr,
            .phys_addr_start = (addr_t)kernel_start_addr,
            .size = (addr_t)kernel_text_end_addr - (addr_t)kernel_start_addr,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE
        },
        {
            //Map for the trampoline (virtual address to physical address)
            .virt_addr_start = (addr_t)VM_TRAMPOLINE_ADDR,
            .phys_addr_start = (addr_t)trampoline_start_addr,
            .size = PAGE_SIZE,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A
        },
        {
            //Map for the trampoline (physical address to physical address)
            .virt_addr_start = (addr_t)trampoline_start_addr,
            .phys_addr_start = (addr_t)trampoline_start_addr,
            .size = PAGE_SIZE,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A
        },
        {
            //Map for the rest of kernel and the rest of RAM
            .virt_addr_start = (addr_t)trampoline_end_addr,
            .phys_addr_start = (addr_t)trampoline_end_addr,
            .size = (addr_t)MEM_TOP_ADDR - (addr_t)trampoline_end_addr,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A
        }
    };
    printf("kernel pagetable create               [OK]\n\r");
    kernel_pagetable = pagetable_create(vm_map_info, 5);
    set_vm_pagetable(kernel_pagetable);

    printf("set kernel pagetable                  [OK]\n\r");
    printf("virtual memory system initialize      [OK]\n\r");
}

void kernel_interrupt_init()
{
    interrupt_init(kernel_interrupt_vector);
    interrupt_enable();
    printf("interrupt system initialize           [OK]\n\r");
}

void general_spinlock_init()
{
    for(int i = 0; i < GENERAL_SPINLOCK_NUM; ++ i)
    {
        spinlock_init(general_spinlock + i, "general_spinlock");
    }
}

void kernel_init()
{
    printf("\n\r");
    kernel_mem_init();
    printf("\n\r");
    kernel_vm_init();
    printf("set kernel pagetable: %x\n\r", r_satp());
    printf("\n\r");
    kernel_interrupt_init();
    printf("\n\r");
    printf("kernel initialize                     [OK]\n\r");
    printf("ready to start...\n\r");
}

void peripheral_init()
{
    uart_init();
    timer_start(0, 1000);
}
