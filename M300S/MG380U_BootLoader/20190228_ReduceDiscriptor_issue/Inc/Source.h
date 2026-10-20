#ifndef __SOURCE_H__
#define __SOURCE_H__
//***********************************************************************
//*								INCLUDE									*
//***********************************************************************

//***********************************************************************
//*								DEFINE									*
//***********************************************************************
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					Flash & Ram						~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define LPC11U37_BOOT_START			0x0000FE00//0x00003F00//0x00007F00						//Setup LPC1113 Check Position
#define LPC11U37_AP_HIGH_ADDRESS	0x00003F01//0x00007F01						//Store Jump AP High Address Position (For LPC1113)
#define LPC11U37_AP_LOW_ADDRESS		0x00003F02//0x00007F02						//Store Jump AP Low Address Position (For LPC1113)

#define	RAM_DATA_ADDR_START		0x10001F00							//Setup Ram Position (0x10001F00) to Check AP Jump Bootloader

#define	FLASH_DATA_BOOT_START	LPC11U37_BOOT_START					//Check Use LPC1113 or LPC1114
#define	AP_START_HIGH_ADDRESS	LPC11U37_AP_HIGH_ADDRESS			//Check Use LPC1113 or LPC1114
#define	AP_START_LOW_ADDRESS	LPC11U37_AP_LOW_ADDRESS				//Check Use LPC1113 or LPC1114

#define BOOT_START_HIGH_ADDRESS	0x00								//AP Jump to Bootloader High Address
#define BOOT_START_LOW_ADDRESS	0xC1								//AP Jump to Bootloader Low Address
#define application_address 0x00004000//0x00008000
#define IAP_ADDRESS				0x00010000							//AP Start Write Address

//***********************************************************************
//*								REGISTER								*
//***********************************************************************
extern unsigned int SourceForI ;
extern unsigned int SourceForJ ;

extern unsigned int APStartAddress ;
extern unsigned char FlashEraseFg ;									//Check Flash Erase(0:NO ; 1:YES)
extern unsigned char APDataFg ;										//Check APData (0:NO AP ; 1:Have AP)
extern unsigned char APRamFg ;										//Check APRam(0 = Boot IC , 1 = AP Jump Bootloader)
extern unsigned int ChangCount ;									//Count Hex Bytes
extern unsigned char ChangData[100] ;								//Store Hex Bytes
extern unsigned char CheckData[100] ;	
extern unsigned char Check_INData[100];
extern unsigned char GetCheckSum ;
extern unsigned char GetCount ;
extern unsigned char WriteStart ;
extern unsigned char GetBytes ;
extern unsigned int GetAddress ;
extern unsigned char GetType ;
extern unsigned int AddressStart ;
extern unsigned char AddressBank ;
extern unsigned char BankCount ;
extern unsigned int CodePage ;

extern unsigned char BurnAPFg ;										//Burn AP Flag (0 = No Burn Ok ; 1 = Burn AP Ok)
extern unsigned char Keyfg ;
extern unsigned char WriteAddressPage ;
extern unsigned char ErrorFg ;
extern unsigned int WriteAddressChuck ;
extern unsigned char WriteStartAddressFg ;
//***********************************************************************
//*								DECLARE									*
//***********************************************************************
extern void CheckAP(void) ;
extern void JumpAP(void) ;
extern void LeaveBootLoader(void) ;
extern unsigned char DrvDataCheck(void) ;
extern void EraseFlash(void) ;
extern void BootToFlash(void) ;
extern unsigned char DrvGPIORead(unsigned char Port ,unsigned char Set) ;
extern void Prepare_Copy( uint32_t * src, uint32_t * dst, uint32_t len );
extern void boot_jump(void);
extern unsigned char DrvDataChecksum(void);
extern unsigned char DrvDataIN_Checksum(void);

//***********************************************************************
//*								END										*
//***********************************************************************
#endif
