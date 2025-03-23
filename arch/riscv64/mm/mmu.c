#include <arch/mm/arch_mmu.h>
#include <arch/arch_mem_layout.h>
#include <common/operation.h>
#include <mm/mem_paging.h>
#include <lib/memops.h>
#include <mm/static_mem.h>

static pgtbl_t *_kernel_pgtbl;
error_t arch_mmu_mapping(pgtbl_t* pagetable, addr_t virt_addr_start,
                    addr_t phys_addr_start, pgsize_t size,
                    uint64_t permisson, mem_type type)
{
    if(size == 0)
    {
        panic("page size invalid");
    }

    volatile pte_t *pte;
    virt_addr_start = ALIGN_FLOOR(virt_addr_start);
    addr_t virt_addr_end = ALIGN_FLOOR(virt_addr_start + size);
    //It's not necessary to align phys_addr_start, it's only for making the whole process more explicit.
    phys_addr_start = ALIGN_FLOOR(phys_addr_start);

    while(virt_addr_start < virt_addr_end)
    {
        pte = arch_pte_retrieve(pagetable, virt_addr_start, type);
        if(pte == NULL)
        {
            printf("pte is NULL");
            return AA_ERROR_MMU_MAP;
        }
        if((*pte) & PTE_PERMISSION_V)
        {
            panic("pte:%p already mapped", pte);
        }
        (*pte) = (PHY_ADDR_TO_PTE(phys_addr_start) | permisson | PTE_PERMISSION_V);
        phys_addr_start += PAGE_SIZE;
        virt_addr_start += PAGE_SIZE;
    }

    return AA_ERROR_SUCCESS;
}

pte_t* arch_pte_retrieve(pgtbl_t* pagetable, addr_t virt_addr,
                         mem_type type)
{
    if(virt_addr >= VM_ADDR_MAX_VAL)
    {
        panic("virt_addr exceeded");
    }
    for (int idx = 2; idx > 0; --idx)
    {
        pte_t* pte = (pte_t*)(&pagetable[VM_ADDR_IDX(virt_addr, idx)]);
        if((*pte) & PTE_PERMISSION_V)
        {
            pagetable = (pgtbl_t*)(PTE_TO_PHY_ADDR(*pte));
        }
        else
        {
            if(type == MEM_TYPE_DYN)
                pagetable = (pgtbl_t*)mem_page_alloc();
            else if(type == MEM_TYPE_STATIC)
            {
                pagetable = (pgtbl_t*)static_mem_alloc();
            }
                
            memset(pagetable, 0, PAGE_SIZE);
            if(pagetable == NULL)
            {
                printf("pagetable NULL\n\r");
                return NULL;
            }
            (*pte) = PHY_ADDR_TO_PTE(pagetable) | PTE_PERMISSION_V;
        }
    }
    return &pagetable[VM_ADDR_IDX(virt_addr, 0)];
}

void arch_mmu_enable(pgtbl_t *pagetable)
{
    WRITE_CSR(satp, (SATP_SV39_MODE << RV64_SATP_MODE_OFFSET) | ADDR_TO_SATP((uintptr_t)pagetable));
    INVALIDATE_TLB();
}

void mmu_disable()
{
    WRITE_CSR(satp, (SATP_SV39_MODE << RV64_SATP_MODE_OFFSET));
    INVALIDATE_TLB();
}

void arch_kernel_mmu_enable()
{
    static_mem_init();
    _kernel_pgtbl = (pgtbl_t*)static_mem_alloc();
    uint32_t _k_tbl_entrys = 5;
    mmu_map_tbl _k_map_tbl[] = {
        {
            //Map for peripherals.
            .virt_addr_start = 0,
            .phys_addr_start = 0,
            .size = (addr_t)__kernel_start,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_SO_STRONG_ORDER
        },
        {
            //Map for kernel text segment
            .virt_addr_start = (addr_t)__kernel_start,
            .phys_addr_start = (addr_t)__kernel_start,
            .size = (addr_t)__kernel_text_start - (addr_t)__kernel_start,
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
            .size = (addr_t)MEM_TOP_ADDR - (addr_t)trampoline_end_addr + PAGE_SIZE,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A
        }
    };
    for(uint32_t i = 0; i < _k_tbl_entrys; ++ i)
    {
        error_t ret = arch_mmu_mapping(_kernel_pgtbl, _k_map_tbl[i].virt_addr_start,
                                       _k_map_tbl[i].phys_addr_start, _k_map_tbl[i].size,
                                       _k_map_tbl[i].permisson, MEM_TYPE_STATIC);
        if(ret != AA_ERROR_SUCCESS) // TODO: maybe need some recycles?
            panic("fail creating kernel pagetable");
    }
    arch_mmu_enable(_kernel_pgtbl);
}