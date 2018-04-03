/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sdio_sdcard.h"


#define BLOCK_SIZE            512 /* Block Size in Bytes */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

    result = SD_Init();
    // translate the reslut code here
    stat = (result != SD_OK ) ? STA_NOINIT : RES_OK;
    
    return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
#include <string.h>
#define SD_BLOCK_SIZE            512 /* Block Size in Bytes */
__align(4) uint8_t align_buffer[SD_BLOCK_SIZE];

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	//int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		// translate the arguments here

//		//result = RAM_disk_read(buff, sector, count);

//		// translate the reslut code here
//        res = RES_PARERR;
//		return res;

//	case DEV_MMC :
		// translate the arguments here
        if((uint32_t)buff%4 != 0)    //传入的buff数据地址不是四字节对齐，需要额外处理
        {
            uint8_t i;
            for(i=0;i<count;i++)
            {
                SD_ReadBlock(align_buffer, sector*SD_BLOCK_SIZE+SD_BLOCK_SIZE*i, SD_BLOCK_SIZE);

                /* Check if the Transfer is finished */
                SD_WaitReadOperation();  //循环查询dma传输是否结束

                /* Wait until end of DMA transfer */
                while(SD_GetStatus() != SD_TRANSFER_OK);

                memcpy(buff,align_buffer,SD_BLOCK_SIZE);

                buff+=SD_BLOCK_SIZE;
            }
        }
        else
        {
            if (count > 1)
            {
                SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);

                /* Check if the Transfer is finished */
                SD_WaitReadOperation();  //循环查询dma传输是否结束

                /* Wait until end of DMA transfer */
                while(SD_GetStatus() != SD_TRANSFER_OK);
            }
            else
            {
                SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);

                /* Check if the Transfer is finished */
                SD_WaitReadOperation();  //循环查询dma传输是否结束

                /* Wait until end of DMA transfer */
                while(SD_GetStatus() != SD_TRANSFER_OK);
            }
        }

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here
        res = RES_OK;
		return res;

//	case DEV_USB :
//		// translate the arguments here

//		//result = USB_disk_read(buff, sector, count);

//		// translate the reslut code here
//        res = RES_PARERR;
//		return res;
//	}

//	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	//int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		// translate the arguments here

//		//result = RAM_disk_write(buff, sector, count);

//		// translate the reslut code here
//        res = RES_PARERR;
//		return res;

//	case DEV_MMC :
		// translate the arguments here
        
		//result = MMC_disk_write(buff, sector, count);
        if((uint32_t)buff%4 != 0)   //若传入的buff地址不是4字节对齐，需要额外处理
        {
            uint8_t i;

            for(i=0;i<count;i++)
            {
                memcpy(align_buffer,buff,SD_BLOCK_SIZE);
                SD_WriteBlock(align_buffer,sector*SD_BLOCK_SIZE+SD_BLOCK_SIZE*i,SD_BLOCK_SIZE);//单个sector的写操作

                /* Check if the Transfer is finished */
                SD_WaitWriteOperation();	                  //等待dma传输结束
                while(SD_GetStatus() != SD_TRANSFER_OK); //等待sdio到sd卡传输结束
                buff+=SD_BLOCK_SIZE;
            }
        }
        else
        {
            if (count > 1)
            {
                SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);

                /* Check if the Transfer is finished */
                SD_WaitWriteOperation();	   //等待dma传输结束
                while(SD_GetStatus() != SD_TRANSFER_OK); //等待sdio到sd卡传输结束
            }
            else
            {
                SD_WriteBlock((uint8_t *)buff,sector*BLOCK_SIZE, BLOCK_SIZE);

                /* Check if the Transfer is finished */
                SD_WaitWriteOperation();	   //等待dma传输结束
                while(SD_GetStatus() != SD_TRANSFER_OK); //等待sdio到sd卡传输结束
            }	
        }
		// translate the reslut code here
        res = RES_OK;
		return res;

//	case DEV_USB :
//		// translate the arguments here

//		//result = USB_disk_write(buff, sector, count);

//		// translate the reslut code here
//        res = RES_PARERR;
//		return res;
//	}

//	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	//int result;

//	switch (pdrv) {
//	case DEV_RAM :

//		// Process of the command for the RAM drive
//        res = RES_PARERR;
//		return res;

//	case DEV_MMC :

		// Process of the command for the MMC/SD card
        res = RES_OK;
		return res;

//	case DEV_USB :

//		// Process of the command the USB drive
//        res = RES_PARERR;
//		return res;
//	}

//	return RES_PARERR;
}


