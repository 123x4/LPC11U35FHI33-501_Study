//***********************************************************************
//*WRITER						ADAM									*
//*PROJECT						WFLASH									*
//***********************************************************************
//***********************************************************************
//*								INCLUDE									*
//***********************************************************************
#include "../inc/LPC11Uxx.h"
#include "../inc/Main.h"
//#include "../inc/OSC.h"
#include "../inc/WFLASH.h"
//***********************************************************************
//*								DEFINE									*
//***********************************************************************

//***********************************************************************
//*								REGISTER								*
//***********************************************************************
unsigned long BootComman [5] = 
{
	0,0,0,0,0
} ;
unsigned long BootResult [4] = 
{
	0,0,0,0
} ;
unsigned char Warning ;
//***********************************************************************
//*								DECLARE									*
//***********************************************************************
unsigned long FlashPrepareSec( unsigned long StartSecNum , unsigned long EndSecNum ) ;
unsigned long FlashCopyRAMToFlash( unsigned long dst , unsigned long src , unsigned long number) ;
unsigned long FlashPEraseSec( unsigned long StartSecNum , unsigned long EndSecNum ) ;
unsigned long FlashBlankChkSec( unsigned long StartSecNum , unsigned long EndSecNum , unsigned long * pResult ) ;
unsigned long FlashReadParID( unsigned long * PartID ) ;
unsigned long FlashReadBootVer( unsigned long * MajorVer , unsigned long * MinorVer ) ;
unsigned long FlashCompare( unsigned long dst , unsigned long src , unsigned long number , unsigned long *offset ) ;
void  FlashReinvokeISP(void) ;
//***********************************************************************
//*								SUB_FUNCTION							*
//***********************************************************************
//-----FlashrepareSec-----
//***********************************************************************
//* Function Name : FlashPrepareSec										*
//* Parameters :														*
//  StartSecNum -- Start Sector Number									*
//  EndSecNum -- End Sector Number										*
//* Return : unsigned long -- Status Code								*
//* Description : This BL_command must be executed before				*
//                executing "Copy RAM to Flash" or						*
//                "Erase Sector(s)" BL_command.							*
//***********************************************************************
unsigned long FlashPrepareSec( unsigned long StartSecNum , unsigned long EndSecNum )
{
	if (EndSecNum < StartSecNum)
	{
		return IAP_STA_INVALD_PARAM ;								//return 12 ;
	}
	BootComman[0] = IAP_CMD_PrepareSec ;
	BootComman[1] = StartSecNum ;
	BootComman[2] = EndSecNum ;
	BL_iap_entry(BootComman , BootResult) ;
	return BootResult[0] ;
}

//-----FlashCopyRAMToFlash-----
//***********************************************************************
//* Function Name : FlashCopyRAMToFlash									*
//* Parameters :														*
//  dst -- Destination Flash address, should be a 256 byte boundary.	*
//  src -- Source RAM address, should be a word boundary				*
//  number -- 256 | 512 |1024 |4096										*
//  Return : unsigned long -- Status Code								*
//* Description : This BL_command is used to program the flash memory.	*
//***********************************************************************
unsigned long FlashCopyRAMToFlash( unsigned long dst , unsigned long src , unsigned long number)
{
	BootComman[0] = IAP_CMD_CopyRAMToFlash ;
	BootComman[1] = dst ;
	BootComman[2] = src ;
	BootComman[3] = number ;
	BootComman[4] = IAP_CLK / 1000 ;
	BL_iap_entry(BootComman , BootResult) ;
	return BootResult[0] ;
}

//-----FlashEraseSec-----
//***********************************************************************
//* Function Name : FlashEraseSec										*
//* Parameters :														*
//  StartSecNum -- Start Sector Number									*
//  EndSecNum -- End Sector Number										*
//* Return : unsigned long -- Status Code								*
//* Description :														*
//  This BootComman is used to erase a sector or multiple sectors of	*
//  on-chip Flash memory.												*
//***********************************************************************
unsigned long FlashEraseSec( unsigned long StartSecNum , unsigned long EndSecNum )
{
	if(EndSecNum < StartSecNum)
	{
		return IAP_STA_INVALD_PARAM ;
	}
	BootComman[0] = IAP_CMD_EraseSec ;
	BootComman[1] = StartSecNum ;
	BootComman[2] = EndSecNum ;
	BootComman[3] = IAP_CLK / 1000 ;
	BL_iap_entry(BootComman , BootResult) ;
	return BootResult[0] ;
}

