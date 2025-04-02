#pragma once

#include <lib/stddef.h>
#include <lib/stdint.h>

typedef struct _TRAPFRAME{
    uintptr_t ra;        // offset: 0
    uintptr_t sp;        // offset: 8
    uintptr_t gp;        // offset: 16
    uintptr_t tp;        // offset: 24
    uintptr_t t0;        // offset: 32
    uintptr_t t1;        // offset: 40
    uintptr_t t2;        // offset: 48
    uintptr_t s0;        // offset: 56
    uintptr_t s1;        // offset: 64
    uintptr_t a0;        // offset: 72
    uintptr_t a1;        // offset: 80
    uintptr_t a2;        // offset: 88
    uintptr_t a3;        // offset: 96
    uintptr_t a4;        // offset: 104
    uintptr_t a5;        // offset: 112
    uintptr_t a6;        // offset: 120
    uintptr_t a7;        // offset: 128
    uintptr_t s2;        // offset: 136
    uintptr_t s3;        // offset: 144
    uintptr_t s4;        // offset: 152
    uintptr_t s5;        // offset: 160
    uintptr_t s6;        // offset: 168
    uintptr_t s7;        // offset: 176
    uintptr_t s8;        // offset: 184
    uintptr_t s9;        // offset: 192
    uintptr_t s10;       // offset: 200
    uintptr_t s11;       // offset: 208
    uintptr_t t3;        // offset: 216
    uintptr_t t4;        // offset: 224
    uintptr_t t5;        // offset: 232
    uintptr_t t6;        // offset: 240
    uintptr_t _pgtbl;    // offset: 248   pagetable in kernel mode.
    uintptr_t _sp;       // offset: 256   used in kernel mode for processes when they trap to the kernel mode.
    uintptr_t _irq_vec;  // offset: 264
    uintptr_t epc;       // offset: 272   saved user program counter.
    uintptr_t pid;       // offset: 280
};

struct context {
    uintptr_t ra;
    uintptr_t sp;
    uintptr_t s0;
    uintptr_t s1;
    uintptr_t s2;
    uintptr_t s3;
    uintptr_t s4;
    uintptr_t s5;
    uintptr_t s6;
    uintptr_t s7;
    uintptr_t s8;
    uintptr_t s9;
    uintptr_t s10;
    uintptr_t s11;
};