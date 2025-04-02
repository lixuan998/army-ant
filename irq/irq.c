#include <arch/irq/arch_irq.h>
#include <irq/irq.h>

void irq_init(void)
{
    arch_irq_init();
}

void irq_enable()
{
    arch_irq_enable();
}
void irq_disable()
{
    arch_irq_disable();
}

uint8_t irq_lock()
{
    return arch_irq_lock();
}

void irq_unlock(uint8_t prev)
{
    arch_irq_unlock(prev);
}