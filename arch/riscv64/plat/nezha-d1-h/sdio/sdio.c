#include "sdio.h"

SMHC_REG_LIST *get_smhc_reg_list(int smhc_no)
{
    SMHC_REG_LIST *smhc_reg_list = (SMHC_REG_LIST *)SMHCn_BASE_ADDR(smhc_no);
    return smhc_reg_list;
}

int sdio_init(int smhc_no, MMCSD_HOST *host)
{
    host->smhc_no = smhc_no;
    host->bus_width = SD_MMC_BUS_WIDTH_4;
    host->clock = 1000 * 1000 * 1000;
    host->freq_min = 400000;
    host->freq_max = 50000000;
    host->cur_valid_ocr = MMC_VDD_32_33 | MMC_VDD_33_34;
    // host->dev_cap_flags =
    return ERR_SUCCESS;
}

int sdio_transfer_cmd(int smhc_no, SDIO_CMD *cmd, SDIO_DATA *data)
{
    uint32 cmd_val = SMHC_CMD_LOAD;
    volatile uint32 status = 0;
    uint32 duration = 0;
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(smhc_no);
    if (cmd->cmd_code == MMC_CMD12_STOP_TRANSMISSION)
    {
        start_timing();
        do
        {
            status = smhc_reg_list->SMHC_STATUS;
            smhc_reg_list->SMHC_CTRL = SMHC_CTRL_HW_RST; // Reset hardwares.
            smhc_reg_list->SMHC_RINTSTS = 0xFFFFFFFF;    // Clear all interrupts.
            timing_duration(&duration);
            if (duration > 100)
                return ERR_TIMEOUT;
        } while (status & SMHC_STATUS_CARD_DATA_BUSY);
        return ERR_SUCCESS;
    }
    if (cmd->cmd_code == SD_MMC_CMD0_GO_IDLE_STATE)
    {
        cmd_val |= SMHC_CMD_SEND_INIT_SEQ;
    }
    if (cmd->resp_type & SD_MMC_WITH_RESP)
    {
        cmd_val |= SMHC_CMD_RCV_RESP;
        if (cmd->resp_type & SD_MMC_RESP_LONG)
        {
            cmd_val |= SMHC_CMD_LONG_RESP;
        }
        if (cmd->resp_type & SD_MMC_RESP_CRC)
        {
            cmd_val |= SMHC_CMD_CHK_RESP_CRC;
        }
    }
    if (data)
    {
        cmd_val |= SMHC_CMD_WITH_DATA_TRANS | SMHC_CMD_WAIT_PRE_OVER;
        if (data->op_flag & SD_MMC_DATA_WRITE)
        {
            cmd_val |= SMHC_CMD_TRANS_DIR_WRITE;
        }
    }
    if (cmd->cmd_code == MMC_CMD25_WRITE_MULTIPLE_BLOCK || cmd->cmd_code == MMC_CMD18_READ_MULTIPLE_BLOCK)
    {
        cmd_val |= SMHC_CMD_AUTO_SEND_CMD_STOP_FLAG;
    }
    smhc_reg_list->SMHC_CMDARG = cmd->arg;
    if (data)
    {
        smhc_reg_list->SMHC_CTRL = smhc_reg_list->SMHC_CTRL | SMHC_CTRL_FIFO_AC_MOD_AHB;
    }
    smhc_reg_list->SMHC_CMD = cmd_val | cmd->cmd_code;

    start_timing();
    do
    {
        status = smhc_reg_list->SMHC_RINTSTS;
        timing_duration(&duration);
        if ((status & SMHC_RINTSTS_ERRORS) || (duration > 100))
        {
            smhc_reg_list->SMHC_CTRL = SMHC_CTRL_HW_RST; // Reset hardwares.
            smhc_reg_list->SMHC_RINTSTS = 0xFFFFFFFF;    // Clear all interrupts.
            return ERR_GENERAL;
        }
    } while (!(status & SMHC_RINTSTS_CC));

    if (cmd->resp_type & SD_MMC_RESP_BUSY)
    {
        start_timing();
        do
        {
            status = smhc_reg_list->SMHC_STATUS;
            timing_duration(&duration);
            if (duration > 100)
            {
                smhc_reg_list->SMHC_CTRL = SMHC_CTRL_HW_RST; // Reset hardwares.
                smhc_reg_list->SMHC_RINTSTS = 0xFFFFFFFF;    // Clear all interrupts.
                return ERR_GENERAL;
            }
        } while (status & SMHC_STATUS_CARD_DATA_BUSY);
    }
    if (cmd->resp_type & SD_MMC_RESP_LONG)
    {
        cmd->resp[0] = smhc_reg_list->SMHC_RESP3;
        cmd->resp[1] = smhc_reg_list->SMHC_RESP2;
        cmd->resp[2] = smhc_reg_list->SMHC_RESP1;
        cmd->resp[3] = smhc_reg_list->SMHC_RESP0;
    }
    else
    {
        cmd->resp[0] = smhc_reg_list->SMHC_RESP0;
    }
    smhc_reg_list->SMHC_RINTSTS = 0xFFFFFFFF; // Clear all interrupts.
    return ERR_SUCCESS;
}

