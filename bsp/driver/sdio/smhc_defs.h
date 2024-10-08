#ifndef __SMHC_DEFS_H__
#define __SMHC_DEFS_H__

#include "arch/riscv/include/riscv_type_defs.h"

#define SMHC0 (0)
#define SMHC1 (1)

#define SMHCn_BASE_ADDR(n) (0x04020000 + (n * 0x1000))
#define SMHC_CTRL_OFFSET (0x0000)         // Control Register
#define SMHC_CLKDIV_OFFSET (0x0004)       // Clock Control Register
#define SMHC_TMOUT_OFFSET (0x0008)        // Time Out Register
#define SMHC_CTYPE_OFFSET (0x000C)        // Bus Width Register
#define SMHC_BLKSIZ_OFFSET (0x0010)       // Block Size Register
#define SMHC_BYTCNT_OFFSET (0x0014)       // Byte Count Register
#define SMHC_CMD_OFFSET (0x0018)          // Command Register
#define SMHC_CMDARG_OFFSET (0x001C)       // Command Argument Register
#define SMHC_RESP0_OFFSET (0x0020)        // Response 0 Register
#define SMHC_RESP1_OFFSET (0x0024)        // Response 1 Register
#define SMHC_RESP2_OFFSET (0x0028)        // Response 2 Register
#define SMHC_RESP3_OFFSET (0x002C)        // Response 3 Register
#define SMHC_INTMASK_OFFSET (0x0030)      // Interrupt Mask Register
#define SMHC_MINTSTS_OFFSET (0x0034)      // Masked Interrupt Status Register
#define SMHC_RINTSTS_OFFSET (0x0038)      // Raw Interrupt Status Register
#define SMHC_STATUS_OFFSET (0x003C)       // Status Register
#define SMHC_FIFOTH_OFFSET (0x0040)       // FIFO Water Level Register
#define SMHC_FUNS_OFFSET (0x0044)         // FIFO Function Select Register
#define SMHC_TCBCNT_OFFSET (0x0048)       // Transferred Byte Count between Controller and Card
#define SMHC_TBBCNT_OFFSET (0x004C)       // Transferred Byte Count between Host Memory and Internal FIFO
#define SMHC_DBGC_OFFSET (0x0050)         // Current Debug Control Register
#define SMHC_CSDC_OFFSET (0x0054)         // CRC Status Detect Control Register
#define SMHC_A12A_OFFSET (0x0058)         // Auto Command 12 Argument Register
#define SMHC_NTSR_OFFSET (0x005C)         // SD New Timing Set Register
#define SMHC_HWRST_OFFSET (0x0078)        // Hardware Reset Register
#define SMHC_IDMAC_OFFSET (0x0080)        // IDMAC Control Register
#define SMHC_DLBA_OFFSET (0x0084)         // Discriptor List Base Address Register
#define SMHC_IDST_OFFSET (0x0088)         // IDMAC Status Register
#define SMHC_IDIE_OFFSET (0x008C)         // IDMAC Interrupt Enable Register
#define SMHC_THLD_OFFSET (0x0100)         // Card Threshold Control Register
#define SMHC_SFC_OFFSET (0x0104)          // Sample FIFO Control Register
#define SMHC_A23A_OFFSET (0x0108)         // Auto Command 23 Argument Register
#define SMHC_DDR_SBIT_DET_OFFSET (0x010C) // eMMC4.5 DDR Start Bit Detection Control Register
#define SMHC_EXT_CMD_OFFSET (0x0138)      // Extended Command Register
#define SMHC_EXT_RESP_OFFSET (0x013C)     // Extended Response Register
#define SMHC_DRV_DL_OFFSET (0x0140)       // Drive Delay Control Register
#define SMHC_SMAP_DL_OFFSET (0x0144)      // Sample Delay Control Register
#define SMHC_DS_DL_OFFSET (0x0148)        // Data Strobe Delay Control Register
#define SMHC_HS400_DL_OFFSET (0x014C)     // HS400 Delay Control Register
#define SMHC_FIFO_OFFSET (0x0200)         // Read/Write FIFO

