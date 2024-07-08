#ifndef __TIMER_H__
#define __TIMER_H__

#include "arch/riscv/include/riscv_type_defs.h"
#include "arch/riscv/include/riscv_basic_operation.h"

#define TIMER_BASE_ADDR                      (0x02050000)
#define TMR_IRQ_EN_REG                       (TIMER_BASE_ADDR + 0x0000)   //Timer IRQ Enable Register.
#define TMR_IRQ_STA_REG                      (TIMER_BASE_ADDR + 0x0004)   //Timer Status Register.
#define TMRn_CTRL_REG(n)                     (TIMER_BASE_ADDR + 0x0010 + (0x0010 * n))  //Timer n Control Register.
#define TMRn_INTV_VALUE_REG(n)               (TIMER_BASE_ADDR + 0x0014 + (0x0010 * n))  //Timer n Interval Value Register.
#define TMRn_CUR_VALUE_REG(n)                (TIMER_BASE_ADDR + 0x0018 + (0x0010 * n))  //Timer n Current Value Register. 

#define TMRn_CTRL_EN_OFFSET                  (0)
#define TMRn_CTRL_RELOAD_OFFSET              (1)
#define TMRn_CTRL_CLK_SRC_OFFSET             (2)
#define TMRn_CLK_PRES_OFFSET                 (4)

#define TMRn_CTRL_PRES_DIVEDE_1              (0)
#define TMRn_CTRL_PRES_DIVEDE_2              (1)
#define TMRn_CTRL_PRES_DIVEDE_4              (2)
#define TMRn_CTRL_PRES_DIVEDE_8              (3)
#define TMRn_CTRL_PRES_DIVEDE_16             (4)
#define TMRn_CTRL_PRES_DIVEDE_32             (5)
#define TMRn_CTRL_PRES_DIVEDE_64             (6)
#define TMRn_CTRL_PRES_DIVEDE_128            (7)

#define TIMER_FREQUENCY                      (24000000)

void timer_interrupt_enable(int n);
void timer_pending_clear(int n);
void timer_set_prescale(int n, int prescale);
void timer_start(int n, int tick);
void timer_stop(int n);

#endif /* __TIMER_H__ */