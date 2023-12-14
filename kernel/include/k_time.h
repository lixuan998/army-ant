#ifndef __K_TIME_H__
#define __K_TIME_H__

#include "arch/defs.h"
#include "k_stdio.h"
#include "k_spinlock.h"
#include "bsp/driver/timer/timer.h"

typedef struct _TIMESTAMP{
    uint32 day;
    uint32 hour;
    uint32 minute;
    uint32 second;
    uint32 millisecond;
} TIMESTAMP;

void k_msleep(int ms);
void k_sleep(int s);
void k_time_stamp_init();
void k_time_stamp_update();

volatile uint32 k_get_time_stamp_duration(TIMESTAMP t1, TIMESTAMP t2);
void k_start_timing();
void k_stop_timing(uint32 *duration);
#endif /* __K_TIME_H__ */