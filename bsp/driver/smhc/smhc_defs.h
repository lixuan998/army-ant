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

// SMHC_CTRL bits:
#define SOFT_RST_BIT (1 << 0)
#define FIFO_RST_BIT (1 << 1)
#define DMA_RST_BIT (1 << 2)
#define INT_ENB_BIT (1 << 4)
#define DMA_ENB_BIT (1 << 5)
#define CD_DBC_ENB_BIT (1 << 8)
#define DDR_MOD_SEL_BIT (1 << 10)
#define TIME_UNIT_DAT_BIT (1 << 11)

// SMHC_CLKDIV bits:
#define CCLK_ENB_BIT (1 << 16)

// SMHC_CMD bits:
#define CMD_LOAD (1 << 31)            // Send current command. Automatically cleared when the current command is sent.
#define VOL_SW (1 << 28)              // Voltage switch command, set for CMD11 only.
#define BOOT_ABT (1 << 27)            // Setting this bit will terminate the boot operation.
#define EXP_BOOT_ACK (1 << 26)        // When the software sets this bit along in mandatory boot operation, the controller expects a boot acknowledge start pattern of 0-1-0 from the selected card.
#define BOOT_MODE_MANDATORY (1 << 24) // Set BOOT_MODE bit to Mandatory Boot Operation.
#define BOOT_MODE_ALTERNATE (2 << 24) // Set BOOT_MODE bit to Alternate Boot Operation.
#define PRG_CLK (1 << 21)             // When this bit is set, the controller will change the clock domain and clock output. No commands will be sent.
#define SEND_INIT_SEQ (1 << 15)       // Send initialization sequence before sending this command.
#define STOP_ABT_CMD (1 << 14)        // Send Stop or Abort command to stop the current data transfer in progress. (CMD12, CMD52 for writing "I/O Abort" in SDIO_CCCR).
#define WAIT_PRE_OVER (1 << 13)       // Wait for data transfer completion before sending the current command.
#define STOP_CMD_FLAG (1 << 12)       // Send stop command automatically at the end of the data transfer. If set, the SMHC_RESP1 will record the response of auto CMD12.
#define TRANS_MODE_STREAM (1 << 11)   // Stream data transfer command. If not set, the default value 0 is Block data transfer command.
#define TRANS_DIR_READ (0 << 10)      // Set the data transfer direction to Read.
#define TRANS_DIR_WRITE (1 << 10)     // Set the data transfer direction to Write.
#define DATA_TRANS (1 << 9)           // With data transfer.
#define CHK_RESP_CRC (1 << 8)         // Check response CRC.
#define LONG_RESP (1 << 7)            // Long response type(136 bits). If not set, means short response type(48 bits).
#define RESP_RCV (1 << 6)             // Command with response. If not set, means command without response.

#define SD_MMC_CMD_WITH_RSP (1)
#define SD_MMC_CMD_WITHOUT_RSP (0)
#define SD_MMC_CMD_CHECK_RSP_CRC (1)
#define SD_MMC_CMD_NO_CHECK_RSP_CRC (0)
#define SD_MMC_CMD_WITH_LONG_RSP (1)
#define SD_MMC_CMD_WITHOUT_SHORT_RSP (0)
#define SD_MMC_CMD_WITH_DATA_TRANS (1)
#define SD_MMC_CMD_WITHOUT_DATA_TRANS (0)
#define SD_MMC_TRANS_DIR_WRITE (1)
#define SD_MMC_TRANS_DIR_READ (0)
#define SD_MMC_TRANS_MODE_STREAM (1)
#define SD_MMC_TRANS_MODE_BLOCK (0)
#define SD_MMC_STOP_CMD_FLAG (1)
#define SD_MMC_NO_STOP_CMD_FLAG (0)
#define SD_MMC_WAIT_PRE_OVER (1)
#define SD_MMC_NO_WAIT_PRE_OVER (0)
#define SD_MMC_STOP_ABT_CMD (1)
#define SD_MMC_SEND_INIT_SEQ (1)
#define SD_MMC_PRG_CLK (1)
#define SD_MMC_ALTERNATE_BOOT_OPERATION (2)
#define SD_MMC_MANDATORY_BOOT_OPERATION (1)
#define SD_MMC_EXP_BOOT_ACK (1)
#define SD_MMC_BOOT_ABT (1)
#define SD_MMC_VOL_SW (1)
#define SD_MMC_NORMAL_CMD (0)

