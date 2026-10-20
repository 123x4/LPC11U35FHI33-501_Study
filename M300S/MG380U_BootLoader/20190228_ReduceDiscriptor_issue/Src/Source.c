//***********************************************************************
//*Writer						ADAM									*
//*Function						SOURCE									*
//***********************************************************************
//***********************************************************************
//*								INCLUDE									*
//***********************************************************************
#include "../inc/LPC11Uxx.h"
#include "../inc/Main.h"
//#include "../inc/OSC.h"
#include "../inc/SOURCE.h"
#include "../inc/WFLASH.h"
#include <string.h>
//***********************************************************************
//*								REGISTER								*
//***********************************************************************
unsigned int SourceForI ;
unsigned int SourceForJ ;
unsigned long *VecDst = (unsigned long *) 0x10000000 ;
char *APData ;														//Read AP Flash & Ram Data
char *Address ;														//Read AP Start Address
unsigned int APStartAddress ;										//AP Start Address
unsigned char FlashEraseFg ;										//Erase Flash Flag (0 = No Erase ; 1 = Have Erase)
unsigned char APDataFg ;											//AP Data flag (0 = No AP ; 1 = Have AP)
unsigned char APRamFg ;												//Check APRam(0 = Boot IC , 1 = AP Jump Bootloader)
unsigned int ChangCount ;											//Cont Hex Bytes
unsigned char ChangData[100] ;										//Store Hex Bytes
unsigned char CheckData[100] ;
unsigned char Check_INData[100];
unsigned char GetCheckSum ;
unsigned char GetCount ;
unsigned char WriteStart ;
unsigned char GetBytes ;
unsigned int GetAddress ;
unsigned char GetType ;
unsigned int AddressStart ;
unsigned char AddressBank ;
unsigned char BankCount ;
unsigned int CodePage ;

unsigned char BurnAPFg ;											//Burn AP Flag (0 = No Burn Ok ; 1 = Burn AP Ok)
unsigned char Keyfg = 0 ;
unsigned char WriteAddressPage = 0 ;
unsigned int CountDelay = 0 ;
unsigned char ErrorFg = 0 ;
unsigned int WriteAddressChuck ;
unsigned char WriteStartAddressFg ;
unsigned char AllchuckFg = 0 ;
//***********************************************************************
//*                             DECLARE                                 *
//***********************************************************************
void CheckAP(void) ;
void JumpAP(void) ;
void LeaveBootLoader(void) ;
unsigned char DrvDataCheck(void) ;
void EraseFlash(void) ;
void BootToFlash(void) ;

unsigned char DrvGPIORead(unsigned char Port ,unsigned char Set) ;
//***********************************************************************
//*                             INTERRUPT_FUNCTION                      *
//***********************************************************************

//***********************************************************************
//*                             SUB_FUNCTION                            *
//***********************************************************************
//-----CheckAP-----
 void CheckAP(void)
{
	uint32_t SW_Reset_Value = 0;
	uint8_t SW_Reset_Flag = 0;

	APData = (char*)FLASH_DATA_BOOT_START ;	//Read AP Data To Determine AP information (Flash) 0x5F00
	if(*APData != 0xAA)			 //No AP Code or AP Code Error
	{
	  EraseFlash( ) ;				 //Erase AP Flash (SOURCE.c)
	}
	else										 //Have AP Code
	{
		FlashEraseFg = 0 ;		 //Flash Erase Flag (0 = No Erase AP , 1 = Erase AP)
		APDataFg = 1 ;				 //AP Data Flag (1 = Have AP , 0 = No AP)
		//-----Read AP Start Address-------------------------------------
		Address = (char*)AP_START_HIGH_ADDRESS; //Read AP Start Address
		APStartAddress = *Address << 8;
		Address = (char*)AP_START_LOW_ADDRESS;
		APStartAddress |= *Address;
		//---------------------------------------------------------------
	}
	//-----Read One RAM Data---------------------------------------------
	APData = (char*)RAM_DATA_ADDR_START;							//Read AP Data To Determine AP information (RAM)
	SW_Reset_Value = LPC_SYSCON->SYSRSTSTAT ;         // Check SW reset flag , add by Martin 2019.3.1
	if((SW_Reset_Value & 1<<4) != 0)
	{
		SW_Reset_Flag = 1; 
		LPC_SYSCON->SYSRSTSTAT |=  1<<4;
	}
	if((*APData == 0xAA) && (SW_Reset_Flag ==1))
	{
		APRamFg = 1 ;												//Check AP Jump to Bootloader
	}
	else
	{
		APRamFg = 0 ;												//Not AP Jump to Bootloader
	}

}
//-----JumpAP-----
void JumpAP(void)
{
	//-----??????
	//-----
	LeaveBootLoader( ) ;
}


