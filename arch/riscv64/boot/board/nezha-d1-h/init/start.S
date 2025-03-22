#include <common/asm.h>
#include <common/config.h>

.section .startup
.comm init_stack, BOOT_STACK_SIZE

FUNC_BEGIN(_start)
    # Set up sp register
    lla   t0, init_stack
    li    t1, BOOT_STACK_SIZE
    # sp == addr(init_stack) + BOOT_STACK_SIZE
    add   sp, t0, t1

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
