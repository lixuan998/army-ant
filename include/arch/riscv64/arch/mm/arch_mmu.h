#pragma once

#include <lib/stdint.h>
#include <lib/stddef.h>
#include <common/error.h>
#include <arch/arch_defs.h>
#include <mm/mem_defs.h>
#include <mm/mmu.h>

#define PTE_PERMISSION_V                (1UL << 0)
#define PTE_PERMISSION_R                (1UL << 1)
#define PTE_PERMISSION_W                (1UL << 2)
#define PTE_PERMISSION_X                (1UL << 3)
#define PTE_PERMISSION_U                (1UL << 4)
#define PTE_PERMISSION_G                (1UL << 5)
#define PTE_PERMISSION_A                (1UL << 6)
#define PTE_PERMISSION_D                (1UL << 7)

#define VM_SO_STRONG_ORDER              (1UL << 63)
#define VM_C_CACHEABLE                  (0UL << 62)
#define VM_B_BUFFERABLE                 (0UL << 61)

#define RV32_SATP_MODE_OFFSET                 30
#define RV64_SATP_MODE_OFFSET                 (60UL)
#define SATP_BARE_MODE                        0UL
#define SATP_SV32_MODE                        1UL
#define SATP_SV39_MODE                        8UL
#define SATP_SV48_MODE                        9UL

#define VM_ADDR_IDX(addr, idx)          ((((uintptr_t) (addr)) >> (idx * 9 + 12)) & 0x1FF)
#define PTE_TO_PHY_ADDR(pte)            (((pte) >> 10) << 12)
#define PHY_ADDR_TO_PTE(phy_addr)       ((((uintptr_t)(phy_addr)) >> 12) << 10)
#define ADDR_TO_SATP(phy_addr)          (((uintptr_t)(phy_addr)) >> 12)



error_t arch_mmu_mapping(pgtbl_t* pagetable, addr_t virt_addr_start,
                         addr_t phys_addr_start, pgsize_t size,
                         uint64_t permisson, mem_type type);

pte_t* arch_pte_retrieve(pgtbl_t* pagetable, addr_t virt_addr,
                         mem_type type);

void arch_mmu_enable(pgtbl_t *pagetable);
void arch_mmu_disable(void);

void arch_kernel_mmu_init(void);