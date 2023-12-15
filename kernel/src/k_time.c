#include "../include/k_time.h"

static time_stamp_t time_stamp, cur_time_stamp;

void k_msleep(int ms)
{
    time_stamp_t tmp_time_stamp = time_stamp;
    uint32 duration = 0;
    while(duration < ms)
    {
        k_get_time_stamp_duration(tmp_time_stamp, time_stamp, &duration);
    }
}    

void k_sleep(int s)
{
    time_stamp_t tmp_time_stamp = time_stamp;
    uint32 duration = 0;
    while(duration < 1000 * s)
    {
        k_get_time_stamp_duration(tmp_time_stamp, time_stamp, &duration);
    }
}

void k_time_stamp_init()
{
    time_stamp = 0;
}

void k_time_stamp_update()
{
    time_stamp ++;    
}

volatile void k_get_time_stamp_duration(time_stamp_t t1, time_stamp_t t2, uint32 *duration)
{
    *duration = (t2 - t1);
}

void k_start_timing()
{
    cur_time_stamp = time_stamp;
}

void k_stop_timing(uint32 *duration)
{
    k_get_time_stamp_duration(cur_time_stamp, time_stamp, duration);
}