// SD/MMC CMD defines
#define SD_MMC_DEVICE_CLK_RESET_CMD (-1)
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

#define SD_MMC_CMD_CHECK_PATTERN (0xaa)

typedef struct _SMHC_REG_LIST
{
    volatile soc_reg_t SMHC_CTRL;         // 0x0000 Control Register.
    volatile soc_reg_t SMHC_CLKDIV;       // 0x0004 Clock Control Register.
    volatile soc_reg_t SMHC_TMOUT;        // 0x0008 Time Out Register.
    volatile soc_reg_t SMHC_CTYPE;        // 0x000C Bus Width Register.
    volatile soc_reg_t SMHC_BLKSIZ;       // 0x0010 Block Size Register.
    volatile soc_reg_t SMHC_BYTCNT;       // 0x0014 Byte Count Register.
    volatile soc_reg_t SMHC_CMD;          // 0x0018 Command Register.
    volatile soc_reg_t SMHC_CMDARG;       // 0x001C Command Argument Register.
    volatile soc_reg_t SMHC_RESP0;        // 0x0020 Response 0 Register.
    volatile soc_reg_t SMHC_RESP1;        // 0x0024 Response 1 Register.
    volatile soc_reg_t SMHC_RESP2;        // 0x0028 Response 2 Register.
    volatile soc_reg_t SMHC_RESP3;        // 0x002C Response 3 Register.
    volatile soc_reg_t SMHC_INTMASK;      // 0x0030 Interrupt Mask Register.
    volatile soc_reg_t SMHC_MINTSTS;      // 0x0034 Masked Interrupt Status Register.
    volatile soc_reg_t SMHC_RINTSTS;      // 0x0038 Raw Interrupt Status Register.
    volatile soc_reg_t SMHC_STATUS;       // 0x003C Status Register.
    volatile soc_reg_t SMHC_FIFOTH;       // 0x0040 FIFO Water Level Register.
    volatile soc_reg_t SMHC_FUNS;         // 0x0044 FIFO Function Select Register.
    volatile soc_reg_t SMHC_TCBCNT;       // 0x0048 Transferred Byte Count between Controller and Card.
    volatile soc_reg_t SMHC_TBBCNT;       // 0x004C Transferred Block Count between Host Memory and Internal FIFO.
    volatile soc_reg_t SMHC_DBGC;         // 0x0050 Current Debug Control Register.
    volatile soc_reg_t SMHC_CSDC;         // 0x0054 CRC Status Detect Control Registers.
    volatile soc_reg_t SMHC_A12A;         // 0x0058 Auto Command 12 Argument Register.
    volatile soc_reg_t SMHC_NTSR;         // 0x005C SD New Timing Set Register.
    volatile soc_reg_t SMHC_HWRST;        // 0x0078 Hardware Reset Register.
    volatile soc_reg_t SMHC_IDMAC;        // 0x0080 IDMAC Control Register.
    volatile soc_reg_t SMHC_DLBA;         // 0x0084 Descriptor List Base Address Register.
    volatile soc_reg_t SNHC_IDST;         // 0x0088 IDMAC Status Register.
    volatile soc_reg_t SMHC_IDIE;         // 0x008C IDMAC Interrupt Enable Register.
    volatile soc_reg_t SMHC_THLD;         // 0x0100 Card Threshold Control Register.
    volatile soc_reg_t SMHC_SFC;          // 0x0104 Sample FIFO Control Register.
    volatile soc_reg_t SMHC_A23A;         // 0x0108 Auto Command 23 Argument Register.
    volatile soc_reg_t EMMC_DDR_SBIT_DET; // 0x010C eMMC4.5 DDR Start Bit Detection Control Register.
    volatile soc_reg_t SMHC_EXT_CMD;      // 0x0138 Extended Command Register.
    volatile soc_reg_t SMHC_EXT_RESP;     // 0x013C Extended Response Register.
    volatile soc_reg_t SMHC_DRV_DL;       // 0x0140 Drive Delay Control Register.
    volatile soc_reg_t SMHC_SMAP_DL;      // 0x0144 Sample Delay Control Register.
    volatile soc_reg_t SMHC_DS_DL;        // 0x0148 Data Strobe Delay Control Register.
    volatile soc_reg_t SMHC_HS400_DL;     // 0x014C HS400 Delay Control Register.
    volatile soc_reg_t SMHC_FIFO;         // 0x0200 Read/Write FIFO.

} SMHC_REG_LIST;

#endif /* __SMHC_DEFS_H__ */