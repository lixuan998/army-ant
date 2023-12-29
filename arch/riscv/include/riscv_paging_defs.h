#ifndef __RISCV_PAGING_DEFS_H__
#define __RISCV_PAGING_DEFS_H__

#include "riscv_type_defs.h"

#define PAGE_SIZE                             (4096)
#define ALIGN_FLOOR(addr)                     (((addr)) & ~(PAGE_SIZE - 1))
#define ALIGN_CEIL(addr)                      (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

#define RV32_SATP_MODE_OFFSET                 30
#define RV64_SATP_MODE_OFFSET                 60
#define SATP_BARE_MODE                        0UL
#define SATP_SV32_MODE                        1UL
#define SATP_SV39_MODE                        8UL
#define SATP_SV48_MODE                        9UL

// Provided in linkld.ld
extern addr_t                                 kernel_start[];
extern addr_t                                 kernel_end[];
extern addr_t                                 kernel_text_end[];

#define UNALLOCTE_PAGE_VALUE                  0xFF
#define ALLOCATED_PAGE_VALUE                  0x00

typedef struct _MEM_PAGE{
    struct _MEM_PAGE *next;
} MEM_PAGE;

void *alloc_single_page();
void free_single_page(void *page);

#endif  /* __RISCV_PAGING_DEFS_H__ */