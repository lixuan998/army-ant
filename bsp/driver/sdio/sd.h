#ifndef __SD_H__
#define __SD_H__

#include "arch/riscv/include/riscv_type_defs.h"
#include "arch/riscv/include/riscv_basic_operation.h"

#include "smhc_defs.h"
#include "sdio.h"
#include "smhcsd_defs.h"

#include "../ccu/ccu.h"
#include "../gpio/gpio.h"

#include "lib/include/time.h"
#include "lib/include/string.h"
#include "lib/include/stdlib.h"

#define SD_CARD_INT_ENABLE                  1
#define SD_CARD_INT_DISABLE                 0

int mmcsd_app_cmd(MMCSD_HOST *host, MMCSD_CARD *card);
int mmcsd_send_app_cmd(MMCSD_HOST *host, MMCSD_CARD *card, SDIO_CMD *cmd);
int mmcsd_send_app_op_cond(MMCSD_HOST *host, uint32 ocr, uint32 *resp_ocr);
int mmcsd_go_idle(int smhc_no);

void mmcsd_power_up(MMCSD_HOST *host);

void init_sd_card(MMCSD_HOST *host, uint32 ocr);


void smhcn_init(int n);
void sd_mmc_int_enable(int enable);
void sd_mmc_int_mask(soc_reg_t mask);
void sd_mmc_fifo_reset();
void sd_mmc_controller_reset();
void sd_mmc_set_bus_width(int bus_width);


SMHC_REG_LIST *get_smhc_reg_list(int smhc_no);

#endif  /* __SD_H__ */