//-----FlashBlankChkSec-----
//***********************************************************************
//* Function Name : FlashBlankChkSec									*
//* Parameters :														*
//  StartSecNum -- Start Sector Number									*
//  EndSecNum -- End Sector Number										*
//* Return : unsigned long -- Status Code								*
//* Description :														*
//  This BL_command is used to blank check a sector or					*
//  mutilple sectors of on-chip Flash memory.							*
//***********************************************************************
unsigned long FlashBlankChkSec( unsigned long StartSecNum , unsigned long EndSecNum , unsigned long * pResult )
{
	if (EndSecNum < StartSecNum)
	{
		return IAP_STA_INVALD_PARAM ;
	}
	BootComman[0] = IAP_CMD_BlankChkSec ;
	BootComman[1] = StartSecNum ;
	BootComman[2] = EndSecNum ;
	BL_iap_entry(BootComman , BootResult) ;
	if (BootResult[0] == IAP_STA_SECTOR_NOT_BLANK)
	{
		*pResult = BootResult[0] ;
		*(pResult+1) = BootResult[1] ;
	}
	return BootResult[0] ;
}

//-----FlashReadParID-----
//***********************************************************************
//* Function Name : FlashReadParID										*
//* Parameters :														*
//  * PartID --															*
//* Return : unsigned long -- Status Code								*
//* Description :														*
//  This BootComman is used to read the part identification number.		*
//***********************************************************************
unsigned long FlashReadParID( unsigned long * PartID )
{
	BootComman[0] = IAP_CMD_ReadParID ;
	BL_iap_entry(BootComman , BootResult) ;
	*PartID = BootResult[1] ;
	return BootResult[0] ;
}

//-----FlashReadBootVer-----
//***********************************************************************
//* Function Name : FlashReadBootVer									*
//*Parameters :															*
//  char * MajorVer														*
//  char * MinorVer														*
//* Return : unsigned long -- Status Code								*
//* Description:														*
//  This BootComman is used to read the boot code version number.		*
//***********************************************************************
unsigned long FlashReadBootVer( unsigned long * MajorVer , unsigned long * MinorVer )
{
	BootComman[0] = IAP_CMD_ReadBootVer ;
	BL_iap_entry(BootComman , BootResult) ;
	*MajorVer = (BootResult[1] & 0xff00)>>8 ;
	*MinorVer = BootResult[1] & 0xff ;
	return BootResult[0] ;
}

//-----FlashCompare-----
//***********************************************************************
//* Function Name : FlashCompare										*
//* Parameters :														*
//  dst -- Destination Flash address									*
//  src -- Source RAM address											*
//  number -- Should be in mutilple of 4								*
//* Return : unsigned long -- Status Code								*
//* Description :														*
//  This BootComman is used to compary the memory contents at two locations.
//* NOTE :																*
//  Compary BootResult may not be correct when source or destination address contains
//  any of the first 64 bytes starting from address zero.				*
//  First 64 bytes can be re-mapped to RAM.								*
//***********************************************************************
unsigned long FlashCompare( unsigned long dst , unsigned long src , unsigned long number , unsigned long *offset )
{
	BootComman[0] = IAP_CMD_Compare ;
	BootComman[1] = dst ;
	BootComman[2] = src ;
	BootComman[3] = number ;
	BL_iap_entry(BootComman , BootResult) ;
	if (BootResult[0] == IAP_STA_COMPARE_ERROR)
	{
		*offset = BootResult[1] ;
	}
	return BootResult[0] ;
}

//-----FlashReinvokeISP-----
void FlashReinvokeISP(void)
{
	BootComman[0] = IAP_CMD_REINVOKEISP ;
	BL_iap_entry(BootComman , BootResult) ; 
}
//-----
//***********************************************************************
//*								END										*
//***********************************************************************

//	for(I = 0 ; I <256 ; I++)
//	{
//		FlashData[I] = 2 ;
//	}
//	 memcpy((uint8_t *)VecDst, &FlashData[0],256);
////-----TEST
//	Warning = FlashPrepareSec(14 , 31) ;
//	Warning = FlashEraseSec(14 , 31) ;

//	Warning = FlashPrepareSec(14 , 31) ;
//	Warning = FlashCopyRAMToFlash(0x0010000 , 0x10000000 , 256) ;
