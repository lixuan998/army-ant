#pragma once

#include <lib/stddef.h>

void irq_init(void);
void irq_enable();
void irq_disable();
uint8_t irq_lock();
void irq_unlock(uint8_t prev);