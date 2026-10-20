/*----------------------------------------------------------------------------
 *      Name:    MAIN.C
 *      Purpose: USB HID Main
 *      Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC microcontroller devices only. Nothing else
 *      gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 * 2012/11/19 add MAC SW4,5 use Macro Key
 * 2012/11/19 Fix ubuntu mouse bug,modify usbdesc.c
 *---------------------------------------------------------------------------*/
#define  _MAIN_C_
//***********************************************************************
//*								INCLUDE									*
//***********************************************************************
//#define  __KEYTBL_C__			
//#include "Keytbl.h"



#include "lpc11Uxx.h"                        /* LPC11xx definitions */ 
#include "system_LPC11Uxx.h"
//#include "Mouse.h"
#include "type.h"
//#include "wdt.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "main.h"
#include "Timer16.h"
#include "Gpio.h"
#include "uart.h"
#include "hiduser.h"
#include "Keytbl.h"
#include "Ssp.h"
//#include "Flash.h"
#include "usbuser.h"
#include "Timer32.h"

//#include "I2c.h"
#include "MouseDatabase.h"
#include "usbdesc.h"
#include "pmu.h"
#include "Pixart_Sensor.h"
#include "led.h"
#include "eepromAPI.h"

#include "bod.h"

//-----WPI
#include "../inc/SOURCE.h"
#include "../inc/WFLASH.h"
unsigned char DFg = 0 ;
unsigned char FlashData[256] ;
typedef struct
{
  __IO uint32_t D0 ;
} LPC_RAM;
#define BootRAM       (0x10001F00UL)
#define BootRAM_Data     ((LPC_RAM*) BootRAM )
//#define ENABLE_MMAP 1
//#define Boot_loader 0
//#define USER_RAM 1   //  remap to SRAM
//#define USER_ROM 2   // remap to flash Rom
//#define VECTOR_ROM ((unsigned char *)0x00000000) //ROM Start addr
//#define VECTOR_RAM ((unsigned char *)0x10000000) //RAM Start addr
//#define VECTOR_BOOT ((unsigned char *)0x1FFF0000) //Boot Load Start addr

#define PC_AP_DELAY 1

#if POWERDOWN_MODE_USB_WAKEUP
extern volatile uint32_t SuspendFlag;
extern volatile uint32_t timer16_0_counter;
extern volatile uint32_t USBWakeupInterruptFlag;
volatile uint8_t WakeupFlag = 0;
#endif

extern bool Read_Sensor_ID(void);

LPC11Uxx_DATABASE_MOUSE_GENSETTING g_dMouse;	// For HansH Add


U8 LED_Rainbow_Color_index;	//total numbers is 12
U8 LED_Lightpipe_index;			//total numbers is 16
U8 LED_Color_Wave_counts=0;	//led effect counts

extern struct Led_Effect_Flag_t Led_Effect_Flag;
extern struct Led_Effect_t Cur_Led_Effect;
U16 LED_Rainbow_Timer;
U16 LED_Breath_Timer;
U16 LED_Colorcycle_Timer;
U8 Report_ID=0;
//U8 DCMD_STATUS=0;
//U8 HCMD_ID=0;
U16 Index=0;
U8 Length=0;
U8 Checksum=0;
//U8 Receive_buffer[58];
U16 FW_SIZE=0;
bool toggle_flag=0;
U8 u8MarcoKeyIn;
bool LED_COLOR_CHANGE_FLAG=false;
bool Command_ErrorFg=FALSE;
uint8_t SendExit_Flag=0;
extern  unsigned long *VecDst; // = (unsigned long *) 0x10000000 ;
//#define IAP_LOCATION 0x1fff1ff1
typedef void (*IAP)(unsigned int[], unsigned int[]);
IAP iap_entry = (IAP) IAP_LOCATION;
uint32_t COMMAND[5], RESULT[4];
#define init_msdstate() *((uint32_t *)(0x10000054)) = 0x0;

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void Mapp_Main_Init(void)
{
	nsAPI_USB_EndPointClr();
}
///==================================================================== HansH

