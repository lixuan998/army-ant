
.section .text
.global _start
_start:

        call boot_cfg
spin:
        j spin
        