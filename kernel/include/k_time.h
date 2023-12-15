#ifndef __K_TIME_H__
#define __K_TIME_H__

#include "arch/defs.h"
#include "k_stdio.h"
#include "k_spinlock.h"
#include "bsp/driver/timer/timer.h"

typedef long long time_stamp_t;

void k_msleep(int ms);
void k_sleep(int s);
void k_time_stamp_init();
void k_time_stamp_update();

void k_get_time_stamp_duration(time_stamp_t t1, time_stamp_t t2, uint32 *duration);
void k_start_timing();
void k_stop_timing(uint32 *duration);
#endif /* __K_TIME_H__ */