// SMHC_CTRL Defines:
#define SMHC_CTRL_SOFT_RST (1 << 0)
#define SMHC_CTRL_FIFO_RST (1 << 1)
#define SMHC_CTRL_DMA_RST (1 << 2)
#define SMHC_CTRL_HW_RST (SMHC_CTRL_SOFT_RST | SMHC_CTRL_FIFO_RST | SMHC_CTRL_DMA_RST)
#define SMHC_CTRL_INT_ENB (1 << 4)
#define SMHC_CTRL_DMA_ENB (1 << 5)
#define SMHC_CTRL_CD_DBC_ENB (1 << 8)
#define SMHC_CTRL_DDR_MOD_SEL (1 << 10)
#define SMHC_CTRL_TIME_UNIT_DAT (1 << 11)
#define SMHC_CTRL_FIFO_AC_MOD_AHB (1 << 31)

// SMHC_CLKDIV bits:
#define SMHC_CLK_CCLK_ENB (1 << 16)

// SMHC_CMD bits:
#define SMHC_CMD_LOAD (1 << 31)                    // Send current command. Automatically cleared when the current command is sent.
#define SMHC_CMD_VOL_SW (1 << 28)                  // Voltage switch command, set for CMD11 only.
#define SMHC_CMD_BOOT_ABT (1 << 27)                // Setting this bit will terminate the boot operation.
#define SMHC_CMD_EXP_BOOT_ACK (1 << 26)            // When the software sets this bit along in mandatory boot operation, the controller expects a boot acknowledge start pattern of 0-1-0 from the selected card.
#define SMHC_CMD_BOOT_MODE_MANDATORY (1 << 24)     // Set BOOT_MODE bit to Mandatory Boot Operation.
#define SMHC_CMD_BOOT_MODE_ALTERNATE (2 << 24)     // Set BOOT_MODE bit to Alternate Boot Operation.
#define SMHC_CMD_PRG_CLK (1 << 21)                 // When this bit is set, the controller will change the clock domain and clock output. No commands will be sent.
#define SMHC_CMD_SEND_INIT_SEQ (1 << 15)           // Send initialization sequence before sending this command.
#define SMHC_CMD_STOP_ABT_CMD (1 << 14)            // Send Stop or Abort command to stop the current data transfer in progress. (CMD12, CMD52 for writing "I/O Abort" in SDIO_CCCR).
#define SMHC_CMD_WAIT_PRE_OVER (1 << 13)           // Wait for data transfer completion before sending the current command.
#define SMHC_CMD_AUTO_SEND_CMD_STOP_FLAG (1 << 12) // Send stop command automatically at the end of the data transfer. If set, the SMHC_RESP1 will record the response of auto CMD12.
#define SMHC_CMD_TRANS_MODE_STREAM (1 << 11)       // Stream data transfer command. If not set, the default value 0 is Block data transfer command.
#define SMHC_CMD_TRANS_DIR_READ (0 << 10)          // Set the data transfer direction to Read.
#define SMHC_CMD_TRANS_DIR_WRITE (1 << 10)         // Set the data transfer direction to Write.
#define SMHC_CMD_WITH_DATA_TRANS (1 << 9)          // With data transfer.
#define SMHC_CMD_CHK_RESP_CRC (1 << 8)             // Check response CRC.
#define SMHC_CMD_LONG_RESP (1 << 7)                // Long response type(136 bits). If not set, means short response type(48 bits).
#define SMHC_CMD_RCV_RESP (1 << 6)                 // Command with response. If not set, means command without response.

