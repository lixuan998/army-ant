#include <arch/mm/arch_mmu.h>
#include <arch/mm/arch_mem_layout.h>
#include <common/operation.h>
#include <mm/mmu.h>
#include <lib/memops.h>

mmu_map_tbl _k_map_tbl[MMU_MAP_TBL_MAX_ENTRYS]; //The real map table.

void mmu_map_init(void)
{
    WRITE_CSR(satp, (uintptr_t)SATP_BARE_MODE << RV64_SATP_MODE_OFFSET);
    INVALIDATE_TLB();
    mmu_map_tbl __k_map_tbl[] = {
        {
            //Map for peripherals.
            .virt_addr_start = (addr_t)__peripheral_start,
            .phys_addr_start = (addr_t)__peripheral_start,
            .size = (addr_t)__peripheral_end - (addr_t)__peripheral_start,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W
        },
        {
            //Map for kernel text segment
            .virt_addr_start = (addr_t)__text_start,
            .phys_addr_start = (addr_t)__text_start,
            .size = (addr_t)__text_end - (addr_t)__text_start,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_X
        },
        {
            //Map for the trampoline (virtual address to physical address)
            .virt_addr_start = (addr_t)VM_TRAMPOLINE_ADDR,
            .phys_addr_start = (addr_t)__trampoline_start,
            .size = PAGE_SIZE,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_X
        },
        {
            //Map for the trampoline (physical address to physical address)
            .virt_addr_start = (addr_t)__trampoline_start,
            .phys_addr_start = (addr_t)__trampoline_start,
            .size = PAGE_SIZE,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_X
        },
        {
            //Map for the rest of kernel and the rest of RAM
            .virt_addr_start = (addr_t)__trampoline_end,
            .phys_addr_start = (addr_t)__trampoline_end,
            .size = (addr_t)__top_addr - (addr_t)__trampoline_end,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A
        }
    };
    memcpy((void *)_k_map_tbl, (void *)__k_map_tbl, sizeof(mmu_map_tbl) * MMU_MAP_TBL_MAX_ENTRYS);
}