#define _ERASE_EERPOM_ 1



//***********************************************************************
//*								MAIN_FUNCTION							*
//***********************************************************************
int main (void)
{
	//-----Setup
	unsigned int I;
//	S_Idle_Delay_uSec(5000);     
	//-----WPI-----------------------------------------------------------
	CheckAP( ) ;												//????
	
	//-----How to Do------------
	if(( APDataFg == 1 )&&( APRamFg == 0 ))					//Have AP Code & AP To Bootloader
//if( APDataFg == 1 )//&&( APRamFg == 0))					//Have AP Code & AP To Bootloader	
	{
		boot_jump();	
	}
	BurnAPFg = 0 ;												//Burn AP Flag (0 = No Burn Ok ; 1 = Burn AP Ok)
	//-------------------------------------------------------------------
	//Primax Setup Value
	LED_Breath_Timer=LED_Rainbow_Timer=LED_Colorcycle_Timer=0;

	g_Main_Setting.OS_ModeType = WINDOW_OS_MODE; //default window OS
	g_StaticMachineEvent1Type = SM_EVENT1_MOUSE_MONITOR; //2016_0407_Hsiang
	g_StaticMachineEvent2Type = SM_EVENT2_IDLE;
	g_StaticMachineEvent3Type = SM_EVENT3_IDLE;
	#ifdef M_EVENT4_5
	g_StaticMachineEvent4Type = SM_EVENT4_IDLE;
	g_StaticMachineEvent5Type = SM_EVENT5_IDLE;
	#endif
	#ifdef M_EVENT6_7
	g_StaticMachineEvent6Type = SM_EVENT6_IDLE;
	g_StaticMachineEvent7Type = SM_EVENT7_IDLE;
	#endif
	g_USBCore_Setting.EP0_SetReq=0;
	g_USBCore_Setting.EP0_GetReq=0;
	g_USBCore_Setting.EP0_DosMode=0;
	//g_dMouse.SensorSetting.dbReportRate =1;	// Report 1mSec	
	g_Repot_Process.Mouse_Report_In =0;
	g_Repot_Process.Media_Report_In =0;
	g_Repot_Process.System_Report_In =0;
	g_Repot_Process.Keyboard_Report_In =0;
	g_Repot_Process.Macro_Keyboard_Report_In =0;
	g_Repot_Process.Consumer_Report_In =0;
	u8MarcoKeyIn =0;
	g_Instant_Key.u8InstantKyeEnable =0;
	g_Instant_Key.u8InstantKyeNum =0xFF;
	g_Instant_Key.u8InstantProfile=0xFF;

	//-----Primax Init-----
	nsAPI_System_Init();
	nsAPI_USB_Init();/* USB Initialization */		
	Mapp_Main_Init();	// for USB 
//	S_Idle_Delay_uSec(5000);   
//	nsAPI_UART_Init(9600);
//	Is_Sensor_Read_Rom_Complete=1;
		
	#if (ENABLE_UART)
	__NOP();
	xprintf("[S]",0);
	#endif
  
	bod_set_level( bod_rst_2_63V,bod_int_2_80V  );
	bod_enable();	
	Clear_reset_status( 1 << 3 );
	
	while(1)
	{
		if (g_Main_Setting.EPInOutReq & EP4_OUT)
		{
			g_Main_Setting.EPInOutReq &= ~EP4_OUT; //clear flag
			nsAPI_USB_EP4DataOutStage();  //receive EP4 data 

		  g_StaticMachineEvent4Type = (STATIC_MACHINE_EVENT4_TYPE)g_Main_Setting.FirmwareUpdateOutReport[GIdx2]; //Byte 0 is Command
			for(I=0;I<64;I++)
			{
				CheckData[I]=g_Main_Setting.FirmwareUpdateOutReport[I];
			}
			Report_ID=g_Main_Setting.FirmwareUpdateOutReport[0];
			DCMD_STATUS=(CMD_STATUS)g_Main_Setting.FirmwareUpdateOutReport[1]; 
			if(DrvDataChecksum()==1)
					{
						Command_ErrorFg=FALSE;
					}
			else
				  {
						Command_ErrorFg=TRUE;
						DCMD_STATUS=FAILED;
						g_StaticMachineEvent4Type = SM_EVENT4_IDLE;
			      g_StaticMachineEvent5Type = (STATIC_MACHINE_EVENT5_TYPE)g_StaticMachineEvent4Type;
						nsAPI_Main_FirmwareReportHandler();
						for(I=0;I<64;I++)
					  {
						 g_Main_Setting.FirmwareUpdateInReport[I]=0;
						}
					}
//		nsAPI_USB_WriteEP(0x84, NULL, 0);	//IN_Stage();
			if(Command_ErrorFg==FALSE)
			{
			  switch(g_StaticMachineEvent4Type)
			{
			  case SM_EVENT4_GET_FIRMWAVE_VERSION:
					   g_StaticMachineEvent5Type = (STATIC_MACHINE_EVENT5_TYPE)g_StaticMachineEvent4Type;
					   g_StaticMachineEvent4Type = SM_EVENT4_IDLE;
			   break;
			 case SM_EVENT4_FIRMWARE_UPDATE:
			      g_StaticMachineEvent5Type = (STATIC_MACHINE_EVENT5_TYPE)g_StaticMachineEvent4Type;
			      g_StaticMachineEvent4Type = SM_EVENT4_IDLE;
			      DCMD_STATUS=SUCCESS;
			      nsAPI_Main_FirmwareReportHandler();	
			   break;
			 case SM_EVENT4_FIRMWARE_DATA:
						g_StaticMachineEvent5Type = (STATIC_MACHINE_EVENT5_TYPE)g_StaticMachineEvent4Type;
						g_StaticMachineEvent4Type = SM_EVENT4_IDLE;
						DCMD_STATUS=SUCCESS;
						nsAPI_Main_FirmwareReportHandler();	
			   break;
			 case SM_EVENT4_EXIT_BOOTLOADER:
						g_StaticMachineEvent5Type = (STATIC_MACHINE_EVENT5_TYPE)g_StaticMachineEvent4Type;
						g_StaticMachineEvent4Type = SM_EVENT4_IDLE;
//			    for(I=0;I<64;I++)
//			      {
//					    CheckData[I]=g_Main_Setting.FirmwareUpdateOutReport[I];
//						}
						DCMD_STATUS=SUCCESS;	
						nsAPI_Main_FirmwareReportHandler();	
						S_Idle_Delay_uSec(1000);            // dealay 1000us is used to respond first and then reset.
					
			   break;
			 default:
//				  if(g_StaticMachineEvent5Type==SM_EVENT5_IDLE) //Point is Busy
//					break;
//					else
						g_StaticMachineEvent4Type = SM_EVENT4_IDLE;
						g_StaticMachineEvent5Type = (STATIC_MACHINE_EVENT5_TYPE)SM_EVENT5_Other_Command;
						#if (ENABLE_UART)
							__NOP();
							xprintf("Idle=%d", g_StaticMachineEvent5Type);
							 #endif 
					  break;
			}	
//			nsAPI_Main_FirmwareUpdateInReportHandler();
			//***************************************
			//Process Get report
			//***************************************
			if (g_StaticMachineEvent5Type != SM_EVENT5_IDLE)
			{
				//addr = (U32*)USB_GetEPCmdStatusPtr( EPNum );
				switch(g_StaticMachineEvent5Type)
				{
					
					case SM_EVENT5_GET_FIRMWAVE_VERSION:
							g_Main_Setting.FirmwareUpdateInReport[GIdx0]= Report_ID; //Command
							g_Main_Setting.FirmwareUpdateInReport[GIdx1]= 0x00; //Key
							g_Main_Setting.FirmwareUpdateInReport[GIdx2]= g_StaticMachineEvent5Type;
							g_Main_Setting.FirmwareUpdateInReport[GIdx3]= 0x00; //DATA_INDEX
							//Data is FW version number V1.00.01 => 00 02 00 01
							g_Main_Setting.FirmwareUpdateInReport[GIdx4]= 0x00; //DATA_INDEX
							g_Main_Setting.FirmwareUpdateInReport[GIdx5]= 0x04; //DATA_LENGTH
							g_Main_Setting.FirmwareUpdateInReport[GIdx6]= 0x00; //Data0
							g_Main_Setting.FirmwareUpdateInReport[GIdx7]= 0x02; //Data1
							g_Main_Setting.FirmwareUpdateInReport[8]=     0x00; //Data2
							g_Main_Setting.FirmwareUpdateInReport[9]=     0x03; //Data3
							for(I=10;I<63;I++)
							{
							 g_Main_Setting.FirmwareUpdateInReport[I]=0;
							}
							for(I=0;I<64;I++)
							{
								Check_INData[I]=g_Main_Setting.FirmwareUpdateInReport[I];
							}
							g_Main_Setting.FirmwareUpdateInReport[63]= DrvDataIN_Checksum();
							g_StaticMachineEvent5Type=SM_EVENT5_IDLE;
							//report to PC
							g_Repot_Process.FirmwareUpdate_Report_In = 1;
//							for(j=0;j<200;j++)
//							{
//							 if(nsAPI_USB_EP_Status(HID_EP4_IN))	
//							 {
//								break;
//							 }
//							 S_Idle_Delay_uSec(10);
//							}
							nsAPI_USB_WriteEP(0x84, &g_Main_Setting.FirmwareUpdateInReport[0],64);
							S_Idle_Delay_uSec(2000);  
					break;
			
					case SM_EVENT5_FIRMWARE_UPDATE:
//						 DCMD_STATUS=SUCCESS;
							g_StaticMachineEvent5Type=SM_EVENT5_IDLE;
	//					  nsAPI_Main_FirmwareReportHandler();		
							if(ErrorFg==0)
							{
									if(FlashEraseFg ==0)
									{
									 EraseFlash( ) ;
									}
							}	
					  break;
					case SM_EVENT5_FIRMWARE_DATA:
//						DCMD_STATUS=SUCCESS;
					    g_StaticMachineEvent5Type=SM_EVENT5_IDLE;
//					  nsAPI_Main_FirmwareReportHandler();	
					    GetBytes = g_Main_Setting.FirmwareUpdateOutReport[5]; 			//Read How Many Hex Bytes to Write
							for(I = 0 ; I <GetBytes + 5 ; I++)
							{
								ChangData[I] = g_Main_Setting.FirmwareUpdateOutReport[I+6];
							}
							if(ErrorFg==0)
							{
							if(DrvDataChecksum()==1)
							{
							if(DrvDataCheck( ) == 1)		//ASCIIto Hex & Check Data CheckSum Right (Right)
							 {
								 #if (ENABLE_UART)
								 __NOP();
								 xprintf("Fl3=%x", FlashEraseFg);
								 #endif	
								if(FlashEraseFg == 0 )
								{
									EraseFlash( ) ;
								}
								BootToFlash( ) ;								//Burn to Flash
								if(BurnAPFg == 1)
								{
	//								JumpAP( ) ;
	//								BootRAM_Data->D0 = 0x00000000 ;
	//								NVIC_SystemReset( ) ; 
								}
								else
								{
									DFg = 1 ;
								}
							} 
							else												//Check Data CheckSum Right (Right)
							{
								DFg = 0 ;
							}
						 }
						} 

					break ;
					case SM_EVENT5_EXIT_BOOTLOADER:
						   g_StaticMachineEvent5Type=SM_EVENT5_IDLE;		
						  if(ErrorFg==0)
							{
								if(DrvDataChecksum()==1) 
								{
									 if(BurnAPFg == 1)          //BurnAPFg=1 Burn Finished
									{
		//								JumpAP( ) ;
//										boot_jump();
										BootRAM_Data->D0 = 0x00000000 ;
										NVIC_SystemReset( ) ; 
									}
								}
							}
					break;
					case SM_EVENT5_Other_Command:
						  DCMD_STATUS=NOTSUPPORT;
					    g_StaticMachineEvent5Type=SM_EVENT5_IDLE;
					    nsAPI_Main_FirmwareReportHandler();	
					    for(I=0;I<64;I++)
					  {
						 g_Main_Setting.FirmwareUpdateInReport[I]=0;
						}
					default:
						  g_StaticMachineEvent5Type=SM_EVENT5_IDLE;
					break;
				}
			}
			else                                               //Busy
				{
					    DCMD_STATUS=BUSY;
					    g_StaticMachineEvent5Type=SM_EVENT5_IDLE;
					    nsAPI_Main_FirmwareReportHandler();	
					    for(I=0;I<64;I++)
					  {
						 g_Main_Setting.FirmwareUpdateInReport[I]=0;
						}
			  }
		 }
		}
		//End FW upadte ****
		//(Add-)Abellee 20161019 Add FW update
//-----WPI---------------------------------------------------------------
    
	}
}
//***********************************************************************
//*                             INTERRUPT_FUNCTION                      *
//***********************************************************************

