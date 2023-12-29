#include "../include/k_paging.h"

void k_mem_paging_init()
{
    extern SPINLOCK mem_paging_spinlock;
    k_spinlock_init(&mem_paging_spinlock, "mem_paging_spinlock");
    k_free_pages((addr_t *)(kernel_end), (addr_t *)(RAM_TOP));    
}

void k_free_pages(void *p_start, void *p_end)
{
    addr_t *pg = (addr_t*)ALIGN_CEIL((addr_t)p_start);
    for(; pg + PAGE_SIZE <= (addr_t *)p_end; pg += PAGE_SIZE)
    {
        k_free_single_page(pg);
    }
}

void k_free_single_page(void *page)
{
    free_single_page(page);
}

void *k_alloc_single_page()
{
    return alloc_single_page();
}
