#include "../include/k_vm.h"

volatile pagetable_t kernel_pagetable;

void k_pagetable_init()
{
    
    kernel_pagetable = (pagetable_t)k_alloc_single_page();

    //Map for peripherals.
    vm_map(kernel_pagetable, 0, 0, (addr_t) kernel_start, PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_SO_STRONG_ORDER);

    //Map for kernel text segment
    vm_map(kernel_pagetable, (addr_t)kernel_start, (addr_t)kernel_start, (addr_t)((addr_t)kernel_text_end - (addr_t)kernel_start), PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE);

    //Map for the rest of kernel and the rest of RAM
    vm_map(kernel_pagetable, (addr_t)kernel_text_end, (addr_t)kernel_text_end, (addr_t)RAM_TOP - (addr_t)kernel_text_end, PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE);
}

void k_vm_enable()
{   
    set_vm_pagetable(kernel_pagetable);
}
