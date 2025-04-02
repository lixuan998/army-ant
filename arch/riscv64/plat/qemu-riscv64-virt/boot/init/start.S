#include <arch/asm.h>
#include <arch/mm/arch_mem_layout.h>

.section .startup
.comm init_stack, BOOT_STACK_SIZE * CPU_NUM

FUNC_BEGIN(_start)
    # Set up sp register
    lla   t0, init_stack
    csrr  t1, mhartid
    add   t1, t1, 1
    li    t2, BOOT_STACK_SIZE
    mul   t1, t1, t2 
    # sp == addr(init_stack) + BOOT_STACK_SIZE
    add   sp, t0, t1

    csrr  t1, mhartid
    li    t2, 0
    bne   t1, t2, boot_cfg

    # Clear BSS Seg
    la    t0, __bss_start
    la    t1, __bss_end
    # Check if t1 <= t0
    ble   t1, t0, bss_done
clear_bss:
    sd    zero, (t0)
    add   t0, t0, 8
    blt   t0, t1, clear_bss
bss_done:
    call  boot_cfg
FUNC_END(_start)
