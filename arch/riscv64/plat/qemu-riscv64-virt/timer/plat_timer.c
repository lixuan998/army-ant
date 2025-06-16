#include <plat/timer/plat_timer.h>
#include <common/log.h>

#define TIME_BASE_FREQ      10000000UL

static uint64_t tim_interval = 10; // 10ms
static uint64_t tim_tick_cnt = 0;

static void _plat_timer_reload(void)
{
    WRITE_CSR(0x14d, READ_CSR(time) + (TIME_BASE_FREQ / tim_interval));
}

void plat_timer_init(void)
{
    WRITE_CSR(mie, READ_CSR(mie) | xIE_STIE_MSK);
    
    // enable the sstc extension (i.e. stimecmp).
    WRITE_CSR(0x30a, READ_CSR(0x30a) | (1UL << 63));

    // allow supervisor to use stimecmp and time.
    WRITE_CSR(mcounteren, READ_CSR(mcounteren) | 2UL);

    _plat_timer_reload();
}

void plat_timer_irq_handler(void)
{
    // KLOG_DEBUG("TIMER IRQ HANDLER", "hart %d's timer triggered", READ_HARTID());
    tim_tick_cnt += 1;
    _plat_timer_reload();
}