
.section .text
.global _start
_start:
        sp = stack0
        la sp, stack0
        li a0, 1024*4
        csrr a1, mhartid
        addi a1, a1, 1
        mul a0, a0, a1
        add sp, sp, a0
        call boot_cfg
spin:
        j spin
        