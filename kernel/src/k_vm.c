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
    for(int i = 0; i < 512; ++ i)
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
    // 检查页大小是否有效
    if(size == 0)
    {
        panic("in vm_mapping, page size invalid.");
    }
    volatile pte_t *pte;
    // 对虚拟地址进行向下对齐
    virt_addr_start = ALIGN_FLOOR(virt_addr_start);
    // 计算虚拟地址的结束地址
    addr_t virt_addr_end = ALIGN_FLOOR(virt_addr_start + size);
    // 对物理地址进行向下对齐，虽然不是必须的，但为了使整个过程更明确
    phys_addr_start = ALIGN_FLOOR(phys_addr_start);
    // 遍历虚拟地址和物理地址
    while(virt_addr_start < virt_addr_end)
    {
        // 获取页表项
        pte = pte_retrieve(pagetable, virt_addr_start);
        // 如果页表项为空，则返回失败
        if(pte == NULL)
        {
            printf("in vm_mapping, pte is NULL.");
            return VM_MAP_FAILED;
        }
        // 如果页表项已经被映射，则返回失败
        if((*pte) & PTE_PERMISSION_V)
        {
            panic("in vm_mapping, pte already mapped.");
        }
        // 设置页表项的物理地址和权限
        (*pte) = (PHY_ADDR_TO_PTE(phys_addr_start) | permisson | PTE_PERMISSION_V);
        // 物理地址和虚拟地址都增加一页的大小
        phys_addr_start += PAGE_SIZE;
        virt_addr_start += PAGE_SIZE;
    }
    // 返回成功
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

// 设置虚拟内存页表
void set_vm_pagetable(pagetable_t pagetable)
{
    // 禁止缓存和写入操作，直到所有之前的内存操作完成
    sfence_vma();
    // 设置SATP寄存器的值为虚拟内存页表的地址
    w_satp((SATP_SV39_MODE << RV64_SATP_MODE_OFFSET) | ADDR_TO_SATP((addr_t)pagetable));
    // 禁止缓存和写入操作，直到所有之前的内存操作完成
    sfence_vma();
}

#endif /* SV39 */