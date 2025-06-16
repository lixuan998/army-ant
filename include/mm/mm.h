#pragma once

#include <arch/mm/arch_mem_layout.h>
#include <mm/mem_paging.h>
#include <mm/static_mem.h>
#include <mm/mmu.h>
#include <mm/mem_allocator.h>

static inline void mm_init(void)
{
    static_mem_init();
    mem_paging_init();
    kernel_mmu_init();
    kmem_allocator_init(PAGE_SIZE * 10);
}

static inline void secondary_mm_init(void)
{
    pgtbl_t *__kernel_pgtbl = get_kernel_pgtbl();
    mmu_enable(__kernel_pgtbl);
}