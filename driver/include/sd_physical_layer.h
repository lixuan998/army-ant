#ifndef __SD_PHYSICAL_LAYER__
#define __SD_PHYSICAL_LAYER__

#include "arch/defs.h"

typedef struct _SD_CMD{
    char cmd_type[5];
    uchar cmd_index;
    uchar with_resp;
    uchar resp_type;    //0 for short response, 1 for long response.
    uchar check_resp_crc;   //Whether to check response's CRC.
    uchar with_data_trans;
    uchar trans_direction;
    uint32 args;
} SD_CMD;

#endif /* __SD_PHYSICAL_LAYER__ */