// CMD defines.
#define SD_MMC_CMD0_GO_IDLE_STATE (0)
#define MMC_CMD1_SEND_OP_COND (1)
#define MMC_CMD2_ALL_SEND_CID (2)
#define MMC_CMD3_SET_RELATIVE_ADDR (3)
#define MMC_CMD4_SET_DSR (4)
#define MMC_CMD6_SWITCH (6)
#define MMC_CMD7_SELECT_CARD (7)
#define MMC_CMD8_SEND_EXT_CSD (8)
#define MMC_CMD9_SEND_CSD (9)
#define MMC_CMD10_SEND_CID (10)
#define MMC_CMD12_STOP_TRANSMISSION (12)
#define MMC_CMD13_SEND_STATUS (13)
#define MMC_CMD16_SET_BLOCKLEN (16)
#define MMC_CMD17_READ_SINGLE_BLOCK (17)
#define MMC_CMD18_READ_MULTIPLE_BLOCK (18)
#define MMC_CMD24_WRITE_SINGLE_BLOCK (24)
#define MMC_CMD25_WRITE_MULTIPLE_BLOCK (25)
#define MMC_CMD35_ERASE_GROUP_START (35)
#define MMC_CMD36_ERASE_GROUP_END (36)
#define MMC_CMD38_ERASE (38)
#define MMC_CMD55_APP_CMD (55)
#define MMC_CMD58_SPI_READ_OCR (58)
#define MMC_CMD59_SPI_CRC_ON_OFF (59)

#define SD_CMD3_SEND_RELATIVE_ADDR (3)
#define SD_CMD6_SWITCH_FUNC (6)
#define SD_CMD8_SEND_IF_COND (8)

#define SD_CMD6_APP_SET_BUS_WIDTH (6)
#define SD_CMD32_ERASE_WR_BLK_START (32)
#define SD_CMD33_ERASE_WR_BLK_END (33)
#define SD_CMD41_APP_SEND_OP_COND (41)
#define SD_CMD51_APP_SEND_SCR (51)

#define SD_IO_SEND_OP_COND (5)
#define SD_IO_RW_DIRECT (52)
#define SD_IO_RW_EXTENDED (53)

#define SD_MMC_CMD_CHECK_PATTERN (0xaa)

// Card Status Defines.
#define SMHC_STATUS_FSM_BUSY (1 << 10)             // Data transmit or receive state-machine is busy.
#define SMHC_STATUS_CARD_DATA_BUSY (1 << 9)        // Card data is busy.
#define SMHC_STATUS_CARD_PRESENT (1 << 8)          // The card is present.
#define SMHC_STATUS_FIFO_FULL (1 << 3)             // FIFO is full.
#define SMHC_STATUS_FIFO_EMPTY (1 << 2)            // FIFO is empty.
#define SMHC_STATUS_FIFO_TX_LEVEL_REACHED (1 << 1) // FIFO reaches the transmit trigger level.
#define SMHC_STATUS_FIFO_RX_LEVEL_REACHED (1 << 0) // FIFO reaches the receive trigger level.
#define SMHC_STATUS_FSM_STA_MASK (0xF << 4)        // FSM_STA [4:7].
#define SMHC_STATUS_FSM_STA_IDLE (0)               // FSM_STA: IDLE - 0000.
#define SMHC_STATUS_FSM_STA_SEND_INIT_SEQ (1)      // FSM_STA: Send init sequence - 0001.