//***********************************************************************
//*                             SUB_FUNCTION                            *
//***********************************************************************
void nsAPI_Main_FirmwareReportHandler(void)
{
	  uint8_t I;
		g_Main_Setting.FirmwareUpdateInReport[GIdx0]= Report_ID; //Command
		g_Main_Setting.FirmwareUpdateInReport[GIdx1]= DCMD_STATUS;; //Key
		 for(I=2;I<63;I++)
		{
		 g_Main_Setting.FirmwareUpdateInReport[I]=CheckData[I];
		}
		for(I=0;I<64;I++)
		{
			Check_INData[I]=g_Main_Setting.FirmwareUpdateInReport[I];
		}
		g_Main_Setting.FirmwareUpdateInReport[63]= DrvDataIN_Checksum();
		g_Repot_Process.FirmwareUpdate_Report_In = 1;
//		for(j=0;j<60;j++)
//		{
//		 if(nsAPI_USB_EP_Status(HID_EP4_IN))	
//		 {
//			break;
//		 }
//		 nsAPI_SSP_Delay1us(10);
//		}
		nsAPI_USB_WriteEP(0x84, &g_Main_Setting.FirmwareUpdateInReport[0],64);

		if(SendExit_Flag==1)
		{
		 SendExit_Flag=2;
		}
		
}

