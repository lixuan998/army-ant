#include <arch/mm/arch_mem_layout.h>
#include <common/lock.h>
#include <common/log.h>
#include <mm/mem_allocator.h>
#include <mm/mem_paging.h>

static struct dl_list mem_pool_list;
static struct mem_pool kmem_pool = {0};

static void* do_find_from_dirty_slices(struct mem_pool _mpool);

void mem_allocator_init(struct mem_pool *_mem_pool, uint32_t size, enum MEM_POOL_FLAG flags)
{
    DL_LIST_INIT(&mem_pool_list);

    DL_LIST_INIT(&_mem_pool->mem_page_list);
    DL_LIST_INIT(&_mem_pool->mem_d_slice_list);
    spinlock_init(&_mem_pool->mpool_spinlock);
    uint8_t mm_pg_num = ((size + (size % PAGE_SIZE ? PAGE_SIZE : 0)) / PAGE_SIZE);
    for (int i = 0; i < mm_pg_num; i++) {
        struct mem_page *_mm_page = (struct mem_page *)mem_page_alloc();
        struct mem_page_meta *_meta = DL_LIST_CONTAINER(&_mm_page->mem_page_node, struct mem_page_meta, mem_page_node);
        KLOG_DEBUG(mm_tag, "Alloc page %p, size: %u, used: %u, meta addr: %p", (addr_t)_mm_page, _meta->size, _meta->used, _meta);
        DL_LIST_ADD(&_mem_pool->mem_page_list, &_mm_page->mem_page_node);
    }

    _mem_pool->size = size;
    _mem_pool->used = 0;
    _mem_pool->flag = flags;
    DL_LIST_ADD(&mem_pool_list, &_mem_pool->mem_pool_node);
}

void kmem_allocator_init(uint32_t size)
{
    mem_allocator_init(&kmem_pool, size, MEM_POOL_FLAG_KERNEL | MEM_POOL_FLAG_AUTO_ENLARGE);
}

void* mem_allocator_alloc(struct mem_pool *_mpool, size_t size)
{
    if (_mpool->flag == MEM_POOL_FLAG_NOT_INIT) {
        PANIC("Memory Allocator Not Initialized.");
    }
    
    spinlock_lock(&_mpool->mpool_spinlock);
    void *pmp = NULL; // pointer to memory piece.
    pmp = do_find_from_dirty_slices(*_mpool);
    if (pmp != NULL) {
        return pmp;
    }
    return NULL;
}

void* kmalloc(size_t size)
{
    if (kmem_pool.flag == MEM_POOL_FLAG_NOT_INIT) {
        PANIC("Memory Allocator Not Initialized.");
    }
    
    spinlock_lock(&kmem_pool.mpool_spinlock);
    void *pmp = NULL; // pointer to memory piece.
    pmp = do_find_from_dirty_slices(kmem_pool);
    if (pmp != NULL) {
        return pmp;
    }

    return NULL;
}

static void* do_find_from_dirty_slices(struct mem_pool _mpool)
{
    if (DL_LIST_EMPTY(&_mpool.mem_d_slice_list)) {
        return NULL;
    }
    return NULL;
}