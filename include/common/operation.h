#pragma once

#include <lib/printk.h>
#include <lib/stdint.h>
#include <lib/stddef.h>
#include <arch/arch_operation.h>

#define ALIGN_FLOOR(addr, size) (((addr)) & ~((size) - 1))

#define ALIGN_CEIL(addr, size) (((addr) + (size) - 1) & ~((size) - 1))

#define IOMEM(addr) ((volatile void __iomem *)((long)(addr)))

#define _PANIC(msg, ...) \
    do { \
        printk("KERNEL PANIC[%s]:%d " msg "\r\n", __func__, __LINE__, ##__VA_ARGS__); \
        while (1); \
    } while (0)

#define PANIC(msg, ...) _PANIC(msg, ##__VA_ARGS__)

#define OFFSET_OF(type, member) ((uintptr_t)&(((type *)0)->member))

#define WRITE_REG32(reg_addr, val) \
    do { \
        volatile uint32_t *p_reg = (uint32_t *)((uintptr_t)(reg_addr)); \
        (*p_reg) = val; \
    } while (0)

#define READ_REG32(reg_addr) (*((uint32_t *)((uintptr_t)(reg_addr))))

#define WRITE_REG64(reg_addr, val) \
    do { \
        volatile uint64_t *p_reg = (uint64_t *)((uintptr_t)(reg_addr)); \
        (*p_reg) = val; \
    } while (0)

#define READ_REG64(reg_addr) (*((volatile uint64_t *)((uintptr_t)(reg_addr))))
