#include <arch/mm/arch_mem_layout.h>

.section .trampoline_section

.globl trampoline
trampoline:
    .align 4

.globl trap_to_kernel
trap_to_kernel:    
    # Save current sscratch
    csrw sscratch, a0
    li a0, VM_TRAPFRAME_ADDR

    # Save general-purpose registers
    sd ra,    0(a0)
    sd sp,    8(a0)
    sd gp,   16(a0)
    sd tp,   24(a0)
    sd t0,   32(a0)
    sd t1,   40(a0)
    sd t2,   48(a0)
    sd s0,   56(a0)
    sd s1,   64(a0)
    sd a1,   80(a0)
    sd a2,   88(a0)
    sd a3,   96(a0)
    sd a4,  104(a0)
    sd a5,  112(a0)
    sd a6,  120(a0)
    sd a7,  128(a0)
    sd s2,  136(a0)
    sd s3,  144(a0)
    sd s4,  152(a0)
    sd s5,  160(a0)
    sd s6,  168(a0)
    sd s7,  176(a0)
    sd s8,  184(a0)
    sd s9,  192(a0)
    sd s10, 200(a0)
    sd s11, 208(a0)
    sd t3,  216(a0)
    sd t4,  224(a0)
    sd t5,  232(a0)
    sd t6,  240(a0)

    # Store the previous sscratch value
    csrr t0, sscratch
    sd   t0, 72(a0)

    # Load new context information
    ld t1,  248(a0)  # satp (page table base)
    ld sp,  256(a0)  # process stack pointer
    ld t0,  264(a0)  # instruction entry point
    ld tp,  280(a0)  # thread pointer

    # Flush TLB and switch address space
    sfence.vma zero, zero
    csrw satp, t1
    sfence.vma zero, zero

    # Jump to the kernel irq handler
    jr t0

.globl back_to_user
back_to_user:
    # Flush TLB and restore user page table
    sfence.vma zero, zero
    csrw satp, a0
    sfence.vma zero, zero

    li a0, VM_TRAPFRAME_ADDR

    # Restore general-purpose registers
    ld ra,    0(a0)
    ld sp,    8(a0)
    ld gp,   16(a0)
    ld tp,   24(a0)
    ld t0,   32(a0)
    ld t1,   40(a0)
    ld t2,   48(a0)
    ld s0,   56(a0)
    ld s1,   64(a0)
    ld a1,   80(a0)
    ld a2,   88(a0)
    ld a3,   96(a0)
    ld a4,  104(a0)
    ld a5,  112(a0)
    ld a6,  120(a0)
    ld a7,  128(a0)
    ld s2,  136(a0)
    ld s3,  144(a0)
    ld s4,  152(a0)
    ld s5,  160(a0)
    ld s6,  168(a0)
    ld s7,  176(a0)
    ld s8,  184(a0)
    ld s9,  192(a0)
    ld s10, 200(a0)
    ld s11, 208(a0)
    ld t3,  216(a0)
    ld t4,  224(a0)
    ld t5,  232(a0)
    ld t6,  240(a0)

    # Restore user a0 and return to user mode
    ld a0,  72(a0)
    sret
