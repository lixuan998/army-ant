#include "kernel/include/k_vm.h"

volatile pagetable_t kernel_pagetable;

int k_vm_map(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, uint64 permisson)
{
    return vm_map(pagetable, virt_addr_start, phys_addr_start, size, permisson);
}

void k_pagetable_init()
{
    kernel_pagetable = (pagetable_t)k_alloc_single_page();

    //Map for peripherals.
    k_vm_map(kernel_pagetable, 0, 0, (addr_t) kernel_start, PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_SO_STRONG_ORDER);

    //Map for kernel text segment
    k_vm_map(kernel_pagetable, (addr_t)kernel_start, (addr_t)kernel_start, (addr_t)((addr_t)kernel_text_end - (addr_t)kernel_start), PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE);

    //Map for the rest of kernel and the rest of RAM
    k_vm_map(kernel_pagetable, (addr_t)kernel_text_end, (addr_t)kernel_text_end, (addr_t)RAM_TOP - (addr_t)kernel_text_end, PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE);
}

void k_vm_enable()
{   
    vm_enable(kernel_pagetable);
}
