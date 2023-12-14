#include "../include/k_time.h"

volatile static TIMESTAMP time_stamp, cur_time_stamp;

void k_msleep(int ms)
{
    volatile TIMESTAMP tmp_time_stamp = time_stamp;
    while(k_get_time_stamp_duration(tmp_time_stamp, time_stamp) < ms);
}

void k_sleep(int s)
{
    volatile TIMESTAMP tmp_time_stamp = time_stamp;
    while(k_get_time_stamp_duration(tmp_time_stamp, time_stamp) < 1000 * s);
}

void k_time_stamp_init()
{
    time_stamp.day = 0;
    time_stamp.hour = 0;
    time_stamp.minute = 0;
    time_stamp.second = 0;
    time_stamp.millisecond = 0;
}

void k_time_stamp_update()
{
    time_stamp.millisecond += 1;
    int carry = time_stamp.millisecond / 1000;
    time_stamp.millisecond = time_stamp.millisecond % 1000;

    time_stamp.second += carry;
    carry = time_stamp.second / 60;
    time_stamp.second = time_stamp.second % 60;

    time_stamp.minute += carry;
    carry = time_stamp.minute / 60;
    time_stamp.minute = time_stamp.minute % 60;

    time_stamp.hour += carry;
    carry = time_stamp.hour / 24;
    time_stamp.hour = time_stamp.hour % 24;

    time_stamp.day += carry;
    
    // k_printf("time_stamp: %d:%d:%d:%d:%d\n\r", time_stamp.day, time_stamp.hour,time_stamp.minute, time_stamp.second, time_stamp.millisecond);
}

volatile uint32 k_get_time_stamp_duration(TIMESTAMP t1, TIMESTAMP t2)
{
    timer_stop(0);
    uint32 ms1, ms2;
    ms1 = t1.day * 24 * 60 * 60 * 1000 + t1.hour * 60 * 60 * 1000 + t1.minute * 60 * 1000 + t1.second * 1000 + t1.millisecond;
    ms2 = t2.day * 24 * 60 * 60 * 1000 + t2.hour * 60 * 60 * 1000 + t2.minute * 60 * 1000 + t2.second * 1000 + t2.millisecond;
    timer_start(0, 1000);
    return ms2 - ms1;
}

void k_start_timing()
{
    cur_time_stamp = time_stamp;
}

void k_stop_timing(uint32 *duration)
{
    *duration = k_get_time_stamp_duration(cur_time_stamp, time_stamp);
}