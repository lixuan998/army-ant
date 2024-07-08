#ifndef __RISCV_MEM_DEFS_H__
#define __RISCV_MEM_DEFS_H__

#include "riscv_spinlock_defs.h"
#include "riscv_vm_defs.h"

void *alloc_single_page();
void free_single_page(addr_t page);
void free_pages(addr_t p_start, addr_t p_end);
void mem_paging_init();

addr_t allocate_shared_memory(PROC *procs[], int proc_num);

void heap_mem_blk_init(HEAP_MEM_BLK *blk);
void heap_mem_blk_cell_use(HEAP_MEM_BLK *blk, int cell_num);
int is_heap_mem_blk_cell_used(HEAP_MEM_BLK *blk, int cell_num);
void heap_mem_blk_cell_free(HEAP_MEM_BLK *blk, int cell_num);

void *k_malloc(int size);
void k_free(void *ptr);

#endif  /* __RISCV_MEM_DEFS_H__ */