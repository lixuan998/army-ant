#include "../include/k_vm.h"

pagetable_t kernel_pagetable;

void k_pagetable_init()
{
    
    kernel_pagetable = (pagetable_t)k_alloc_single_page();

    VM_MAP_INFO vm_map_info[] = {
        {
            //Map for peripherals.
            .virt_addr_start = 0,
            .phys_addr_start = 0,
            .size = (addr_t)kernel_start,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_SO_STRONG_ORDER
        },
        {
            //Map for kernel text segment
            .virt_addr_start = (addr_t)kernel_start,
            .phys_addr_start = (addr_t)kernel_start,
            .size = (addr_t)kernel_text_end - (addr_t)kernel_start,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE
        },
        {
            //Map for the rest of kernel and the rest of RAM
            .virt_addr_start = (addr_t)kernel_text_end,
            .phys_addr_start = (addr_t)kernel_text_end,
            .size = (addr_t)RAM_TOP - (addr_t)kernel_text_end,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE
        }
    };

    pagetabe_init(kernel_pagetable, vm_map_info, 3);
}

void k_vm_enable()
{   
    set_vm_pagetable(kernel_pagetable);
}
