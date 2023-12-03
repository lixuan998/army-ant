#include "../include/vm.h"

int vm_map(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, uint64 permisson)
{
    if(size == 0)
    {
        k_panic("in vm_map, page size invalid.");
    }

    pte_t *pte;
    virt_addr_start = ALIGN_FLOOR(virt_addr_start);
    addr_t virt_addr_end = ALIGN_FLOOR(virt_addr_start + size);

    //It's not necessary to align phys_addr_start, it's only for making the whole process more explicit.k_printf("phy addr: %x\n\r", phys_addr_start);
    phys_addr_start = ALIGN_FLOOR(phys_addr_start);

    while(virt_addr_start < virt_addr_end)
    {
        pte = pte_retrieve(pagetable, virt_addr_start);
        if(pte == NULL)
        {
            k_printf("in vm_map, pte is NULL.");
            return VM_MAP_FAILED;
        }
        if((*pte) & PTE_PERMISSION_V)
        {
            k_panic("in vm_map, pte already mapped.");
        }
        (*pte) = PHY_ADDR_TO_PTE(phys_addr_start) | permisson | PTE_PERMISSION_V;
        phys_addr_start += PAGE_SIZE;
        virt_addr_start += PAGE_SIZE;
    }
    return VM_MAP_SUCCESS;
}

pte_t* pte_retrieve(pagetable_t pagetable, addr_t virt_addr)
{
    #ifdef SV39 //Use Sv39 scheme
    if(virt_addr >= ADDR_MAX_VAL)
    {
        k_panic("in pte_retrieve, virt_addr exceeded.");
    }

    for(int idx = 2; idx > 0; -- idx)
    {
        pte_t* pte = &pagetable[ADDR_IDX(virt_addr, idx)];
        if((*pte) & PTE_PERMISSION_V)
        {
            pagetable = (pagetable_t)(PTE_TO_PHY_ADDR(*pte));
        }
        else
        {
            pagetable = (pagetable_t)k_alloc_single_page();
            memset(pagetable, 0, PAGE_SIZE);
            if(pagetable == NULL)
            {
                k_printf("pagetable NULL\n\r");
                return NULL;
            }
            (*pte) = PHY_ADDR_TO_PTE(pagetable) | PTE_PERMISSION_V;
        }
    }
    #endif /* SV39 */
    return &pagetable[ADDR_IDX(virt_addr, 0)];
}

void vm_enable(pagetable_t pagetable)
{
    sfence_vma();
    w_satp((SATP_SV39_MODE << RV64_SATP_MODE_OFFSET) | PHY_TO_SATP((addr_t)pagetable));
    sfence_vma();
}
