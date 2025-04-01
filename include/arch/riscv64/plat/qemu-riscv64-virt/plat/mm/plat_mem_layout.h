#pragma once
#ifndef __ASSEMBLER__
extern char __peripheral_start[];
extern char __peripheral_end[];
extern char __base_addr[];
extern char __top_addr[];
extern char __kernel_end[];
extern char __text_start[];
extern char __text_end[];
extern char __trampoline_start[];
extern char __trampoline_end[];

extern char trampoline[];
#endif

#define STACK_SIZE      4096
#define STACK_ALIGNMENT 16

#define BOOT_STACK_SIZE 4096

#define PAGE_SIZE                             (4096UL)
#define SQRT_PAGE_SIZE                        (64UL)

#define VM_ADDR_MAX_VAL                       (0xa0000000)

#define VM_TRAMPOLINE_ADDR                    (VM_ADDR_MAX_VAL - PAGE_SIZE)
#define VM_TRAPFRAME_ADDR                     (VM_TRAMPOLINE_ADDR - PAGE_SIZE)
