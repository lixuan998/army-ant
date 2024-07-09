#ifndef __SMHCSD_DEFS_H__
#define __SMHCSD_DEFS_H__

#include "arch/riscv/include/riscv_type_defs.h"

#define SMHC_CARD_TYPE_MMC 0
#define SMHC_CARD_TYPE_SD 1
#define SMHC_CARD_FLAG_HIGHSPEED (1 << 0)
#define SMHC_CARD_FLAG_SDHC (1 << 1) // SDHC card.
#define SMHC_CARD_FLAG_SDXC (1 << 2) // SDXC card.

typedef struct _SCR
{
    // For more details about SCR register, look for "SD Specifications Part 1 Physical Layer Simplified Specification Version 3.01", page 106.
    uint8 CMD_SUPPORT;           // Command Support bits. bit range: [32:33], width:2, Cell type R.
    uint8 EX_SECURITY;           // Extended Security Support bits. bit range: [43:46], width:42, Cell type R.
    uint8 SD_SPEC3;              // Spec. Version 3.00 or higher. bit range: [47:47], width:12, Cell type R.
    uint8 SD_BUS_WIDTHS;         // DAT Bus widths supported. bit range: [48:51], width:42, Cell type R.
    uint8 SD_SECURITY;           // CPRM Security Support. bit range: [52:54], width:32, Cell type R.
    uint8 DATA_STAT_AFTER_ERASE; // data_status_after erases. bit range: [55:55], width:12, Cell type R.
    uint8 SD_SPEC;               // SD Memory Card - Spec. Version. bit range: [56:59], width:42, Cell type R.
    uint8 SCR_STRUCTURE;         // SCR Structure. bit range: [60:63], width:42, Cell type R.
} SCR;

typedef struct _CSD
{
    // For more details about SCR register, look for "SD Specifications Part 1 Physical Layer Simplified Specification Version 3.01", page 103.
    uint8 CRC;                // CRC. bit range: [1:7], width:7, Cell type R/W.
    uint8 FILE_FORMAT;        // File format. bit range: [10:11], width:2, Cell type R.
    uint8 TMP_WRITE_PROTECT;  // Temporary write protection. bit range: [12:12], width:1, Cell type R/W(1).
    uint8 PERM_WRITE_PROTECT; // Permanent write protection. bit range: [13:13], width:1, Cell type R/W(1).
    uint8 COPY;               // Copy flag. bit range: [14:14], width:1, Cell type R/W(1).
    uint8 FILE_FORMAT_GRP;    // File format group. bit range: [15:15], width:1, Cell type R.
    uint8 WRITE_BL_PARTIAL;   // Partial blocks for write allowed. bit range: [21:21], width:1, Cell type R.
    uint8 WRITE_BL_LEN;       // Max. write data block length. bit range: [22:25], width:4, Cell type R.
    uint8 R2W_FACTOR;         // Write speed factor. bit range: [26:28], width:3, Cell type R.
    uint8 WP_GRP_ENABLE;      // Write protect group enable. bit range: [31:31], width:1, Cell type R.
    uint8 WP_GRP_SIZE;        // Write protect group size. bit range: [32:38], width:7, Cell type R.
    uint8 SECTOR_SIZE;        // Erase sector size. bit range: [39:45], width:7, Cell type R.
    uint8 ERASE_BLK_EN;       // Erase single block enable. bit range: [46:46], width:1, Cell type R.
    uint32 C_SIZE;            // Device size. bit range: [48:69], width:22, Cell type R.
    uint8 DSR_IMP;            // DSR implemented. bit range: [76:76], width:1, Cell type R.
    uint8 READ_BLK_MISALIGN;  // Read block misalignment. bit range: [77:77], width:1, Cell type R.
    uint8 WRITE_BLK_MISALIGN; // Write block misalignment. bit range: [78:78], width:1, Cell type R.
    uint8 READ_BL_PARTIAL;    // Partial blocks for read allowed. bit range: [79:79], width:1, Cell type R.
    uint8 READ_BL_LEN;        // Max. read data block length. bit range: [80:83], width:4, Cell type R.
    uint16 CCC;               // Card command classes. bit range: [84:95], width:12, Cell type R.
    uint8 TRAN_SPEED;         // Max. transfer rate. bit range: [96:103], width:8, Cell type R.
    uint8 NSAC;               // Data read access-time in CLK (NSAC) cycles (NSAC*100). bit range: [104:111], width:8, Cell type R.
    uint8 TAAC;               // Data read access-time. bit range: [112:119], width:8, Cell type R.
    uint8 CSD_STRUCTURE;      // CSD structure. bit range: [126:127], width:2, Cell type R.
} CSD;