int sdio_transfer_data(int smhc_no, SDIO_CMD *cmd, SDIO_DATA *data)
{
    uint32 data_len = (uint32)(data->blk_count * data->blk_size);
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(smhc_no);
    smhc_reg_list->SMHC_BLKSIZ = data->blk_size;
    smhc_reg_list->SMHC_BYTCNT = data_len;
    if (data->op_flag & SD_MMC_DATA_READ)
    {
        if (sdio_transfer_cmd(smhc_no, cmd, data) != ERR_SUCCESS)
        {
            return ERR_GENERAL;
        }
        int ret = sdio_read_data(smhc_no, data->data_buf, data->blk_count, data->blk_size);
        if (ret != ERR_SUCCESS)
            return ERR_GENERAL;
    }
    else if (data->op_flag & SD_MMC_DATA_WRITE)
    {
        if (sdio_transfer_cmd(smhc_no, cmd, data) != ERR_SUCCESS)
        {
            return ERR_GENERAL;
        }
        int ret = sdio_write_data(smhc_no, data->data_buf, data->blk_count, data->blk_size);
        if (ret != ERR_SUCCESS)
            return ERR_GENERAL;
    }
    return ERR_SUCCESS;
}

int sdio_transfer(int smhc_no, SDIO_CMD *cmd, SDIO_DATA *data)
{
    if (!data)
        sdio_transfer_cmd(smhc_no, cmd, data);
    else
        sdio_transfer_data(smhc_no, cmd, data);
}

int sdio_read_data(int smhc_no, uint32 *data_buf, uint32 blk_count, uint32 blk_size)
{
    uint64 data_bytes = blk_count * blk_size;
    uint32 *tmp_data_buf = data_buf;
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(smhc_no);
    uint32 status, err, done;
    status = smhc_reg_list->SMHC_STATUS;
    err = smhc_reg_list->SMHC_RINTSTS & SMHC_RINTSTS_ERRORS;
    while ((!err) && (data_bytes) >= sizeof(uint32)) // The reason why read 32 bit at a time is that the FIFO is 32 bit.
    {
        if (!(status & SMHC_STATUS_FIFO_EMPTY))
        {
            *(tmp_data_buf) = smhc_reg_list->SMHC_FIFO;
            tmp_data_buf++;
            data_bytes -= sizeof(uint32);
        }
        status = smhc_reg_list->SMHC_STATUS;
        err = smhc_reg_list->SMHC_RINTSTS & SMHC_RINTSTS_ERRORS;
    }
    do
    {
        err = (smhc_reg_list->SMHC_RINTSTS) & SMHC_RINTSTS_ERRORS;
        if (blk_count > 1)
            done = (smhc_reg_list->SMHC_RINTSTS) & SMHC_RINTSTS_ACD;
        else
            done = (smhc_reg_list->SMHC_RINTSTS) & SMHC_RINTSTS_DTC;
    } while (!done && !err);

    if (err & SMHC_RINTSTS_ERRORS)
    {
        return ERR_GENERAL;
    }

    smhc_reg_list->SMHC_RINTSTS = 0xFFFFFFFF; // Clear all interrupts.

    if (data_bytes)
    {
        return ERR_READ_DATA;
    }
    return ERR_SUCCESS;
}

