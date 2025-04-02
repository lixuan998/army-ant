#pragma once

#include <mm/mem_defs.h>
#include <common/error.h>
#include <arch/arch_defs.h>

#define MMU_MAP_TBL_MAX_ENTRYS 32

typedef uintptr_t pgtbl_t;
typedef uintptr_t pte_t;
typedef uint64_t  pgsize_t;

typedef struct mmu_map_tbl{
    addr_t virt_addr_start;
    addr_t phys_addr_start;
    pgsize_t size;
    uint64_t permisson;
} mmu_map_tbl;

error_t pgtbl_insert(pgtbl_t* pgtbl, mmu_map_tbl map_tbl[],
                     uint32_t tbl_entry, mem_type type);

pgtbl_t* pgtbl_create(mmu_map_tbl map_tbl[], uint32_t tbl_entrys,
                      mem_type type);
void pgtbl_destroy(pgtbl_t* pagetable, mem_type type);
pgtbl_t* get_kernel_pgtbl();
void kernel_mmu_init();
void mmu_enable(pgtbl_t *pagetable);
void mmu_disable();