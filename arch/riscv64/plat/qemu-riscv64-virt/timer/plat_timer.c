#include <plat/timer/plat_timer.h>
#include <common/log.h>

void plat_timer_init(void)
{
    WRITE_CSR(mie, READ_CSR(mie) | xIE_MTIE_MSK);
     
    // enable the sstc extension (i.e. stimecmp).
    WRITE_CSR(0x30a, READ_CSR(0x30a) | (1UL << 63));

    // allow supervisor to use stimecmp and time.
    WRITE_CSR(mcounteren, READ_CSR(mcounteren) | 2UL);

    // ask for the very first timer interrupt.
    WRITE_CSR(mcounteren, READ_CSR(mcounteren) | 2UL);

    // Set timer interval (about 1/10 sec).
    WRITE_CSR(0x14d, READ_CSR(time) + 1000000);
}

void plat_timer_irq_handler(void)
{
    KLOG_DEBUG("TIMER IRQ HANDLER", "timer triggered");
    WRITE_CSR(0x14d, READ_CSR(time) + 1000000);
}