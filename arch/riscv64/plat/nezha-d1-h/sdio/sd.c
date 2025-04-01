#include "sd.h"

int mmcsd_app_cmd(MMCSD_HOST *host, MMCSD_CARD *card)
{
    int err = ERR_SUCCESS;
    SDIO_CMD cmd = {};
    cmd.cmd_code = MMC_CMD55_APP_CMD;
    if (card)
    {
        cmd.arg = card->rca << 16;
        cmd.resp_type = SD_MMC_WITH_RESP;
    }
    else
    {
        cmd.arg = 0;
        cmd.resp_type = SD_MMC_WITH_RESP;
    }
    err = sdio_transfer(host->smhc_no, &cmd, NULL);
    if (!(cmd.resp[0] & R1_APP_CMD))
        err = ERR_GENERAL;
    return err;
}

int mmcsd_send_app_cmd(MMCSD_HOST *host, MMCSD_CARD *card, SDIO_CMD *cmd)
{
    int err = ERR_SUCCESS;
    int retry = 10;
    for (int i = 0; i < retry; ++i)
    {
        err = mmcsd_app_cmd(host, card);
        if (err)
        {
            continue;
        }
        err = sdio_transfer(host->smhc_no, cmd, NULL);
        if (!err)
            break;
    }
    return err;
}

int mmcsd_send_app_op_cond(MMCSD_HOST *host, uint32 ocr, uint32 *resp_ocr)
{
    SDIO_CMD cmd = {};
    uint32 err = ERR_SUCCESS;
    cmd.cmd_code = SD_CMD41_APP_SEND_OP_COND;
    cmd.arg = ocr;
    cmd.resp_type = SD_MMC_RESP_CRC;
    for (int i = 0; i < 100; ++i)
    {
        err = mmcsd_send_app_cmd(host, NULL, &cmd);
        if (err)
            break;
        if (ocr == 0)
            break;
        if (cmd.resp[0] & CARD_BUSY)
            break;
        err = ERR_TIMEOUT;
        msleep(10);
    }
    if(resp_ocr)
    {
        *resp_ocr = cmd.resp[0];
    }
    return err;
}

int mmcsd_go_idle(int smhc_no)
{
    uint32 err;
    SDIO_CMD cmd = {};
    cmd.cmd_code = SD_MMC_CMD0_GO_IDLE_STATE;
    cmd.arg = 0;
    cmd.resp_type = SD_MMC_RESP_NONE;
    err = sdio_transfer(smhc_no, &cmd, NULL);
    msleep(1);
    return err;
}

void mmcsd_power_up(MMCSD_HOST *host)
{

}

void init_sd_card(MMCSD_HOST *host, uint32 ocr)
{
    if(host->card == NULL)
    {

    }
}

void smhcn_init(int n)
{
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(SMHC0);
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
        sdio_update_clock(SMHC0);
        sd_mmc_fifo_reset();
        sd_mmc_controller_reset();
        sd_mmc_int_mask(0xFFFFFFFF);
        sd_mmc_int_enable(SD_CARD_INT_ENABLE);

        sdio_set_bus_width(SMHC0, 4);
        // mmcsd_go_idle(SMHC0);
        uint32 buff_w[1024], buff_r[1024] = {};
        buff_w[0] = 0x01;
        buff_w[1] = 0x02;
        int err = 0;
        err = sdio_write_data(SMHC0, buff_w, 1, 512);
        printf("write err: %d\n\r", err);
        err = sdio_read_data(SMHC0, buff_r, 1, 512);
        printf("read err: %d\n\r", err);
    }
}

void sd_mmc_int_enable(int enable)
{
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(SMHC0);
    soc_reg_t reg_val = smhc_reg_list->SMHC_CTRL;
    if (enable == SD_CARD_INT_ENABLE)
    {
        reg_val |= SMHC_CTRL_INT_ENB;
    }
    else
    {
        reg_val &= (~SMHC_CTRL_INT_ENB);
    }
    smhc_reg_list->SMHC_CTRL = reg_val;
}

void sd_mmc_int_mask(soc_reg_t mask)
{
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(SMHC0);
    smhc_reg_list->SMHC_INTMASK = mask;
}

void sd_mmc_fifo_reset()
{
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(SMHC0);
    smhc_reg_list->SMHC_CTRL |= SMHC_CTRL_FIFO_RST;
    msleep(10);
    // wait for fifo reset complete.
    while ((smhc_reg_list->SMHC_CTRL) & SMHC_CTRL_FIFO_RST)
    {
        msleep(10);
    };
}

void sd_mmc_controller_reset()
{
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(SMHC0);
    smhc_reg_list->SMHC_CTRL |= SMHC_CTRL_SOFT_RST;
    smhc_reg_list->SMHC_CTRL |= (1 << 31);
    msleep(10);
    // wait for controller reset complete.
    int limit = 30;
    while ((smhc_reg_list->SMHC_CTRL) & SMHC_CTRL_SOFT_RST)
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
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(SMHC0);
    if (bus_width == 8)
    {
        smhc_reg_list->SMHC_CTYPE = 2;
    }
    else if (bus_width == 4)
    {
        smhc_reg_list->SMHC_CTYPE = 1;
    }
    else
    {
        smhc_reg_list->SMHC_CTYPE = 0;
    }
}