// Card Raw Interrupt Defines.
#define SMHC_RINTSTS_CARD_REMOVAL (1 << 31) // Card removed.
#define SMHC_RINTSTS_CARD_INSERT (1 << 30)  // Card inserted.
#define SMHC_RINTSTS_SDIOI_INT (1 << 16)    // SDIO interrupt.
#define SMHC_RINTSTS_DEE (1 << 15)          // Data End-bit Error. When the bit is set during receiving data, it means that the host controller does not receive the valid data end bit. When the bit is set during transmitting data, it means that the host controller does not receive the CRC status token.
#define SMHC_RINTSTS_ACD (1 << 14)          // Auto Command Done. When set, it means tat the auto-stop command (CMD12) is completed.
#define SMHC_RINTSTS_DSE_BC (1 << 13)       // Data Start Error/Busy Clear. When the bit is set during receiving data,it means that the host controller found an error start bit. When the bit is set during transmitting data, it means that the busy signal is cleared after the last block.
#define SMHC_RINTSTS_CB_IW (1 << 12)        // Command Busy and Illegal Write.
#define SMHC_RINTSTS_FU_FO (1 << 11)        // FIFO Underrun/Overflow.
#define SMHC_RINTSTS_DSTO_VSD (1 << 10)     // Data Starvation Timeout/V1.8 Switch Done.
#define SMHC_RINTSTS_DTO_BDS (1 << 9)       // Data Timeout/Boot Data Start. When the bit is set during receiving data, it means that some of the channel of DATA[3:0] lack of the start bit.
#define SMHC_RINTSTS_RTO_BACK (1 << 8)      // Response Timeout/Boot ACK Received.
#define SMHC_RINTSTS_DCE (1 << 7)           // Data CRC Error.
#define SMHC_RINTSTS_RCE (1 << 6)           // Response CRC Error.
#define SMHC_RINTSTS_DRR (1 << 5)           // Data Receive Request. When set, it means that there are enough data in FIFO during receiving data.
#define SMHC_RINTSTS_DTR (1 << 4)           // Data Transmit Request. When set, it means that there are enough space in FIFO during transmitting data.
#define SMHC_RINTSTS_DTC (1 << 3)           // Data Transfer Complete. When set, it means that the current command completes even though error occurs.
#define SMHC_RINTSTS_CC (1 << 2)            // Command Complete. When set, it means that the current command completes even though error occurs.
#define SMHC_RINTSTS_RE (1 << 1)            // Response Error. When set, it means that the transmit bit error, end bit error, or CMD index error may occur.
#define SMHC_RINTSTS_ERRORS (SMHC_RINTSTS_RE | SMHC_RINTSTS_RCE | SMHC_RINTSTS_DCE | SMHC_RINTSTS_RTO_BACK | SMHC_RINTSTS_DTO_BDS | SMHC_RINTSTS_FU_FO | SMHC_RINTSTS_CB_IW | SMHC_RINTSTS_DSE_BC | SMHC_RINTSTS_DEE)
#define SMHC_RINTSTS_DONE (SMHC_RINTSTS_ACD | SMHC_RINTSTS_DTC | SMHC_RINTSTS_CC | SMHC_RINTSTS_DSTO_VSD)
// Error Defines.
#define ERR_SUCCESS (0)
#define ERR_TIMEOUT (1)
#define ERR_GENERAL (2)
#define ERR_READ_DATA (3)
#define ERR_WRITE_DATA (4)

// Response Defines.
#define SD_MMC_RESP_NONE (0)
#define SD_MMC_WITH_RESP (1 << 0)
#define SD_MMC_RESP_LONG (1 << 1)
#define SD_MMC_RESP_CRC (1 << 2)
#define SD_MMC_RESP_BUSY (1 << 3)
#define SD_MMC_RESP_OPCODE (1 << 4)

// Data Direction Defines.
#define SD_MMC_DATA_READ (1 << 0)
#define SD_MMC_DATA_WRITE (1 << 1)

// Card Bus Width Defines.
#define SD_MMC_BUS_WIDTH_1 (0)
#define SD_MMC_BUS_WIDTH_4 (1)
#define SD_MMC_BUS_WIDTH_8 (2)

