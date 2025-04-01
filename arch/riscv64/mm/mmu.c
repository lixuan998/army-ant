#include <arch/mm/arch_mmu.h>
#include <arch/mm/arch_mem_layout.h>
#include <common/operation.h>
#include <common/log.h>
#include <mm/mem_paging.h>
#include <lib/memops.h>
#include <mm/static_mem.h>

static pgtbl_t *_kernel_pgtbl;
extern mmu_map_tbl _k_map_tbl[MMU_MAP_TBL_MAX_ENTRYS];

error_t arch_mmu_mapping(pgtbl_t* pagetable, addr_t virt_addr_start,
                         addr_t phys_addr_start, pgsize_t size,
                         uint64_t permisson, mem_type type)
{
    if (size == 0) {
        PANIC("page size invalid");
    }

    volatile pte_t *pte;
    virt_addr_start = ALIGN_FLOOR(virt_addr_start, PAGE_SIZE);
    addr_t virt_addr_end = ALIGN_FLOOR(virt_addr_start + size, PAGE_SIZE);

    //It's not necessary to align phys_addr_start, it's only for making the whole process more explicit.
    phys_addr_start = ALIGN_FLOOR(phys_addr_start, PAGE_SIZE);
    KLOG_INFO("MMU", "mapping phy addr range %p ~ %p to virt addr range %p ~ %p",
              phys_addr_start, phys_addr_start + size,
              virt_addr_start, virt_addr_end);
    while (virt_addr_start < virt_addr_end) {
        pte = arch_pte_retrieve(pagetable, virt_addr_start, type);
        if (pte == NULL) {
            KLOG_ERR("MMU", "pte NULL");
            return AA_ERROR_MMU_MAP;
        }
        if ((*pte) & PTE_PERMISSION_V) {
            PANIC("pte:%p already mapped", pte);
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
    if (virt_addr >= VM_ADDR_MAX_VAL) {
        PANIC("virt_addr exceeded");
    }
    for (int idx = 2; idx > 0; --idx) {
        pte_t* pte = (pte_t*)(&pagetable[VM_ADDR_IDX(virt_addr, idx)]);
        if ((*pte) & PTE_PERMISSION_V) {
            pagetable = (pgtbl_t*)(PTE_TO_PHY_ADDR(*pte));
        } else {
            if (type == MEM_TYPE_DYN) {
                pagetable = (pgtbl_t*)mem_page_alloc();
            } else if (type == MEM_TYPE_STATIC) {
                pagetable = (pgtbl_t*)static_mem_alloc();
            }
 
            if (pagetable == NULL) {
                KLOG_ERR("MMU", "pagetable NULL");
                return NULL;
            }
            memset(pagetable, 0, PAGE_SIZE);
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

void arch_mmu_disable(void)
{
    WRITE_CSR(satp, (SATP_SV39_MODE << RV64_SATP_MODE_OFFSET));
    INVALIDATE_TLB();
}

void arch_kernel_mmu_init(void)
{
    static_mem_init();
    _kernel_pgtbl = (pgtbl_t*)static_mem_alloc();
    uint32_t _k_tbl_entrys = 5;
    
    // Create kernel page table.
    for (uint32_t i = 0; i < _k_tbl_entrys; ++i) {
        error_t ret = arch_mmu_mapping(_kernel_pgtbl, _k_map_tbl[i].virt_addr_start,
                                       _k_map_tbl[i].phys_addr_start, _k_map_tbl[i].size,
                                       _k_map_tbl[i].permisson, MEM_TYPE_STATIC);
        if (ret != AA_ERROR_SUCCESS) {
            PANIC("fail creating kernel pagetable");
        } // TODO: maybe need some recycles?
    }
    arch_mmu_enable(_kernel_pgtbl);
}