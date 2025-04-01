#include "arch/riscv/include/riscv_vm_defs.h"
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

void pagetable_destroy(pagetable_t pagetable)
{
    for(int i = 0; i < 512; ++i)
    {
        pte_t pte = pagetable[i];
        if((pte & PTE_PERMISSION_V) && ((pte & PTE_PERMISSION_R) == 0))
        {
            addr_t pagetable_child = PTE_TO_PHY_ADDR(pte);
            pagetable_destroy((pagetable_t)pagetable_child);
            pagetable[i] = NULL;
        }
    }

    free_single_page((addr_t)pagetable);
    pagetable = NULL;
}

pte_t* pte_retrieve(pagetable_t pagetable, addr_t virt_addr)
{
    if(virt_addr >= VM_ADDR_MAX_VAL)
    {
        PANIC("in pte_retrieve, virt_addr exceeded.");
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