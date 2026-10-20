#ifndef __WFLASH_H__
#define __WFLASH_H__
//***********************************************************************
//*								INCLUDE									*
//***********************************************************************

//***********************************************************************
//*								DEFINE									*
//***********************************************************************
#define SECTOR_0_START			0x00000000
#define SECTOR_1_START			0x00001000
#define SECTOR_2_START			0x00002000
#define SECTOR_3_START			0x00003000
#define SECTOR_4_START			0x00004000
#define SECTOR_5_START			0x00005000
#define SECTOR_6_START			0x00006000
#define SECTOR_7_START			0x00007000
#define SECTOR_8_START			0x00008000
#define SECTOR_9_START			0x00009000
#define SECTOR_10_START			0x0000A000
#define SECTOR_11_START			0x0000B000
#define SECTOR_12_START			0x0000C000
#define SECTOR_13_START			0x0000D000
#define SECTOR_14_START			0x0000E000
#define SECTOR_15_START			0x0000F000
#define SECTOR_16_START			0x00010000
#define SECTOR_17_START			0x00011000
#define SECTOR_18_START			0x00012000
#define SECTOR_19_START			0x00013000
#define SECTOR_20_START			0x00014000
#define SECTOR_21_START			0x00015000
#define SECTOR_22_START			0x00016000
#define SECTOR_23_START			0x00017000
#define SECTOR_24_START			0x00018000
#define SECTOR_25_START			0x00019000
#define SECTOR_26_START			0x0001A000
#define SECTOR_27_START			0x0001B000
#define SECTOR_28_START			0x0001C000
#define SECTOR_29_START			0x0001D000
#define SECTOR_30_START			0x0001E000
#define SECTOR_31_START			0x0001F000

#define SECTOR_0_END			0x00000FFF
#define SECTOR_1_END			0x00001FFF
#define SECTOR_2_END			0x00002FFF
#define SECTOR_3_END			0x00003FFF
#define SECTOR_4_END			0x00004FFF
#define SECTOR_5_END			0x00005FFF
#define SECTOR_6_END			0x00006FFF
#define SECTOR_7_END			0x00007FFF
#define SECTOR_8_END			0x00008FFF
#define SECTOR_9_END			0x00009FFF
#define SECTOR_10_END			0x0000AFFF
#define SECTOR_11_END			0x0000BFFF
#define SECTOR_12_END			0x0000CFFF
#define SECTOR_13_END			0x0000DFFF
#define SECTOR_14_END			0x0000EFFF
#define SECTOR_15_END			0x0000FFFF
#define SECTOR_16_END			0x00010FFF
#define SECTOR_17_END			0x00011FFF
#define SECTOR_18_END			0x00012FFF
#define SECTOR_19_END			0x00013FFF
#define SECTOR_20_END			0x00014FFF
#define SECTOR_21_END			0x00015FFF
#define SECTOR_22_END			0x00016FFF
#define SECTOR_23_END			0x00017FFF
#define SECTOR_24_END			0x00018FFF
#define SECTOR_25_END			0x00019FFF
#define SECTOR_26_END			0x0001AFFF
#define SECTOR_27_END			0x0001BFFF
#define SECTOR_28_END			0x0001CFFF
#define SECTOR_29_END			0x0001DFFF
#define SECTOR_30_END			0x0001EFFF
#define SECTOR_31_END			0x0001FFFF

#define SECTOR_0				0
#define SECTOR_1				1
#define SECTOR_2				2
#define SECTOR_3				3
#define SECTOR_4				4
#define SECTOR_5				5
#define SECTOR_6				6
#define SECTOR_7				7
#define SECTOR_8				8
#define SECTOR_9				9
#define SECTOR_10				10
#define SECTOR_11				11
#define SECTOR_12				12
#define SECTOR_13				13
#define SECTOR_14				14
#define SECTOR_15				15
#define SECTOR_16				16
#define SECTOR_17				17
#define SECTOR_18				18
#define SECTOR_19				19
#define SECTOR_20				20
#define SECTOR_21				21
#define SECTOR_22				22
#define SECTOR_23				23
#define SECTOR_24				24
#define SECTOR_25				25
#define SECTOR_26				26
#define SECTOR_27				27
#define SECTOR_28				28
#define SECTOR_29				29
#define SECTOR_30				30
#define SECTOR_31				31

#define USER_START_SECTOR		4//7//15
#define USER_STOP_SECTOR		15//31

//-----IAP Command-----
#define IAP_CMD_PrepareSec		50									//Select Sector
#define IAP_CMD_CopyRAMToFlash	51									//Copy Data From Ram to Flash
#define IAP_CMD_EraseSec		52									//Erase Sector
#define IAP_CMD_BlankChkSec		53									//Check If Sector is Blank
#define IAP_CMD_ReadParID		54									//Read Chip ID
#define IAP_CMD_ReadBootVer		55									//Read BOOT Version
#define IAP_CMD_Compare			56									//Compare
#define IAP_CMD_REINVOKEISP		57									//Reinvoke ISP Command

//-----IAP Status Codes-----
#define IAP_STA_CMD_SUCCESS									0
#define IAP_STA_INVALID_COMMAND								1
#define IAP_STA_SRC_ADDR_ERROR								2
#define IAP_STA_DST_ADDR_ERROR								3
#define IAP_STA_SRC_ADDR_NOT_MAPPED							4
#define IAP_STA_DST_ADDR_NOT_MAPPED							5
#define IAP_STA_COUNT_ERROR									6
#define IAP_STA_INVALID_SECTOR								7
#define IAP_STA_SECTOR_NOT_BLANK							8
#define IAP_STA_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION		9
#define IAP_STA_COMPARE_ERROR								10
#define IAP_STA_BUSY										11
#define IAP_STA_INVALD_PARAM								12

#define Fcclk					48000000
#define IAP_CLK					Fcclk

#define IAP_LOCATION			0x1FFF1FF1
#define BL_iap_entry(a, b)		((void (*)())(IAP_LOCATION))(a, b)

#define BL_FW_STARTSECTOR		8
#define FW_ENDSECTOR			13

//***********************************************************************
//*								REGISTER								*
//***********************************************************************
extern unsigned long BootComman [5] ;
extern unsigned long BootResult [4] ;
extern unsigned char Warning ;
//***********************************************************************
//*								DECLARE									*
//***********************************************************************
extern unsigned long FlashPrepareSec( unsigned long StartSecNum , unsigned long EndSecNum ) ;
extern unsigned long FlashCopyRAMToFlash( unsigned long dst , unsigned long src , unsigned long number) ;
extern unsigned long FlashEraseSec( unsigned long StartSecNum , unsigned long EndSecNum ) ;
extern unsigned long FlashBlankChkSec( unsigned long StartSecNum , unsigned long EndSecNum , unsigned long * pResult ) ;
extern unsigned long FlashReadParID( unsigned long * PartID ) ;
extern unsigned long FlashReadBootVer( unsigned long * MajorVer , unsigned long * MinorVer ) ;
extern unsigned long FlashCompare( unsigned long dst , unsigned long src , unsigned long number , unsigned long *offset ) ;
extern void  FlashReinvokeISP(void) ;
//***********************************************************************
//*								END										*
//***********************************************************************
#endif 