typedef struct _CID
{
    // For more details about SCR register, look for "SD Specifications Part 1 Physical Layer Simplified Specification Version 3.01", page 93.
    uint8 CRC;   // CRC7 checksum. bit range: [1:7], width:7.
    uint16 MDT;  // Manufacture date. bit range: [8:19], width:12.
    uint32 PSN;  // Product serial number. bit range: [24:55], width:32.
    uint8 PRV;   // Product revision. bit range: [56:664], width:8.
    char PNM[5]; // Product name. bit range: [64:103], width:40.
    uint16 OID;  // OEM/Application ID. bit range: [104:119], width:16.
    uint8 MID;   // Manufacturer ID. bit range: [120:127], width:8.
} CID;

// Card capacity.
#define MMC_VDD_165_195 (0x00000080) /* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_20_21 (0x00000100)   /* VDD voltage 2.0 ~ 2.1 */
#define MMC_VDD_21_22 (0x00000200)   /* VDD voltage 2.1 ~ 2.2 */
#define MMC_VDD_22_23 (0x00000400)   /* VDD voltage 2.2 ~ 2.3 */
#define MMC_VDD_23_24 (0x00000800)   /* VDD voltage 2.3 ~ 2.4 */
#define MMC_VDD_24_25 (0x00001000)   /* VDD voltage 2.4 ~ 2.5 */
#define MMC_VDD_25_26 (0x00002000)   /* VDD voltage 2.5 ~ 2.6 */
#define MMC_VDD_26_27 (0x00004000)   /* VDD voltage 2.6 ~ 2.7 */
#define MMC_VDD_27_28 (0x00008000)   /* VDD voltage 2.7 ~ 2.8 */
#define MMC_VDD_28_29 (0x00010000)   /* VDD voltage 2.8 ~ 2.9 */
#define MMC_VDD_29_30 (0x00020000)   /* VDD voltage 2.9 ~ 3.0 */
#define MMC_VDD_30_31 (0x00040000)   /* VDD voltage 3.0 ~ 3.1 */
#define MMC_VDD_31_32 (0x00080000)   /* VDD voltage 3.1 ~ 3.2 */
#define MMC_VDD_32_33 (0x00100000)   /* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34 (0x00200000)   /* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_34_35 (0x00400000)   /* VDD voltage 3.4 ~ 3.5 */
#define MMC_VDD_35_36 (0x00800000)   /* VDD voltage 3.5 ~ 3.6 */

typedef struct _SMHC_CARD
{
    struct _SMHC_HOST *host;
    CID cid; // CID register: Card Identification Number Register; card individual number for identification.
    CSD csd; // CSD register: Card Specific Data Register; information about the card operation conditions.
    SCR scr; // SCR register: SD Configuration Register; information about the SD Memory Card's Special Features capabilities.

    uint16 tacc_clks;     // data access time by ns
    uint32 tacc_ns;       // data access time by clk cycles
    uint32 max_data_rate; // max data transfer rate
    uint32 card_capacity; // card capacity, unit:KB
    uint32 card_blksize;  // card block size
    uint32 erase_size;    // erase size in sectors
    uint32 card_type;

    uint16 card_flag;

    uint16 hs_max_data_rate; // max data transfer rate in high speed mode
} SMHC_CARD;

typedef struct _SMHC_HOST
{
    struct _SMHC_CARD *card;

} SMHC_HOST;

#endif /* __SMHCSD_DEFS_H__ */