int sdio_write_data(int smhc_no, uint32 *data_buf, uint32 blk_count, uint32 blk_size)
{
    uint64 data_bytes = blk_count * blk_size;
    uint32 *tmp_data_buf = data_buf;
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(smhc_no);
    uint32 status, err, done;
    status = smhc_reg_list->SMHC_STATUS;
    err = smhc_reg_list->SMHC_RINTSTS & SMHC_RINTSTS_ERRORS;
    while ((!err) && data_bytes)
    {
        printf("in while, status & SMHC_STATUS_FIFO_FULL: %d, databytes: %d\n\r", (status & SMHC_STATUS_FIFO_FULL), data_bytes);
        if (!(status & SMHC_STATUS_FIFO_FULL))
        {
            smhc_reg_list->SMHC_FIFO = *(tmp_data_buf);
            tmp_data_buf++;
            data_bytes -= sizeof(uint32);
        }
        status = smhc_reg_list->SMHC_STATUS;
        err = smhc_reg_list->SMHC_RINTSTS & SMHC_RINTSTS_ERRORS;
    }

    do
    {
        err = (smhc_reg_list->SMHC_RINTSTS) & SMHC_RINTSTS_ERRORS;
        if (blk_count > 1)
            done = (smhc_reg_list->SMHC_RINTSTS) & SMHC_RINTSTS_ACD;
        else
            done = (smhc_reg_list->SMHC_RINTSTS) & SMHC_RINTSTS_DTC;
    } while (!done && !err);

    if (err & SMHC_RINTSTS_ERRORS)
    {
        return ERR_GENERAL;
    }

    smhc_reg_list->SMHC_RINTSTS = 0xFFFFFFFF; // Clear all interrupts.

    if (data_bytes)
    {
        return ERR_WRITE_DATA;
    }
    return ERR_SUCCESS;
}

int sdio_update_clock(int smhc_no)
{
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(smhc_no);
    uint32 cmd_val = SMHC_CMD_LOAD | SMHC_CMD_PRG_CLK | SMHC_CMD_WAIT_PRE_OVER;
    uint32 duration;
    smhc_reg_list->SMHC_CMD = cmd_val;
    start_timing();
    do
    {
        timing_duration(&duration);
        if (duration > 100)
        {
            return ERR_GENERAL;
        }
    } while ((smhc_reg_list->SMHC_CMD) & SMHC_CMD_LOAD);
    smhc_reg_list->SMHC_RINTSTS = smhc_reg_list->SMHC_RINTSTS;
    return ERR_SUCCESS;
}

int sdio_set_clock(int smhc_no, uint32 clk)
{
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(smhc_no);
    uint32 ratio = 125;
    if ((ratio & 0xFF) != ratio)
    {
        return ERR_GENERAL;
    }
    smhc_reg_list->SMHC_CLKDIV = smhc_reg_list->SMHC_CLKDIV & (~SMHC_CLK_CCLK_ENB); // Temporarily disable clock.
    smhc_reg_list->SMHC_CLKDIV = (ratio & 0xFF);                                    // Set ratio. Source clock is divided by 2 * ratio.
    if (sdio_update_clock(smhc_no) != ERR_SUCCESS)
        return ERR_GENERAL;
    smhc_reg_list->SMHC_CLKDIV = smhc_reg_list->SMHC_CLKDIV | SMHC_CLK_CCLK_ENB; // Enable clock.
    if (sdio_update_clock(smhc_no) != ERR_SUCCESS)
        return ERR_GENERAL;
    return ERR_SUCCESS;
}

int sdio_set_bus_width(int smhc_no, uint32 bus_width)
{
    volatile SMHC_REG_LIST *smhc_reg_list = get_smhc_reg_list(smhc_no);
    switch (bus_width)
    {
    case SD_MMC_BUS_WIDTH_1:
        smhc_reg_list->SMHC_CTYPE = SD_MMC_BUS_WIDTH_1;
        break;
    case SD_MMC_BUS_WIDTH_4:
        smhc_reg_list->SMHC_CTYPE = SD_MMC_BUS_WIDTH_4;
    case SD_MMC_BUS_WIDTH_8:
        smhc_reg_list->SMHC_CTYPE = SD_MMC_BUS_WIDTH_8;
        break;
    default:
        return ERR_GENERAL;
    }
    return ERR_SUCCESS;
}