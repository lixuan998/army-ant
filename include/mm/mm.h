#pragma once

#include <mm/mem_paging.h>
#include <mm/static_mem.h>
#include <mm/mmu.h>

static inline void mm_init(void)
{
    static_mem_init();
    mem_paging_init();
    kernel_mmu_init();
}

static inline void secondary_mm_init(void)
{
    pgtbl_t *__kernel_pgtbl = get_kernel_pgtbl();
    mmu_enable(__kernel_pgtbl);
}