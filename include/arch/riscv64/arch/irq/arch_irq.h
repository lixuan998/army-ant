#pragma once

#include <common/operation.h>
#include <lib/stddef.h>

void arch_irq_init(void);
void arch_irq_enable(void);
void arch_irq_disable(void);
uint8_t arch_irq_lock(void);
void arch_irq_unlock(uint8_t prev);