typedef struct _SMHC_REG_LIST
{
    volatile uint32 SMHC_CTRL;         // 0x0000 Control Register.
    volatile uint32 SMHC_CLKDIV;       // 0x0004 Clock Control Register.
    volatile uint32 SMHC_TMOUT;        // 0x0008 Time Out Register.
    volatile uint32 SMHC_CTYPE;        // 0x000C Bus Width Register.
    volatile uint32 SMHC_BLKSIZ;       // 0x0010 Block Size Register.
    volatile uint32 SMHC_BYTCNT;       // 0x0014 Byte Count Register.
    volatile uint32 SMHC_CMD;          // 0x0018 Command Register.
    volatile uint32 SMHC_CMDARG;       // 0x001C Command Argument Register.
    volatile uint32 SMHC_RESP0;        // 0x0020 Response 0 Register.
    volatile uint32 SMHC_RESP1;        // 0x0024 Response 1 Register.
    volatile uint32 SMHC_RESP2;        // 0x0028 Response 2 Register.
    volatile uint32 SMHC_RESP3;        // 0x002C Response 3 Register.
    volatile uint32 SMHC_INTMASK;      // 0x0030 Interrupt Mask Register.
    volatile uint32 SMHC_MINTSTS;      // 0x0034 Masked Interrupt Status Register.
    volatile uint32 SMHC_RINTSTS;      // 0x0038 Raw Interrupt Status Register.
    volatile uint32 SMHC_STATUS;       // 0x003C Status Register.
    volatile uint32 SMHC_FIFOTH;       // 0x0040 FIFO Water Level Register.
    volatile uint32 SMHC_FUNS;         // 0x0044 FIFO Function Select Register.
    volatile uint32 SMHC_TCBCNT;       // 0x0048 Transferred Byte Count between Controller and Card.
    volatile uint32 SMHC_TBBCNT;       // 0x004C Transferred Block Count between Host Memory and Internal FIFO.
    volatile uint32 SMHC_DBGC;         // 0x0050 Current Debug Control Register.
    volatile uint32 SMHC_CSDC;         // 0x0054 CRC Status Detect Control Registers.
    volatile uint32 SMHC_A12A;         // 0x0058 Auto Command 12 Argument Register.
    volatile uint32 SMHC_NTSR;         // 0x005C SD New Timing Set Register.
    volatile uint32 reserved0[6];      // 0x0060 - 0x0078 Reserved.
    volatile uint32 SMHC_HWRST;        // 0x0078 Hardware Reset Register.
    volatile uint32 reserved1;         // 0x007C Reserved.
    volatile uint32 SMHC_IDMAC;        // 0x0080 IDMAC Control Register.
    volatile uint32 SMHC_DLBA;         // 0x0084 Descriptor List Base Address Register.
    volatile uint32 SNHC_IDST;         // 0x0088 IDMAC Status Register.
    volatile uint32 SMHC_IDIE;         // 0x008C IDMAC Interrupt Enable Register.
    volatile uint32 reserved2[28];     // 0x0090 - 0x00100 Reserved.
    volatile uint32 SMHC_THLD;         // 0x0100 Card Threshold Control Register.
    volatile uint32 SMHC_SFC;          // 0x0104 Sample FIFO Control Register.
    volatile uint32 SMHC_A23A;         // 0x0108 Auto Command 23 Argument Register.
    volatile uint32 EMMC_DDR_SBIT_DET; // 0x010C eMMC4.5 DDR Start Bit Detection Control Register.
    volatile uint32 reserved3[10];     // 0x0110 - 0x0138 Reserved.
    volatile uint32 SMHC_EXT_CMD;      // 0x0138 Extended Command Register.
    volatile uint32 SMHC_EXT_RESP;     // 0x013C Extended Response Register.
    volatile uint32 SMHC_DRV_DL;       // 0x0140 Drive Delay Control Register.
    volatile uint32 SMHC_SMAP_DL;      // 0x0144 Sample Delay Control Register.
    volatile uint32 SMHC_DS_DL;        // 0x0148 Data Strobe Delay Control Register.
    volatile uint32 SMHC_HS400_DL;     // 0x014C HS400 Delay Control Register.
    volatile uint32 reserved4[44];     // 0x0150 - 0x0200 Reserved.
    volatile uint32 SMHC_FIFO;         // 0x0200 Read/Write FIFO.
} SMHC_REG_LIST;

typedef struct _SDIO_CMD
{
    uint32 cmd_code;
    uint32 arg;
    uint32 resp_type;
    uint32 resp[4];
} SDIO_CMD;

typedef struct _SDIO_DATA
{
    uint8 *data_buf;
    uint32 op_flag;
    uint32 blk_size;
    uint32 blk_count;
} SDIO_DATA;

#endif /* __SMHC_DEFS_H__ */