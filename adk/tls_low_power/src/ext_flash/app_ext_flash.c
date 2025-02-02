/*******************************************************************************
*
*               COPYRIGHT (c) 2012-2013 GainSpan Corporation
*                         All Rights Reserved
*
* The source code contained or described herein and all documents
* related to the source code ("Material") are owned by GainSpan
* Corporation or its licensors.  Title to the Material remains
* with GainSpan Corporation or its suppliers and licensors.
*
* The Material is protected by worldwide copyright and trade secret
* laws and treaty provisions. No part of the Material may be used,
* copied, reproduced, modified, published, uploaded, posted, transmitted,
* distributed, or disclosed in any way except in accordance with the
* applicable license agreement.
*
* No license under any patent, copyright, trade secret or other
* intellectual property right is granted to or conferred upon you by
* disclosure or delivery of the Materials, either expressly, by
* implication, inducement, estoppel, except in accordance with the
* applicable license agreement.
*
* Unless otherwise agreed by GainSpan in writing, you may not remove or
* alter this notice or any other notice embedded in Materials by GainSpan
* or GainSpan's suppliers or licensors in any way.
*
* $RCSfile: app_ext_flash.c,v
*/

#include "gsn_includes.h"
#include "safe-flash\nor\flashdrv.h"
#include "ext_app\app_ext_flash.h"

#include "gsn_br_flashldr.h"

/* gpio mappings */
#ifdef GS2011MxxS_EVB
#define GPIO_5_SPI1_CLK_5    5
#define GPIO_6_SPI1_DIN_6    6

#elif GS2100M_EVB
#define GPIO_28_SPI1_CLK_28   28
#define GPIO_30_SPI1_DIN_30   30

#else
/* by default GS2011M_EVB enable */
#define GPIO_5_SPI1_CLK_5     5
#define GPIO_30_SPI1_DIN_30   30
#endif

#define GPIO_10_SPI1_CS_10    10
#define GPIO_31_SPI1_DOUT_31  31

extern GSN_OSAL_BYTE_POOL_T romFreeMemPool;

/* Please check maximum storeable file and used sector number in FSmem.exe */
GSN_EXTFLASH_CTX_T AppExtFlashCtxFs;
extern GSN_EXTFLASH_CTX_T gsExtFlashCtxFs;

#ifdef CONFIG_FROM_SUPERBLOCK
UINT32 blockStart, descBlockStart, maxBlocks;

UINT32 AppExtFlash_FsConfig()
{
	GSN_BRFLASHLDR_SUPERBLOCK_T superBlock;
	
	GsnExtFlash_Read(&AppExtFlashCtxFs,sizeof(superBlock),0,(UINT8 *)&superBlock,GSN_FLASH_SPI_POLL,NULL,NULL);

    if (strncmp((const char*)superBlock.flok, "flok", 4) != 0)
    {
        return 1;
    }
    descBlockStart = superBlock.fs1Address / BLOCKSIZE_EF;
    blockStart = descBlockStart + DESCBLOCKS_EF;
    maxBlocks = superBlock.fs1Size / BLOCKSIZE_EF;
	maxBlocks = maxBlocks - DESCBLOCKS_EF;
    return 0;
}
#endif


