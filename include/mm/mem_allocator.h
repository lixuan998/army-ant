#pragma once

#include <common/dl_list.h>
#include <common/lock.h>

#include <mm/mem_paging.h>

#include <lib/stddef.h>

enum MEM_POOL_FLAG {
    MEM_POOL_FLAG_NOT_INIT      = 0x0,
    MEM_POOL_FLAG_KERNEL        = 0x1,
    MEM_POOL_FLAG_USER          = 0x2,
    MEM_POOL_FLAG_AUTO_ENLARGE  = 0x4,
};

struct mem_slice {
    uint16_t offset;
    uint16_t size;
    struct dl_list mem_page_node;
    struct dl_list mem_slice_node;
};

struct mem_pool {
    uint32_t size;
    uint32_t used; 
    enum MEM_POOL_FLAG flag;
    spinlock_t mpool_spinlock;
    struct dl_list mem_pool_node;
    struct dl_list mem_page_list;
    struct dl_list mem_d_slice_list; // Dirty slices list.
};

void mem_allocator_init(struct mem_pool *_mem_pool, uint32_t size, enum MEM_POOL_FLAG flags);
void kmem_allocator_init(uint32_t size);

void* mem_allocator_alloc(struct mem_pool *_mpool, size_t size);
void mem_allocator_free(struct mem_pool *_mpool, void *ptr);

void* kmalloc(size_t size);
void kfree(void *ptr);