#include "sd.h"

static volatile SMHC_REG_LIST *smhc0_reg_list;
static SD_CMD sd_cmd;

void smhcn_init(int n)
{
    smhc0_reg_list = get_smhc_reg_list(SMHCn_BASE_ADDR(SMHC0));
    if (n == 0)
    {
        N_GPIO_CFG smhc0_gpio_cfg[] = {
            {.port = GPIO_PF,
             .port_num = 0,
             .func_select = 0x0002,
             .drv_level = 0x0002,
             .pull_state = 0x0002,
             .data = 0,
             .data_read = 0},
            {.port = GPIO_PF,
             .port_num = 1,
             .func_select = 0x0002,
             .drv_level = 0x0002,
             .pull_state = 0x0002,
             .data = 0,
             .data_read = 0},
            {.port = GPIO_PF,
             .port_num = 2,
             .func_select = 0x0002,
             .drv_level = 0x0002,
             .pull_state = 0x0002,
             .data = 0,
             .data_read = 0},
            {.port = GPIO_PF,
             .port_num = 3,
             .func_select = 0x0002,
             .drv_level = 0x0002,
             .pull_state = 0x0002,
             .data = 0,
             .data_read = 0},
            {.port = GPIO_PF,
             .port_num = 4,
             .func_select = 0x0002,
             .drv_level = 0x0002,
             .pull_state = 0x0002,
             .data = 0,
             .data_read = 0}};
        gpio_config(smhc0_gpio_cfg, 5);
        smhcn_clk_init(SMHC0);
        sd_mmc_fifo_reset();
        sd_mmc_controller_reset();
        sd_mmc_int_mask(0xFFFFFFFF);
        sd_mmc_int_enable(SD_CARD_INT_ENABLE);

        sd_mmc_set_bus_width(1);
        sd_card_dev_clk_set();

        // Send CMD0 first.
        sd_cmd = phrase_sd_mmc_cmd(SD_MMC_CMD0_GO_IDLE_STATE);
        sd_mmc_send_cmd(&sd_cmd);

        // Send CMD55 to define to the card that the next command is an application specific command.
    }
}

void sd_mmc_int_enable(int enable)
{
    soc_reg_t reg_val = smhc0_reg_list->SMHC_CTRL;
    if (enable == SD_CARD_INT_ENABLE)
    {
        reg_val |= INT_ENB_BIT;
    }
    else
    {
        reg_val &= (~INT_ENB_BIT);
    }
    smhc0_reg_list->SMHC_CTRL = reg_val;
}

void sd_mmc_int_mask(soc_reg_t mask)
{
    smhc0_reg_list->SMHC_INTMASK = mask;
}

void sd_card_dev_clk_set()
{
    smhc0_reg_list->SMHC_CLKDIV |= CCLK_ENB_BIT;
    sd_cmd = phrase_sd_mmc_cmd(SD_MMC_DEVICE_CLK_RESET_CMD);
    sd_mmc_send_cmd(&sd_cmd);
}

void sd_mmc_fifo_reset()
{
    smhc0_reg_list->SMHC_CTRL |= FIFO_RST_BIT;
    msleep(10);
    // wait for fifo reset complete.
    while ((smhc0_reg_list->SMHC_CTRL) & FIFO_RST_BIT)
    {
        msleep(10);
    };
}

void sd_mmc_controller_reset()
{
    smhc0_reg_list = get_smhc_reg_list(SMHCn_BASE_ADDR(SMHC0));
    smhc0_reg_list->SMHC_CTRL |= SOFT_RST_BIT;
    smhc0_reg_list->SMHC_CTRL |= (1 << 31);
    msleep(10);
    // wait for controller reset complete.
    int limit = 30;
    while ((smhc0_reg_list->SMHC_CTRL) & FIFO_RST_BIT)
    {
        msleep(10);
        limit--;
        if (limit <= 0)
            break;
    }
    if (limit <= 0)
    {
        panic("in sd_mmc_controller_reset, command excute timeout\n");
    }
}

void sd_mmc_set_bus_width(int bus_width)
{
    smhc0_reg_list = get_smhc_reg_list(SMHCn_BASE_ADDR(SMHC0));
    if (bus_width == 8)
    {
        smhc0_reg_list->SMHC_CTYPE = 2;
    }
    else if (bus_width == 4)
    {
        smhc0_reg_list->SMHC_CTYPE = 1;
    }
    else
    {
        smhc0_reg_list->SMHC_CTYPE = 0;
    }
}

SMHC_REG_LIST *get_smhc_reg_list(soc_reg_t smhc_reg_base_addr)
{
    SMHC_REG_LIST *smhc_reg_list = (SMHC_REG_LIST *)((long)smhc_reg_base_addr);
    return smhc_reg_list;
}

