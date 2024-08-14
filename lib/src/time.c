#include "../include/time.h"

volatile static timestamp_t timestamp = 0;
volatile static timestamp_t cur_timestamp;

void msleep(int ms)
{
    timestamp_t tmp_timestamp = timestamp;
    uint32 duration = 0;
    while(duration < ms)
    {
        get_timestamp_duration(tmp_timestamp, timestamp, &duration);
    }
}    

void sleep(int s)
{
    timestamp_t tmp_timestamp = timestamp;
    uint32 duration = 0;
    while(duration < 1000 * s)
    {
        get_timestamp_duration(tmp_timestamp, timestamp, &duration);
    }
}

void timestamp_update()
{
    timestamp ++;    
}

volatile void get_timestamp_duration(timestamp_t t1, timestamp_t t2, uint32 *duration)
{
    *duration = (t2 - t1);
}

void start_timing()
{
    cur_timestamp = timestamp;
}

void timing_duration(uint32 *duration)
{
    get_timestamp_duration(cur_timestamp, timestamp, duration);
}