#if 0
void Mapp_StaticMachineEvent1(void)
{
	switch(g_StaticMachineEvent1Type)
	{
		case SM_EVENT1_MOUSE_MONITOR:
		 	if(nsAPI_USB_EP_Status(HID_EP1_IN))
				S_Sensor_MotionCheck();
			g_StaticMachineEvent1Type = SM_EVENT1_KEYBOARD_MONITOR;
		break;
		case SM_EVENT1_KEYBOARD_MONITOR:
			nsAPI_GPIO_ButtonEventMointor();
			g_StaticMachineEvent1Type = SM_EVENT1_MOUSE_MONITOR;	//2016_0407_Hsiang
		break;
		case SM_EVENT1_IDLE:
		default:
		break;
	}
}

//-----------------------------------------------------------------------

void Mapp_StaticMachineEvent2(void)
{
	switch(g_StaticMachineEvent2Type)		//Button handle
	{
		case SM_EVENT2_BUTTON_HANDLER:
		{		
			g_StaticMachineEvent2Type = SM_EVENT2_IDLE;
		}
		break;
		//================================================================== KeyScan
		case SM_EVENT2_IDLE:
		{
			if(g_Sensor_Data.Changed == 1)
			{
				nsAPI_Main_MouseInReportHandler();
			}
		}
		default:
		break;
	}
}
 
