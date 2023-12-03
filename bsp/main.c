#include "../kernel/include/k_logo.h"
#include "../kernel/include/k_stdio.h"
#include "../kernel/include/k_paging.h"
#include "../kernel/include/k_vm.h"
#include "../arch/riscv/include/asm_operation.h"

extern void boot_cfg();

int main()
{    
    k_printf("kernel start: %x\n\r", (addr_t)kernel_start);
    k_printf("kernel end: %x\n\r", (addr_t)kernel_end);
    k_printf("kernel text end: %x\n\r", (addr_t)kernel_text_end);
    k_printf("ram size: %x\n\r", RAM_SIZE);
    k_printf("ram top: %x\n\r", (addr_t)RAM_TOP);
    
    k_printf("kernel memory paging initializing...\n\r");
    k_mem_paging_init();
    k_printf("kernel memory paging initializing complete\n\r");
    k_printf("enabling virtual memory...\n\r");
    k_pagetable_init();
    k_vm_enable();
    k_printf("virtual memory enabled!\n\r");
    
    k_printf(ARMY_ANT_LOGO);
    
    return 0;
}