void Prepare_Copy( uint32_t * src, uint32_t * dst, uint32_t len )
{
		if(dst != src)
		{
		while(len)
		{
		*dst++ = *src++;
		len -= 4;
		}
		}
}

void boot_jump(void)
{
		uint32_t entry;
		__disable_irq();
		Prepare_Copy((uint32_t *)application_address, (uint32_t*)0x10000000, 512);
		LPC_SYSCON -> SYSMEMREMAP = 0x01;
		__enable_irq();
		__set_MSP(*(uint32_t *) application_address);
		entry = *((volatile unsigned int *)( application_address+4));
		((void(*)(void))entry)();
}
//-----LeaveBootLoader-----
void LeaveBootLoader(void)
{
	void (*user_code_entry)(void) ;
	//user_code_entry = (void (*)(void))((APStartAddress+0x10000)) ;
	user_code_entry = (void (*)(void))((0x00C1+0x10000)) ;
	user_code_entry( ) ;
}
//-----DrvDataCheck-----
unsigned char DrvDataCheck(void)
{
	ChangCount = ChangData[0]+5 ;
    GetCheckSum = 0 ;												//GetCheckSum Return 0
    for(SourceForI = 0 ; SourceForI < (ChangCount-1) ; SourceForI++)
    {
        GetCheckSum += ChangData[SourceForI]  ;
    }
    GetCheckSum =  0x01 + ~( GetCheckSum) ;
    if(GetCheckSum == ChangData[ChangCount-1] )
    {
        GetCount = ChangCount-1 ;
        return 1 ;
    }
    else
    {
        return 0 ;
    }
}

unsigned char DrvDataChecksum(void)
{
	 uint8_t i=0;
   uint16_t GetCheckSumC = 0 ;
   for(i=0;i<63;i++)
	  {
		 GetCheckSumC+= CheckData[i];
		}
    GetCheckSumC=0x55-GetCheckSumC&(0xFF);
		if((GetCheckSumC&0x00FF)==CheckData[63])
		{
		 return 1;	
		}
		else
		{
			return 0;
		}
}

unsigned char DrvDataIN_Checksum(void)
{
	 uint8_t i=0;
   uint16_t GetCheckSumC = 0 ;
   for(i=0;i<63;i++)
	  {
		 GetCheckSumC+= Check_INData[i];
		}
    GetCheckSumC=0x55-GetCheckSumC&(0xFF);
		GetCheckSumC=GetCheckSumC&0x00FF;
		return GetCheckSumC;
}

void Disable_IRQ(void)
{
		NVIC_DisableIRQ(SysTick_IRQn);
		NVIC_DisableIRQ(UART_IRQn);
		NVIC_DisableIRQ(TIMER_32_0_IRQn);
		NVIC_DisableIRQ(TIMER_32_1_IRQn);
		NVIC_DisableIRQ(TIMER_16_0_IRQn);
		NVIC_DisableIRQ(TIMER_16_1_IRQn);
		NVIC_DisableIRQ(I2C_IRQn);
		NVIC_DisableIRQ(GINT0_IRQn);
		NVIC_DisableIRQ(GINT1_IRQn);
		NVIC_DisableIRQ(SSP0_IRQn);
		NVIC_DisableIRQ(SSP1_IRQn);
		NVIC_DisableIRQ(USB_FIQn);
		NVIC_DisableIRQ(USB_IRQn) ;
}

