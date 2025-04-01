#pragma once

#include <lib/stddef.h>
#include <lib/stdint.h>
#include <common/operation.h>
#include <arch/arch_defs.h>

#ifndef STATIC_MEM_SIZE
#define STATIC_MEM_PAGES                      (1024)
#endif

void static_mem_init();
addr_t static_mem_alloc();
void static_mem_free(addr_t addr);