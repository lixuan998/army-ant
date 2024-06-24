#include "arch/defs.h"
#include "lib/include/stdio.h"
#include "lib/include/stdlib.h"

#ifdef SV39

pagetable_t kernel_pagetable = NULL;

pagetable_t pagetable_create(VM_MAP_INFO map_info[], int num_of_mapping)
{
    pagetable_t pagetable = (pagetable_t)alloc_single_page();

    pagetable_entry_add(pagetable, map_info, num_of_mapping);
    return pagetable;
}

void pagetable_entry_add(pagetable_t pagetable, VM_MAP_INFO map_info[], int num_of_mapping)
{
    if(pagetable == NULL) panic("in pagetable_entry_add, pagetable is NULL.");
    for(int i = 0; i < num_of_mapping; ++ i)
    {
        vm_mapping(pagetable, map_info[i].virt_addr_start, map_info[i].phys_addr_start, map_info[i].size, map_info[i].permisson);
    }
}

int vm_mapping(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, isa_reg_t permisson)
{
    if(size == 0)
    {
        panic("in vm_mapping, page size invalid.");
    }

    volatile pte_t *pte;
    virt_addr_start = ALIGN_FLOOR(virt_addr_start);
    addr_t virt_addr_end = ALIGN_FLOOR(virt_addr_start + size);

    //It's not necessary to align phys_addr_start, it's only for making the whole process more explicit.
    phys_addr_start = ALIGN_FLOOR(phys_addr_start);

    while(virt_addr_start < virt_addr_end)
    {
        pte = pte_retrieve(pagetable, virt_addr_start);
        if(pte == NULL)
        {
            printf("in vm_mapping, pte is NULL.");
            return VM_MAP_FAILED;
        }
        if((*pte) & PTE_PERMISSION_V)
        {
            panic("in vm_mapping, pte already mapped.");
        }
        (*pte) = (PHY_ADDR_TO_PTE(phys_addr_start) | permisson | PTE_PERMISSION_V);
        phys_addr_start += PAGE_SIZE;
        virt_addr_start += PAGE_SIZE;
    }
    return VM_MAP_SUCCESS;
}

pte_t* pte_retrieve(pagetable_t pagetable, addr_t virt_addr)
{
    if(virt_addr >= VM_ADDR_MAX_VAL)
    {
        panic("in pte_retrieve, virt_addr exceeded.");
    }
    for (int idx = 2; idx > 0; --idx)
    {
        pte_t* pte = &pagetable[VM_ADDR_IDX(virt_addr, idx)];
        if((*pte) & PTE_PERMISSION_V)
        {
            pagetable = (pagetable_t)(PTE_TO_PHY_ADDR(*pte));
        }
        else
        {
            pagetable = (pagetable_t)alloc_single_page();
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

void set_vm_pagetable(pagetable_t pagetable)
{
    sfence_vma();
    w_satp((SATP_SV39_MODE << RV64_SATP_MODE_OFFSET) | ADDR_TO_SATP((addr_t)pagetable));
    sfence_vma();
}

#endif /* SV39 */