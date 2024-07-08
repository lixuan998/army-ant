#ifndef __SD_H__
#define __SD_H__

#include "arch/riscv/include/riscv_type_defs.h"
#include "arch/riscv/include/riscv_basic_operation.h"

#include "smhc_defs.h"
#include "smhcsd_defs.h"

#include "../ccu/ccu.h"
#include "../gpio/gpio.h"

#include "lib/include/time.h"
#include "lib/include/string.h"

#define SD_CARD_INT_ENABLE                  1
#define SD_CARD_INT_DISABLE                 0

typedef struct _SD_CMD{
    char cmd_type[5];
    uchar cmd_index;
    uchar with_resp;
    uchar resp_type;        //0 for short response, 1 for long response.
    uchar check_resp_crc;   //Whether to check response's CRC.
    uchar with_data_trans;  //0 for No data transfer, 1 for data transfer.
    uchar trans_direction;  //0 for Read operation, 1 for Write operation.
    uchar trans_mode;       //0 for Block data transfer mode, 1 for Stream data transfer mode.
    uchar stop_cmd_flag;
    uchar wait_pre_over;    //Wait for data transfer complete before sending the current command.
    uchar stop_abt_cmd;
    uchar send_init_seq;
    uchar prg_clk;          //Change clock.
    uchar boot_mod;
    uchar exp_boot_ack;
    uchar boot_abt;
    uchar vol_sw;
    uint32 arg;
} SD_CMD;

typedef struct _SD_MMC_DEV {
	char name[32];
	void *priv;
	unsigned voltages;
	unsigned version;
	unsigned has_init;
	unsigned control_num;
	unsigned f_min;
	unsigned f_max;
	unsigned f_max_ddr;
	int high_capacity;
	unsigned bus_width;
	unsigned clock;
	unsigned card_caps;
	unsigned host_caps;
	unsigned ocr;
	unsigned scr[2];
	unsigned csd[4];
	unsigned cid[4];
	unsigned rca;/*unsigned short rca;*/
	unsigned part_config;/*char part_config;*/
	unsigned part_num;/*char part_num;*/
	unsigned tran_speed;
	unsigned read_bl_len;
	unsigned write_bl_len;
	unsigned erase_grp_size;
	unsigned long long capacity;
	// int (*send_cmd) (struct _SD_MMC_DEV *mmc,
	// 		 SD_CMD *cmd, struct mmc_data *data);
	// void (*set_ios) (struct _SD_MMC_DEV *mmc);
	// int (*init) (struct _SD_MMC_DEV *mmc);
	// int (*update_phase) (struct _SD_MMC_DEV *mmc);
	unsigned b_max;
	unsigned lba;/* number of blocks */
	unsigned blksz;/* block size */
	char revision[8 + 8];/*char revision[8+1];*/        /* CID:  PRV */

	uint speed_mode;
} SD_MMC_DEV;

void smhcn_init(int n);
void sd_mmc_int_enable(int enable);
void sd_mmc_int_mask(soc_reg_t mask);
void sd_card_dev_clk_set();
void sd_mmc_fifo_reset();
void sd_mmc_controller_reset();
void sd_mmc_set_bus_width(int bus_width);

SD_CMD phrase_sd_mmc_cmd(int cmd_type);
void sd_mmc_send_cmd(SD_CMD *cmd);

SMHC_REG_LIST *get_smhc_reg_list(soc_reg_t smhc_reg_base_addr);


#endif  /* __SD_H__ */