//-----------------------------------------------------------------------
#ifdef M_Macro_Key
void Mapp_StaticMachineEvent3(void)
{
	switch(g_StaticMachineEvent3Type)
	{
		case SM_EVENT3_MACRO_HANDLER:
			MacroKey_Handler();
		break;
		case SM_EVENT3_IDLE:
			if (u8MarcoKeyIn)
			{
				MarcoKey_Release_Key();
			}
		default:
		break;
	}
}
#endif


void nsAPI_Main_FirmwareUpdateInReportHandler(void)
{
	if(nsAPI_USB_EP_Status(HID_EP4_IN))
	{
		nsAPI_USB_WriteEP(HID_EP4_IN,&g_Main_Setting.FirmwareUpdateInReport[0],FWUPDATE_IN_REPORT_SIZE);
		nsAPI_Main_Clr_EP4Event();
		g_Main_Setting.EPInOutReq &= ~EP4_IN;
		g_Repot_Process.FirmwareUpdate_Report_In = 0;
	}
}

//-----------------------------------------------------------------------
#ifdef M_EVENT6_7

#endif
//-----------------------------------------------------------------------

void nsAPI_Main_MouseInReportHandler(void)
{
	//S_Y_Sleep_ReCheck();	//2016_0621_Hsiang
	#if REMOTE_WAKEUP_ENABLE
	if ( rSuspendFlag == 1 ) 
	{
		rSuspendFlag = 0;
		S_PWM_INIT();
		//S_ADN9800_INIT_ValueReset();	//2016_0517_Hsiang
		nsAPI_Sensor_Init();		
		RemoteWakeupEvent();
		g_Main_Setting.SensorReset = 1;
		g_Repot_Process.Mouse_Report_In = 0;
		return;
  	}
	#endif
	#if (SUPPORT_MAC_BOOT_AND_DOS_MODE)
	if (g_USBCore_Setting.EP0_DosMode)
	{
		if(g_Sensor_Data.Changed == 1)
		{
			//g_Main_Setting.MouseInReport[GIdx1] = g_Sensor_Data.X1;
			//g_Main_Setting.MouseInReport[GIdx2] = g_Sensor_Data.Y1;
			//g_Main_Setting.MouseInReport[GIdx3] = 0;
			//g_Main_Setting.MouseInReport[GIdx4] = 0;
			//org size is 8 byte, 2016_0608 become 6 byte and remove id
			g_Main_Setting.MouseInReport[GIdx1] = g_Sensor_Data.X1;
			g_Main_Setting.MouseInReport[GIdx2] = g_Sensor_Data.Y1;
			g_Main_Setting.MouseInReport[GIdx3] = 0;
			g_Main_Setting.MouseInReport[GIdx4] = 0;
		}
	}
	else
	#endif //SUPPORT_MAC_BOOT_AND_DOS_MODE
	{
		//g_Main_Setting.MouseInReport[GIdx0] = 0x01;//Report ID
		if(g_Sensor_Data.Changed == 1)
		{
			//org size is 8 byte, 2016_0608 become 6 byte and remove id
			//g_Main_Setting.MouseInReport[GIdx1] = g_Sensor_Data.X1;
			//g_Main_Setting.MouseInReport[GIdx2] = g_Sensor_Data.X2;
			//g_Main_Setting.MouseInReport[GIdx3] = g_Sensor_Data.Y1;
			//g_Main_Setting.MouseInReport[GIdx4] = g_Sensor_Data.Y2;
			g_Main_Setting.MouseInReport[GIdx1] = g_Sensor_Data.X1;
			g_Main_Setting.MouseInReport[GIdx2] = g_Sensor_Data.X2;
			g_Main_Setting.MouseInReport[GIdx3] = g_Sensor_Data.Y1;
			g_Main_Setting.MouseInReport[GIdx4] = g_Sensor_Data.Y2;
		}
	}
	if(nsAPI_USB_EP_Status(HID_EP1_IN))	//if((g_Main_Setting.EPInOutReq & EP1_IN) == EP1_IN)  //modify 20120808
	{	
		nsAPI_USB_WriteEP(HID_EP1_IN, &g_Main_Setting.MouseInReport[0], MOUSE_IN_REPORT_SIZE);
		nsAPI_Main_Clr_EP1Event();
		g_Main_Setting.EPInOutReq &= ~EP1_IN;
		g_Repot_Process.Mouse_Report_In = 0;
		g_Sensor_Data.Changed = 0;
	}
}
#endif
//-----------------------------------------------------------------------
void nsAPI_Main_MediaInReportHandler(void)
{
}
//-----------------------------------------------------------------------
void nsAPI_Main_SystemInReportHandler(void)
{
}
//-----------------------------------------------------------------------
void nsAPI_Main_KeyboardInReportHandler(void)
{
}
//-----------------------------------------------------------------------
void nsAPI_Main_MacroKeyInReportHandler(void)
{
}
//-----------------------------------------------------------------------
void nsAPI_Main_ShortcutInReportHandler(void)
{
}
//-----------------------------------------------------------------------
void nsAPI_Main_ConsumerInReportHandler(void)
{
}
//-----------------------------------------------------------------------
void nsAPI_Main_Clr_EP1Event(void)
{
	//org size is 8 byte, 2016_0608 become 6 byte and remove id
	/*
	if(g_Main_Setting.MouseInReport[GIdx0] != 1)
	{
		g_Main_Setting.MouseInReport[GIdx0] = 0;
		g_Main_Setting.MouseInReport[GIdx1] = 0;
	}
	else
	*/
	{
		g_Main_Setting.MouseInReport[GIdx1] = 0;
		g_Main_Setting.MouseInReport[GIdx2] = 0;
		g_Main_Setting.MouseInReport[GIdx3] = 0;
		g_Main_Setting.MouseInReport[GIdx4] = 0;
		g_Main_Setting.MouseInReport[GIdx5] = 0;
		//g_Main_Setting.MouseInReport[GIdx7] = 0;
	}

}
//-----------------------------------------------------------------------
void nsAPI_Main_Clr_KeyboardEvent(void)
{
	g_Main_Setting.KeyboardInReport[GIdx0] = 1;
	g_Main_Setting.KeyboardInReport[GIdx1] = 0;
	g_Main_Setting.KeyboardInReport[GIdx2] = 0;
	g_Main_Setting.KeyboardInReport[GIdx3] = 0;
	g_Main_Setting.KeyboardInReport[GIdx4] = 0;
	g_Main_Setting.KeyboardInReport[GIdx5] = 0;
	g_Main_Setting.KeyboardInReport[GIdx6] = 0;
	g_Main_Setting.KeyboardInReport[GIdx7] = 0;
	g_Main_Setting.KeyboardInReport[GIdx8] = 0;	
}
//-----------------------------------------------------------------------
void nsAPI_Main_Clr_ConsumerEvent(void)
{
	g_Main_Setting.ConsumerInReport[GIdx0] = 2;
	g_Main_Setting.ConsumerInReport[GIdx1] = 0;
	g_Main_Setting.ConsumerInReport[GIdx2] = 0;
//	g_Main_Setting.ConsumerInReport[GIdx3] = 0;   //hawk
//	g_Main_Setting.ConsumerInReport[GIdx4] = 0;   //hawk
//	g_Main_Setting.ConsumerInReport[GIdx5] = 0;   //hawk
//	g_Main_Setting.ConsumerInReport[GIdx6] = 0;   //hawk
//	g_Main_Setting.ConsumerInReport[GIdx7] = 0;   //hawk
}
//-----------------------------------------------------------------------
void nsAPI_Main_Clr_EP3Event(void)      // hawk
{
	U8 i;

	for(i=0;i<CUSTOMER_IN_REPORT_SIZE;i++)
		g_Main_Setting.InReport[i] = 0;
}

void nsAPI_Main_Clr_EP4Event(void) 
{
	U8 i;
	for(i=0;i<FWUPDATE_IN_REPORT_SIZE;i++)
		g_Main_Setting.FirmwareUpdateInReport[i] = 0;
}
//-----------------------------------------------------------------------
#ifdef M_Macro_Key
void MacroKey_Handler()
{
}


void MarcoKey_Release_Key(void)
{
}

#endif
//-----------------------------------------------------------------------
/*
void HardFault_Handler(void)
{
	 xprintf("Hard Fail ",0);
}
*/
//***********************************************************************
//*                             END                                     *
//***********************************************************************



