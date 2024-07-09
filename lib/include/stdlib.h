#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "arch/riscv/include/riscv_type_defs.h"
#include "stdio.h"

#define META_DATA_SIZE        (sizeof(int))
#define USER_MEM_BLK          (0)
#define KERNEL_MEM_BLK        (1)

void memset(void *ptr, int value, int size);

void memcpy(char *dest, char *src, int size);

int memcmp(void *s1, void *s2, int n);

char *strchr(char *s, int c);
// void *malloc(int size);

// void free(void *ptr);

void debug_malloc(HEAP_MEM_BLK *blk);

#endif /* __STDLIB_H__ */