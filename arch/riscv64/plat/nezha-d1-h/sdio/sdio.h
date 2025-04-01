#ifndef __SDIO_H__
#define __SDIO_H__

#include "smhc_defs.h"
#include "smhcsd_defs.h"
#include "lib/include/time.h"

SMHC_REG_LIST *get_smhc_reg_list(int smhc_no);

int sdio_init(int smhc_no, MMCSD_HOST *host);

int sdio_transfer_cmd(int smhc_no, SDIO_CMD *cmd, SDIO_DATA *data);
int sdio_transfer_data(int smhc_no, SDIO_CMD *cmd, SDIO_DATA *data);
int sdio_transfer(int smhc_no, SDIO_CMD *cmd, SDIO_DATA *data);
int sdio_read_data(int smhc_no, uint32 *data_buf, uint32 blk_count, uint32 blk_size);
int sdio_write_data(int smhc_no, uint32 *data_buf, uint32 blk_count, uint32 blk_size);

int sdio_update_clock(int smhc_no);
int sdio_set_clock(int smhc_no, uint32 clk);
int sdio_set_bus_width(int smhc_no, uint32 bus_width);

#endif /* __SDIO_H__ */