void Enable_IRQ(void)
{
		NVIC_EnableIRQ(UART_IRQn);
		NVIC_EnableIRQ(TIMER_32_0_IRQn);
		NVIC_EnableIRQ(TIMER_16_0_IRQn);
		NVIC_EnableIRQ(TIMER_16_1_IRQn);
		NVIC_EnableIRQ(TIMER_32_1_IRQn);
		NVIC_EnableIRQ(I2C_IRQn);
		NVIC_EnableIRQ(GINT0_IRQn);
		NVIC_EnableIRQ(GINT1_IRQn);
		NVIC_EnableIRQ(SSP0_IRQn);
		NVIC_EnableIRQ(SSP1_IRQn);
		NVIC_EnableIRQ(USB_FIQn);
		NVIC_EnableIRQ(USB_IRQn) ;
}

//-----EraseFlash-----
void EraseFlash(void)
{

//				NVIC_DisableIRQ(SysTick_IRQn);
//				NVIC_DisableIRQ(UART_IRQn);
//				NVIC_DisableIRQ(TIMER_32_0_IRQn);
//				NVIC_DisableIRQ(TIMER_32_1_IRQn);
//				NVIC_DisableIRQ(TIMER_16_0_IRQn);
//				NVIC_DisableIRQ(TIMER_16_1_IRQn);
//				NVIC_DisableIRQ(I2C_IRQn);
//				NVIC_DisableIRQ(GINT0_IRQn);
//				NVIC_DisableIRQ(GINT1_IRQn);
//				NVIC_DisableIRQ(SSP0_IRQn);
//				NVIC_DisableIRQ(SSP1_IRQn);
//				NVIC_DisableIRQ(USB_FIQn);
//				NVIC_DisableIRQ(USB_IRQn) ;
	Disable_IRQ();
	//-----Erase Chip(One Page 100mS)
	Warning = FlashPrepareSec(USER_START_SECTOR , USER_STOP_SECTOR) ;
	Warning = FlashEraseSec(USER_START_SECTOR , USER_STOP_SECTOR) ;
	//-------------------------------
	Enable_IRQ();
//				NVIC_EnableIRQ(UART_IRQn);
//				NVIC_EnableIRQ(TIMER_32_0_IRQn);
//				NVIC_EnableIRQ(TIMER_16_0_IRQn);
//				NVIC_EnableIRQ(TIMER_16_1_IRQn);
//				NVIC_EnableIRQ(TIMER_32_1_IRQn);
//				NVIC_EnableIRQ(I2C_IRQn);
//				NVIC_EnableIRQ(GINT0_IRQn);
//				NVIC_EnableIRQ(GINT1_IRQn);
//				NVIC_EnableIRQ(SSP0_IRQn);
//				NVIC_EnableIRQ(SSP1_IRQn);
//				NVIC_EnableIRQ(USB_FIQn);
//				NVIC_EnableIRQ(USB_IRQn);
				
	FlashEraseFg = 1 ;												//Erase Flash Flag (0 = No Erase ; 1 = Have Erase)
	WriteStart = 0 ;
	APDataFg = 0 ;													//AP Data flag (0 = No AP ; 1 = Have AP)
	WriteAddressPage = 0 ;
	ErrorFg = 0 ;

}
//-----BootToFlash-----
void BootToFlash(void)
{
	#if (ENABLE_UART)
	 __NOP();
	 xprintf("Flash4=%x", FlashEraseFg);
	 #endif	
	GetBytes = ChangData[0] ;										//Read How Many Hex Bytes to Write
	GetAddress = (ChangData[1] <<8 | ChangData[2]) & 0xFFFF ;		//Read Write Address
	GetType = ChangData[3];										//Check Hex Type
	if(WriteStartAddressFg==0 && GetType==0)
	{
		WriteAddressChuck = GetAddress ;
		WriteAddressChuck += GetBytes ;
		WriteStartAddressFg = 1 ;
	}
	else if(WriteStartAddressFg==1 && GetType==0)
	{
		if(WriteAddressChuck != GetAddress)
		{
			ErrorFg = 1 ;
		}
		else
		{
			WriteAddressChuck += GetBytes ;
		}
			
	}
	switch(GetType)
    {
        case 0 :                                                    //Data Record
            if(WriteStart==0)										//
            {
                WriteStart = 1 ;									//WristStart (0 = No Flash Write ; 1 = Flash Start Write)
                BankCount = 0 ;
            }
            AddressStart = GetAddress % 0x0100 ;					//Get Write Address
            AddressBank = GetAddress / 0x0100  ;					//Get Write Bank
            for(SourceForI=4 ; SourceForI< (GetBytes + 4) ; SourceForI++ )
            {
                FlashData[AddressStart] = ChangData[SourceForI] ;
                AddressStart++ ;
            }	
            if(AddressStart==0x0100)//0x0100
            {
//				NVIC_DisableIRQ(SysTick_IRQn);
//				NVIC_DisableIRQ(UART_IRQn);
//				NVIC_DisableIRQ(TIMER_32_0_IRQn);
//				NVIC_DisableIRQ(TIMER_32_1_IRQn);
//				NVIC_DisableIRQ(TIMER_16_0_IRQn);
//				NVIC_DisableIRQ(TIMER_16_1_IRQn);
//				NVIC_DisableIRQ(I2C_IRQn);
//				NVIC_DisableIRQ(GINT0_IRQn);
//				NVIC_DisableIRQ(GINT1_IRQn);
//				NVIC_DisableIRQ(SSP0_IRQn);
//				NVIC_DisableIRQ(SSP1_IRQn);
//				NVIC_DisableIRQ(USB_FIQn);
//				NVIC_DisableIRQ(USB_IRQn) ;
        Disable_IRQ();
				memcpy((uint8_t *)VecDst, &FlashData[0],256);
                Warning = FlashPrepareSec(USER_START_SECTOR , USER_STOP_SECTOR) ;
				Warning = FlashCopyRAMToFlash((CodePage+(0x0100*AddressBank)) , 0x10000000 , 256) ;
//                Warning = FlashCopyRAMToFlash((0x0100*AddressBank) , (unsigned long)FlashData , 256) ;
				Enable_IRQ();			
//				NVIC_EnableIRQ(UART_IRQn);
//				NVIC_EnableIRQ(TIMER_32_0_IRQn);
//				NVIC_EnableIRQ(TIMER_16_0_IRQn);
//				NVIC_EnableIRQ(TIMER_16_1_IRQn);
//				NVIC_EnableIRQ(TIMER_32_1_IRQn);
//				NVIC_EnableIRQ(I2C_IRQn);
//				NVIC_EnableIRQ(GINT0_IRQn);
//				NVIC_EnableIRQ(GINT1_IRQn);
//				NVIC_EnableIRQ(SSP0_IRQn);
//				NVIC_EnableIRQ(SSP1_IRQn);
//				NVIC_EnableIRQ(USB_FIQn);
//				NVIC_EnableIRQ(USB_IRQn) ;
				
        for( SourceForI = 0 ;  SourceForI<256 ;  SourceForI++)
        {
           FlashData[SourceForI] = 0xFF ;
        }
          BankCount++ ;
         }
        break ;
        case 1 :													//End Of File Record
//			if(AllchuckFg==1)
			{
            //-----adam_20100819
				FlashData[0] = 0xAA ;
				FlashData[1] = (APStartAddress >> 8) & 0x00FF ;
				FlashData[2] = APStartAddress & 0x00FF ;
				FlashData[3] = BOOT_START_HIGH_ADDRESS ;
				FlashData[4] = BOOT_START_LOW_ADDRESS ;
		
//				NVIC_DisableIRQ(SysTick_IRQn);
//				NVIC_DisableIRQ(UART_IRQn);
//				NVIC_DisableIRQ(TIMER_32_0_IRQn);
//				NVIC_DisableIRQ(TIMER_32_1_IRQn) ;
//				NVIC_DisableIRQ(TIMER_16_0_IRQn);
//				NVIC_DisableIRQ(TIMER_16_1_IRQn);
//				NVIC_DisableIRQ(I2C_IRQn);
//				NVIC_DisableIRQ(GINT0_IRQn);
//				NVIC_DisableIRQ(GINT1_IRQn);
//				NVIC_DisableIRQ(SSP0_IRQn);
//				NVIC_DisableIRQ(SSP1_IRQn);
//				NVIC_DisableIRQ(USB_FIQn);
//				NVIC_DisableIRQ(USB_IRQn) ;
			    Disable_IRQ();
            for( SourceForI = 5 ;  SourceForI<251 ;  SourceForI++)
            {
                FlashData[SourceForI] = 0xAA ;
            }
			      memcpy((uint8_t *)VecDst, &FlashData[0],256);
            Warning = FlashPrepareSec(USER_START_SECTOR , USER_STOP_SECTOR) ;
            Warning = FlashCopyRAMToFlash(FLASH_DATA_BOOT_START , 0x10000000 , 256) ;
          Enable_IRQ();		
//				NVIC_EnableIRQ(UART_IRQn);
//				NVIC_EnableIRQ(TIMER_32_0_IRQn);
//				NVIC_EnableIRQ(TIMER_16_0_IRQn);
//				NVIC_EnableIRQ(TIMER_16_1_IRQn);
//				NVIC_EnableIRQ(TIMER_32_1_IRQn) ;
//				NVIC_EnableIRQ(I2C_IRQn);
//				NVIC_EnableIRQ(GINT0_IRQn);
//				NVIC_EnableIRQ(GINT1_IRQn);
//				NVIC_EnableIRQ(SSP0_IRQn);
//				NVIC_EnableIRQ(SSP1_IRQn);
//				NVIC_EnableIRQ(USB_FIQn);
//				NVIC_EnableIRQ(USB_IRQn) ;
				for( SourceForI = 0 ;  SourceForI<256 ;  SourceForI++)
				{
						FlashData[SourceForI] = 0xFF ;
				}
            //-----end
				BurnAPFg = 1 ;											//Burn AP Ok
			}
//			else
			{
//				ErrorFg = 1 ;
			}
        break ;
        case 2 :													//
            
        break ;
        case 3 :													//
            
        break ;
        case 4 :													//Extended Linear Address Record
				CodePage = 0x10000*ChangData[5] ;
				WriteStartAddressFg = 0 ;
				if(WriteAddressPage==0)
				{
					WriteAddressPage = 1 ;
				}
				else
				{
					APStartAddress  = (ChangData[6] <<8 | ChangData[7]) & 0xFFFF ;
					memcpy((uint8_t *)VecDst, &FlashData[0],256);

					NVIC_DisableIRQ(UART_IRQn);
					NVIC_DisableIRQ(I2C_IRQn);
					NVIC_DisableIRQ(GINT0_IRQn);
					NVIC_DisableIRQ(GINT1_IRQn);
					NVIC_DisableIRQ(SSP0_IRQn);
					NVIC_DisableIRQ(SSP1_IRQn);
					NVIC_DisableIRQ(USB_FIQn);
					NVIC_DisableIRQ(USB_IRQn) ;
				
					Warning = FlashPrepareSec(USER_START_SECTOR , USER_STOP_SECTOR) ;
					Warning = FlashCopyRAMToFlash((CodePage+(0x0100*AddressBank)) , 0x10000000 , 256) ;
//			Warning = FlashCopyRAMToFlash((IAP_ADDRESS+(0x0100*BankCount)) , (unsigned long)FlashData , 256) ;		
					NVIC_EnableIRQ(UART_IRQn);
					NVIC_EnableIRQ(I2C_IRQn);
					NVIC_EnableIRQ(GINT0_IRQn);
					NVIC_EnableIRQ(GINT1_IRQn);
					NVIC_EnableIRQ(SSP0_IRQn);
					NVIC_EnableIRQ(SSP1_IRQn);
					NVIC_EnableIRQ(USB_FIQn);
					NVIC_EnableIRQ(USB_IRQn) ;					
				}
        break ;
        case 5 :													//Start Linear Address Record
			APStartAddress  = (ChangData[6] <<8 | ChangData[7]) & 0xFFFF ;
//				NVIC_DisableIRQ(SysTick_IRQn);
//				NVIC_DisableIRQ(UART_IRQn);
//				NVIC_DisableIRQ(TIMER_32_0_IRQn);
//				NVIC_DisableIRQ(TIMER_32_1_IRQn) ;
//				NVIC_DisableIRQ(TIMER_16_0_IRQn);
//				NVIC_DisableIRQ(TIMER_16_1_IRQn);
//				NVIC_DisableIRQ(I2C_IRQn);
//				NVIC_DisableIRQ(GINT0_IRQn);
//				NVIC_DisableIRQ(GINT1_IRQn);
//				NVIC_DisableIRQ(SSP0_IRQn);
//				NVIC_DisableIRQ(SSP1_IRQn);
//				NVIC_DisableIRQ(USB_FIQn);
//				NVIC_DisableIRQ(USB_IRQn) ;
			  Disable_IRQ();			
			memcpy((uint8_t *)VecDst, &FlashData[0],256);		
			Warning = FlashPrepareSec(USER_START_SECTOR , USER_STOP_SECTOR) ;
			Warning = FlashCopyRAMToFlash((CodePage+(0x0100*AddressBank)) , 0x10000000 , 256) ;
				  Enable_IRQ();
//				NVIC_EnableIRQ(UART_IRQn);
//				NVIC_EnableIRQ(TIMER_32_0_IRQn);
//				NVIC_EnableIRQ(TIMER_16_0_IRQn);
//				NVIC_EnableIRQ(TIMER_16_1_IRQn);
//				NVIC_EnableIRQ(TIMER_32_1_IRQn) ;
//				NVIC_EnableIRQ(I2C_IRQn);
//				NVIC_EnableIRQ(GINT0_IRQn);
//				NVIC_EnableIRQ(GINT1_IRQn);
//				NVIC_EnableIRQ(SSP0_IRQn);
//				NVIC_EnableIRQ(SSP1_IRQn);
//				NVIC_EnableIRQ(USB_FIQn);
//				NVIC_EnableIRQ(USB_IRQn) ;
//			Warning = FlashCopyRAMToFlash((IAP_ADDRESS+(0x0100*BankCount)) , (unsigned long)FlashData , 256) ;
        break ;
		case 6 :
			
			if((WriteAddressChuck & 0xFFFF) != GetAddress)
			{
				ErrorFg = 1 ;
			}
			else
			{
				AllchuckFg = 1 ;
			}
		break ;
       
        default:
            
        break ;
    }

}
//-----------------------------------------------------------------------
unsigned char DrvGPIORead(unsigned char Port ,unsigned char Set)
{
	unsigned char RegVal ;
	unsigned long Data ;
	Data = (LPC_GPIO->PIN[Port] >> Set) ;
	Data = (Data & 0x01) ; 
	if( Data == 1)
	{
		RegVal = 1 ;
	}
	else
	{
		RegVal = 0 ;
	}
	return ( RegVal );	
}
//***********************************************************************
//*                             END                                     *
//***********************************************************************

