#ifndef __VM_H__
#define __VM_H__

#include "arch/riscv/include/riscv_defs.h"
#include "kernel/include/k_paging.h"
#include "kernel/include/k_stdio.h"

#define ADDR_MAX_VAL        1UL << (9 + 9 + 9 + 12)

#define ADDR_IDX(addr, idx)       ((((uint64) (addr)) >> (idx * 9 + 12)) & 0x1FF)
#define PTE_TO_PHY_ADDR(pte)      (((pte) >> 10) << 12)
#define PHY_ADDR_TO_PTE(phy_addr) ((((addr_t)(phy_addr)) >> 12) << 10)
#define PHY_TO_SATP(phy_addr)     (((addr_t)(phy_addr)) >> 12)

#define PTE_PERMISSION_V    1UL << 0
#define PTE_PERMISSION_R    1UL << 1
#define PTE_PERMISSION_W    1UL << 2
#define PTE_PERMISSION_X    1UL << 3
#define PTE_PERMISSION_U    1UL << 4
#define PTE_PERMISSION_G    1UL << 5
#define PTE_PERMISSION_A    1UL << 6
#define PTE_PERMISSION_D    1UL << 7

#define VM_SO_STRONG_ORDER  1UL << 63
#define VM_C_CACHEABLE      1UL << 62
#define VM_B_BUFFERABLE     1UL << 61

#define VM_MAP_SUCCESS      0
#define VM_MAP_FAILED       -1


int vm_map(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, uint64 permisson);
pte_t* pte_retrieve(pagetable_t pagetable, addr_t virt_addr);
void vm_enable(pagetable_t pagetable);

#endif /* __VM_H__ */