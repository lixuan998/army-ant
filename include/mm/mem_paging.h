#pragma once

#include <arch/arch_defs.h>
#include <common/dl_list.h>

#define FREE_PAGE_VALUE      0xFF
#define ALLOCATED_PAGE_VALUE 0x00

struct mem_page {
    struct dl_list mem_page_node;
};

struct mem_page_meta {
    struct dl_list *mem_page_node;
    uint16_t size;
    uint16_t used;
};

#define GET_MEM_PAGE_META(page) \
    (((struct mem_page_meta *)((addr_t)__kernel_end)) + ((addr_t)__top_addr - (addr_t)page) / PAGE_SIZE)

void mem_paging_init();
addr_t mem_page_alloc();
void mem_page_free(addr_t page);
void mem_page_range_free(addr_t start, addr_t end);