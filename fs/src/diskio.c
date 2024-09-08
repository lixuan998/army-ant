/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "fs/include/ff.h"	   /* Obtains integer types */
#include "fs/include/diskio.h" /* Declarations of disk functions */
#include <lib/include/stdlib.h>
#include "bsp/driver/sdio/sd.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM 0 /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC 1 /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB 2 /* Example: Map USB MSD to physical drive 2 */

#define SECTOR_SIZE 512

static BYTE virtual_disk[PAGE_SIZE * 10000];

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv)
	{
	case DEV_RAM:
		memset(virtual_disk + 512, 0, PAGE_SIZE);

		return 0;

	case DEV_MMC:
		smhcn_init(0);

		return 0;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,	  /* Physical drive nmuber to identify the drive */
	BYTE *buff,	  /* Data buffer to store read data */
	LBA_t sector, /* Start sector in LBA */
	UINT count	  /* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv)
	{
	case DEV_RAM:
		memcpy(buff, virtual_disk + sector * SECTOR_SIZE, count * SECTOR_SIZE);
		return RES_OK;

	case DEV_MMC:
		// translate the arguments here

		// result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return RES_OK;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
	BYTE pdrv,		  /* Physical drive nmuber to identify the drive */
	const BYTE *buff, /* Data to be written */
	LBA_t sector,	  /* Start sector in LBA */
	UINT count		  /* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv)
	{
	case DEV_RAM:

		memcpy(virtual_disk + sector * SECTOR_SIZE, buff, count * SECTOR_SIZE);
		return RES_OK;

	case DEV_MMC:
		// translate the arguments here

		// result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return RES_OK;
	}

	return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
	BYTE pdrv, /* Physical drive nmuber (0..) */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv)
	{
	case DEV_RAM:

		switch (cmd)
		{
		case CTRL_SYNC:
			return RES_OK;
		case GET_SECTOR_COUNT:
			*(DWORD *)buff = 80000;
			return RES_OK;
		case GET_SECTOR_SIZE:
			*(WORD *)buff = SECTOR_SIZE;
			return RES_OK;
		case GET_BLOCK_SIZE:
			*(DWORD *)buff = 1;
			return RES_OK;
		}

		return RES_PARERR;

	case DEV_MMC:

		// Process of the command for the MMC/SD card

		return RES_OK;
	}

	return RES_PARERR;
}

// 打印目录内容的函数
void print_directory(const char *path)
{
	DIR dir;
	FILINFO fno;
	FRESULT res;

	res = f_opendir(&dir, path); // 打开目录
	if (res == FR_OK)
	{
		while (1)
		{
			res = f_readdir(&dir, &fno); // 读取目录项
			if (res != FR_OK || fno.fname[0] == 0)
				break; // 错误或到达目录末尾

			if (fno.fattrib & AM_DIR)
			{
				printf("Directory: %s%s\n\r", path, fno.fname);
			}
			else
			{
				printf("File: %s%s\n\r", path, fno.fname);
			}
		}
		f_closedir(&dir); // 关闭目录
	}
	else
	{
		printf("Failed to open directory: %s\n\r", path);
	}
}
FATFS fs;
void register_virtual_disk(void)
{
	FRESULT res;
	MKFS_PARM opt = {FM_FAT32, 0, 0, 0, 0};

	// mount filesystem

	char tmp[1024] = {};
	res = f_mkfs("", &opt, tmp, 1024);
	if (res != FR_OK)
	{
		printf("Format error:%d\n\r", res);
		return;
	}
	res = f_mount(&fs, "0:/", 1);
	if (res != FR_OK)
	{
		printf("Mount error 2: %d\n\r", res);
		return;
	}
	// FIL file;
	// DWORD fre_clust, fre_sect, tot_sect;

	// /* Get volume information and free clusters of drive 1 */
	// res = f_getfree("0:", &fre_clust, &fs);

	// /* Get total sectors and free sectors */
	// tot_sect = (fs.n_fatent - 2) * fs.csize;
	// fre_sect = fre_clust * fs.csize;

	// /* Print the free space (assuming 512 bytes/sector) */
	// printf("0: %d KiB total drive space.\n\r%d KiB available.\n\r", tot_sect / 2, fre_sect / 2);
	// f_mkdir("0:/testdiR1");
	// f_mkdir("0:/testdiR2");
	// f_mkdir("0:/testdiR3");
	// f_open(&file, "0:/aaaaaaaasdfgsdgaa.txt", FA_CREATE_NEW | FA_WRITE);
	// f_write(&file, "Hello, world!", 13, NULL);
	// f_close(&file);
	// f_open(&file, "0:/aaaaaaaasdfgsdgaa.txt", FA_READ);
	// f_read(&file, tmp, 13, NULL);
	// printf("tmp: %s\n\r", tmp);

	// res = f_getfree("0:", &fre_clust, &fs);

	// /* Get total sectors and free sectors */
	// tot_sect = (fs.n_fatent - 2) * fs.csize;
	// fre_sect = fre_clust * fs.csize;

	// /* Print the free space (assuming 512 bytes/sector) */
	// printf("1: %d KiB total drive space.\n\r%d KiB available.\n\r", tot_sect / 2, fre_sect / 2);

	// print_directory("0:/");
}

void destory_virtual_disk(void)
{
	f_mount(NULL, "", 0);
}