int AppExtFlash_FsMount ( FS_FLASH * flash )
{
	flash->ReadFlash = GsnExtFlash_FsRead;       /* read content */
	flash->EraseFlash = GsnExtFlash_FsErase;     /* erase a block */
  	flash->WriteFlash = GsnExtFlash_FsWrite;     /* write content */
  	flash->VerifyFlash = GsnExtFlash_FsVerifyFlash;   /* verify content */
  	flash->BlockCopy = GsnExtFlash_FsBlockCopy;

	flash->maxblock = MAXBLOCKS_EF;
	flash->blocksize = BLOCKSIZE_EF;
	flash->sectorsize = SECTORSIZE_EF;
	flash->sectorperblock = SECTORPERBLOCK_EF;
	flash->blockstart = BLOCKSTART_EF;     /* where 1st block starts */
	flash->descsize = DESCSIZE_EF;         /* 128K      */
	flash->descblockstart = DESCBLOCKSTART_EF;
	flash->descblockend = DESCBLOCKSTART_EF + DESCBLOCKS_EF - 1; /*  num of last desc block  */
	flash->cacheddescsize = DESCCACHE_EF;                     /*  size of write cache in descriptor  */
	return 0;
}
VOID
AppExtFlash_Init(GSN_FLASH_SPI_FLAG_T spiFlag)
{
	if (gsExtFlashCtxFs.spiPinConf != APP_SPI_ALL_PIN_CONF)
	{
	/*    GPIO Mux enable for SPI Port-1	   */
#ifdef GS2011MxxS_EVB
	GsnIoMux_Sel(GPIO_6_SPI1_DIN_6, GSN_IOMUX_GPIO_6_ASSP1_DIN); // DIN
	GsnIoMux_PullDownEnable (GPIO_6_SPI1_DIN_6);
	GsnIoMux_Sel(GPIO_5_SPI1_CLK_5, GSN_IOMUX_GPIO_5_ASSP1_CLK); // CLK
	GsnIoMux_PullDownEnable (GPIO_5_SPI1_CLK_5);
#elif GS2100M_EVB
	GsnIoMux_Sel(GPIO_30_SPI1_DIN_30, GSN_IOMUX_GPIO_30_ASSP1_DIN); // DIN
	GsnIoMux_PullDownEnable (GPIO_30_SPI1_DIN_30);
	GsnIoMux_Sel(GPIO_28_SPI1_CLK_28, GSN_IOMUX_GPIO_28_ASSP1_CLK); // CLK
	GsnIoMux_PullDownEnable (GPIO_28_SPI1_CLK_28);
#else
	/* by default GS2011M_EVB GPIO Mux enable */
	GsnIoMux_Sel(GPIO_30_SPI1_DIN_30, GSN_IOMUX_GPIO_30_ASSP1_DIN); // DIN
	GsnIoMux_PullDownEnable (GPIO_30_SPI1_DIN_30);
	GsnIoMux_Sel(GPIO_5_SPI1_CLK_5, GSN_IOMUX_GPIO_5_ASSP1_CLK); // CLK
	GsnIoMux_PullDownEnable (GPIO_5_SPI1_CLK_5);
#endif

	GsnIoMux_Sel(GPIO_10_SPI1_CS_10, GSN_IOMUX_GPIO_10_AGPIO_10); // CS
	GsnIoMux_PullDownDisable (GPIO_10_SPI1_CS_10);

	GsnIoMux_Sel(GPIO_31_SPI1_DOUT_31, GSN_IOMUX_GPIO_31_ASSP1_DOUT); // DOUT
	GsnIoMux_PullDownEnable (GPIO_31_SPI1_DOUT_31);

		/* Use SPI CS pin10 as GPIO to control from driver */
		GsnGpio_Init(GS2011_SPI_CS, 0xfffffBff);
		gsExtFlashCtxFs.spiCsGpio = GS2011_SPI_CS;
	}

	gsExtFlashCtxFs.spiConfig.spiMode			= GSN_SPI_OPERATION_MODE_MASTER;
	gsExtFlashCtxFs.spiConfig.clockDivider 		= 2;

	gsExtFlashCtxFs.spiConfig.spiProtocol		= GSN_SPI_PROTOCOL_MOTOROLA;
	gsExtFlashCtxFs.spiConfig.clockSelect		= GSN_SPI_CLK_XTAL; //GSN_SPI_CLK_XTAL;GSN_SPI_CLK_HSRC

	/*	  SPI Mode-1	*/
	gsExtFlashCtxFs.spiConfig.clockPolarity		= GSN_SPI_CLK_POL_INACTIVEHIGH;
	gsExtFlashCtxFs.spiConfig.clockPhase		= GSN_SPI_CLK_PHASE_180;
	gsExtFlashCtxFs.spiConfig.dataWordSize 		= GSN_SPI_DATAWORDSIZE_8;
	gsExtFlashCtxFs.spiFlagFs					= spiFlag;


	if(spiFlag == GSN_FLASH_SPI_POLL)
		gsExtFlashCtxFs.spiConfig.dmaMode		= GSN_SPI_DMA_CLEAR;
	else
		gsExtFlashCtxFs.spiConfig.dmaMode		= GSN_SPI_DMA_SET;


	GsnExtFlash_Init(&gsExtFlashCtxFs,SPI_NUM_1,&gsExtFlashCtxFs.spiConfig,gsExtFlashCtxFs.spiFlagFs);

	gsExtFlashCtxFs.extFlashStatus = EXTFLASH_INITIALIZED;

}

VOID
App_ExtFlash_FsInit()
{
	UINT32 memsize;
	INT32 rc;
	UINT8 *pFsBuff;
	
	if(gsExtFlashCtxFs.extFlashStatus == EXTFLASH_NOT_INITIALIZED)
	{
		AppExtFlash_Init(GSN_FLASH_SPI_POLL);
	}

	GsnExtFlash_FsInit(&gsExtFlashCtxFs);

	memsize = fs_getmem_flashdrive(AppExtFlash_FsMount);
	pFsBuff = gsn_malloc(memsize);

    if(pFsBuff != NULL)
    {
		rc = f_mountdrive(2, pFsBuff, memsize, (void *)fs_mount_flashdrive,
			AppExtFlash_FsMount);
                
		if(rc == F_ERR_NOTFORMATTED)
		{
                    f_format(2);
		}

	}
	
}


