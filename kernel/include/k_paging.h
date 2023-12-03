#ifndef __K_PAGING_H__
#define __K_PAGING_H__
#include "k_defs.h"
#include "k_spinlock.h"
#include "k_stdlib.h"

#define UNALLOCTE_PAGE_VALUE 0xFF
#define ALLOCATED_PAGE_VALUE 0x00

typedef struct _MEM_PAGE{
    struct _MEM_PAGE *next;
} MEM_PAGE;

void k_mem_paging_init();
void k_free_pages(void *p_start, void *p_end);
void k_free_single_page(void *page);
void *k_alloc_single_page();

#endif /* __K_PAGING_H__ */