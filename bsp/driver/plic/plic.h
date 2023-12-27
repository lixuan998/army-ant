#ifndef __PLIC_H__
#define __PLIC_H__

#include "arch/defs.h"
#include "lib/include/stdio.h"

#define PLIC_ENABLE                 (1)
#define PLIC_DISABLE                (0)

#define PLIC_BASE_ADDR              (0x10000000UL)
#define PLIC_PRIO_REG(n)            (PLIC_BASE_ADDR + 0x0000UL + 0x0004UL * n)   //PLIC Priority Register n.
#define PLIC_IP_REG(n)              (PLIC_BASE_ADDR + 0x1000UL + 0x0004UL * n)   //PLIC Interrupt Pending Register n.
#define PLIC_MIE_REG(n)             (PLIC_BASE_ADDR + 0x2000UL + 0x0004UL * n)   //PLIC Machine Mode Interrupt Enable Register n.
#define PLIC_SIE_REG(n)             (PLIC_BASE_ADDR + 0x2080UL + 0x0004UL * n)   //PLIC Superuser Mode Interrupt Enable Register n.
#define PLIC_CTRL_REG               (PLIC_BASE_ADDR + 0x1FFFFCUL)              //PLIC Control Register.
#define PLIC_MTH_REG                (PLIC_BASE_ADDR + 0x200000UL)              //PLIC Machine Threshold Register.
#define PLIC_MCLAIM_REG             (PLIC_BASE_ADDR + 0x200004UL)              //PLIC Machine Claim Register.
#define PLIC_STH_REG                (PLIC_BASE_ADDR + 0x201000UL)              //PLIC Superuser Threshold Register.
#define PLIC_SCLAIM_REG             (PLIC_BASE_ADDR + 0x201004UL)              //PLIC Superuser Claim Register.

#define PLIC_CTRL_REG_S_MODE_ACCESS 1UL

#define PLIC_INTERRUPT_MAX          256



void plic_s_mode_access();
void plic_interrupt_enable();
void plic_interrupt_disable();
#endif /* __PLIC_H__ */