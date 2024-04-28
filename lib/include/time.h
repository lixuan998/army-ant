#ifndef __TIME_H__
#define __TIME_H__

#include "arch/defs.h"
#include "stdio.h"

typedef long long timestamp_t;

void msleep(int ms);
void sleep(int s);
void timestamp_update();

void get_timestamp_duration(timestamp_t t1, timestamp_t t2, uint32 *duration);
void start_timing();
void stop_timing(uint32 *duration);
#endif /* __TIME_H__ */