SD_CMD phrase_sd_mmc_cmd(int cmd_type)
{
    SD_CMD sd_cmd;
    switch (cmd_type)
    {
    case SD_MMC_DEVICE_CLK_RESET_CMD:
    {
        sd_cmd.cmd_index = NULL;
        sd_cmd.with_resp = SD_MMC_CMD_WITHOUT_RSP;
        sd_cmd.resp_type = SD_MMC_CMD_WITHOUT_SHORT_RSP;
        sd_cmd.check_resp_crc = SD_MMC_CMD_NO_CHECK_RSP_CRC;
        sd_cmd.with_data_trans = SD_MMC_CMD_WITHOUT_DATA_TRANS;
        sd_cmd.trans_direction = SD_MMC_TRANS_DIR_READ;
        sd_cmd.trans_mode = SD_MMC_TRANS_MODE_BLOCK;
        sd_cmd.stop_cmd_flag = SD_MMC_NO_STOP_CMD_FLAG;
        sd_cmd.wait_pre_over = SD_MMC_WAIT_PRE_OVER;
        sd_cmd.stop_abt_cmd = SD_MMC_NORMAL_CMD;
        sd_cmd.send_init_seq = SD_MMC_NORMAL_CMD;
        sd_cmd.prg_clk = SD_MMC_PRG_CLK;
        sd_cmd.boot_mod = SD_MMC_NORMAL_CMD;
        sd_cmd.exp_boot_ack = SD_MMC_NORMAL_CMD;
        sd_cmd.boot_abt = SD_MMC_NORMAL_CMD;
        sd_cmd.vol_sw = SD_MMC_NORMAL_CMD;
        sd_cmd.arg = NULL;
        strcpy(sd_cmd.cmd_type, "bc");
        break;
    }
    case SD_MMC_CMD0_GO_IDLE_STATE:
    {
        sd_cmd.cmd_index = SD_MMC_CMD0_GO_IDLE_STATE;
        sd_cmd.with_resp = SD_MMC_CMD_WITHOUT_RSP;
        sd_cmd.resp_type = SD_MMC_CMD_WITHOUT_SHORT_RSP;
        sd_cmd.check_resp_crc = SD_MMC_CMD_NO_CHECK_RSP_CRC;
        sd_cmd.with_data_trans = SD_MMC_CMD_WITHOUT_DATA_TRANS;
        sd_cmd.trans_direction = SD_MMC_TRANS_DIR_READ;
        sd_cmd.trans_mode = SD_MMC_TRANS_MODE_BLOCK;
        sd_cmd.stop_cmd_flag = SD_MMC_NO_STOP_CMD_FLAG;
        sd_cmd.wait_pre_over = SD_MMC_WAIT_PRE_OVER;
        sd_cmd.stop_abt_cmd = SD_MMC_NORMAL_CMD;
        sd_cmd.send_init_seq = SD_MMC_NORMAL_CMD;
        sd_cmd.prg_clk = SD_MMC_NORMAL_CMD;
        sd_cmd.boot_mod = SD_MMC_NORMAL_CMD;
        sd_cmd.exp_boot_ack = SD_MMC_NORMAL_CMD;
        sd_cmd.boot_abt = SD_MMC_NORMAL_CMD;
        sd_cmd.vol_sw = SD_MMC_NORMAL_CMD;
        sd_cmd.arg = NULL;
        strcpy(sd_cmd.cmd_type, "bc");
        break;
    }

    default:
        break;
    }
    return sd_cmd;
}

void sd_mmc_send_cmd(SD_CMD *cmd)
{
    smhc0_reg_list = get_smhc_reg_list(SMHCn_BASE_ADDR(SMHC0));
    smhc0_reg_list->SMHC_CMD = 0;               // Clear register SMHC_CMD first.
    smhc0_reg_list->SMHC_CMD |= cmd->cmd_index; // Set command index.
    smhc0_reg_list->SMHC_CMDARG = cmd->arg;     // Set command arguments.
    if (cmd->with_resp)
    {
        smhc0_reg_list->SMHC_CMD |= RESP_RCV;
    }
    if (cmd->resp_type)
    {
        smhc0_reg_list->SMHC_CMD |= LONG_RESP;
    }
    if (cmd->check_resp_crc)
    {
        smhc0_reg_list->SMHC_CMD |= CHK_RESP_CRC;
    }
    if (cmd->with_data_trans)
    {
        smhc0_reg_list->SMHC_CMD |= DATA_TRANS;
    }
    if (cmd->trans_direction)
    {
        smhc0_reg_list->SMHC_CMD |= TRANS_DIR_WRITE;
    }
    if (cmd->trans_mode)
    {
        smhc0_reg_list->SMHC_CMD |= TRANS_MODE_STREAM;
    }
    if (cmd->stop_cmd_flag)
    {
        smhc0_reg_list->SMHC_CMD |= STOP_CMD_FLAG;
    }
    if (cmd->wait_pre_over)
    {
        smhc0_reg_list->SMHC_CMD |= WAIT_PRE_OVER;
    }
    if (cmd->stop_abt_cmd)
    {
        smhc0_reg_list->SMHC_CMD |= STOP_ABT_CMD;
    }
    if (cmd->send_init_seq)
    {
        smhc0_reg_list->SMHC_CMD |= SEND_INIT_SEQ;
    }
    if (cmd->prg_clk)
    {
        smhc0_reg_list->SMHC_CMD |= PRG_CLK;
    }
    if (cmd->boot_mod == 0x01)
    {
        smhc0_reg_list->SMHC_CMD |= BOOT_MODE_MANDATORY;
    }
    else if (cmd->boot_mod == 0x02)
    {
        smhc0_reg_list->SMHC_CMD |= BOOT_MODE_ALTERNATE;
    }
    if (cmd->exp_boot_ack)
    {
        smhc0_reg_list->SMHC_CMD |= EXP_BOOT_ACK;
    }
    if (cmd->boot_abt)
    {
        smhc0_reg_list->SMHC_CMD |= BOOT_ABT;
    }
    if (cmd->vol_sw)
    {
        smhc0_reg_list->SMHC_CMD |= VOL_SW;
    }

    smhc0_reg_list->SMHC_CMD |= CMD_LOAD;

    msleep(10);
    int limit = 30;
    while ((smhc0_reg_list->SMHC_CMD) & (1 << 31))
    {
        msleep(10);
        limit--;
        if (limit <= 0)
            break;
    }
    if (limit <= 0)
    {
        panic("in sd_mmc_send_cmd, command send timeout\n");
    }
}