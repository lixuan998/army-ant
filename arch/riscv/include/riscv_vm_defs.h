#ifndef __RISCV_VM_DEFS_H__
#define __RISCV_VM_DEFS_H__

#include "riscv_type_defs.h"
#include "riscv_paging_defs.h"
#include "riscv_mem_layout.h"

#define SV39

#define VM_ADDR_IDX(addr, idx)                   ((((isa_reg_t) (addr)) >> (idx * 9 + 12)) & 0x1FF)
#define PTE_TO_PHY_ADDR(pte)                  (((pte) >> 10) << 12)
#define PHY_ADDR_TO_PTE(phy_addr)             ((((addr_t)(phy_addr)) >> 12) << 10)
#define ADDR_TO_SATP(phy_addr)                 (((addr_t)(phy_addr)) >> 12)

#define PTE_PERMISSION_V                      (1UL << 0)
#define PTE_PERMISSION_R                      (1UL << 1)
#define PTE_PERMISSION_W                      (1UL << 2)
#define PTE_PERMISSION_X                      (1UL << 3)
#define PTE_PERMISSION_U                      (1UL << 4)
#define PTE_PERMISSION_G                      (1UL << 5)
#define PTE_PERMISSION_A                      (1UL << 6)
#define PTE_PERMISSION_D                      (1UL << 7)

#define VM_SO_STRONG_ORDER                    (1UL << 63)
#define VM_C_CACHEABLE                        (0UL << 62)
#define VM_B_BUFFERABLE                       (0UL << 61)

#define VM_MAP_SUCCESS                        (0)
#define VM_MAP_FAILED                         (-1)

#define VM_K_STACK_ADDR(pid)                  ((VM_TRAMPOLINE_ADDR - ((pid) + 1) * 2 * PAGE_SIZE))

typedef struct _VM_MAP_INFO{
    addr_t virt_addr_start;
    addr_t phys_addr_start;
    pagesize_t size;
    volatile isa_reg_t permisson;
} VM_MAP_INFO;

pagetable_t pagetable_create(VM_MAP_INFO map_info[], int num_of_mapping);
void pagetable_entry_add(pagetable_t pagetable, VM_MAP_INFO map_info[], int num_of_mapping);
int vm_mapping(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, isa_reg_t permisson);
pte_t* pte_retrieve(pagetable_t pagetable, addr_t virt_addr);
void set_vm_pagetable(pagetable_t pagetable);

extern HEAP_MEM_BLK *mem_blk_list;

#endif  /* __RISCV_VM_DEFS_H__ */