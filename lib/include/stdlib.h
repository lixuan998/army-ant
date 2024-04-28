#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "arch/defs.h"
#include "stdio.h"

#define META_DATA_SIZE        (sizeof(int))
#define USER_MEM_BLK          (0)
#define KERNEL_MEM_BLK        (1)

void memset(void *ptr, int value, int size);

void memcpy(void *dest, void *src, int size);

void blk_init(MEM_BLK *blk);

void blk_cell_use(MEM_BLK *blk, int cell_num);

int is_blk_cell_used(MEM_BLK *blk, int cell_num);

void blk_cell_free(MEM_BLK *blk, int cell_num);

void malloc_init();

void *malloc(int size);

void mem_blk_head_switch(int is_user, void *mem_blk_head);

void free(void *ptr);

void debug_malloc(MEM_BLK *blk);

#endif /* __STDLIB_H__ */