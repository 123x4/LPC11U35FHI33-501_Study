/****************************************************************************
 *   $Id:: gpio.c 6874 2011-03-22 01:58:31Z usb00423                        $
 *   Project: NXP LPC11xx GPIO example
 *
 *   Description:
 *     This file contains GPIO code example which include GPIO
 *     initialization, GPIO interrupt handler, and related APIs for
 *     GPIO access.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#define __GPIO_C__

#include <stdbool.h>
#include "LPC11Uxx.h"/* LPC11xx Peripheral Registers */
#include "Mouse.h"
#include "gpio.h"
#include "uart.h"
#include "main.h"
#include "Keytbl.h"
#include "Ssp.h"
//#include "I2c.h"
#include "Timer32.h"
#include "MouseDatabase.h"


#include "Wheel.h"

#if 1
//#define AP_CTL_GPIO 0;

extern U8	R_Scroll_Value;
extern U16 R_KeyStatus ;
extern U16 R_KeyScan_Bounce ;
#ifdef V_SleepMode_Pin_Debug
extern	U8	F_Togle;
#endif

#if 0 //no use
volatile uint32_t gint0_counter = 0;
volatile uint32_t gint1_counter = 0;
volatile uint32_t gint0_level_counter = 0;
volatile uint32_t gint0_edge_counter = 0;
volatile uint32_t gint1_level_counter = 0;
volatile uint32_t gint1_edge_counter = 0;
#endif
/*****************************************************************************
** Function name:		FLEX_INT0_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
/*
void FLEX_INT0_IRQHandler(void)
{
	while(1){}
}
*/
/*****************************************************************************
** Function name:		FLEX_INT1_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
/*
void FLEX_INT1_IRQHandler(void)
{
	while(1){}
}
*/
/*****************************************************************************
** Function name:		FLEX_INT2_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
/*
void FLEX_INT2_IRQHandler(void)
{
	while(1){}
}
*/
/*****************************************************************************
** Function name:		FLEX_INT3_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
/*
void FLEX_INT3_IRQHandler(void)
{
	while(1){}
}
*/
/*****************************************************************************
** Function name:		FLEX_INT4_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
/*
void FLEX_INT4_IRQHandler(void)
{
	while(1){}
}
*/
/*****************************************************************************
** Function name:		FLEX_INT5_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
/*
void FLEX_INT5_IRQHandler(void)
{
	while(1){}
}
*/
/*****************************************************************************
** Function name:		FLEX_INT6_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
/*
void FLEX_INT6_IRQHandler(void)
{
	while(1){}
}
*/
/*****************************************************************************
** Function name:		FLEX_INT7_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
/*
void FLEX_INT7_IRQHandler(void)
{
	while(1){}
}
*/
/*****************************************************************************
** Function name:		GINT0_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void GINT0_IRQHandler(void)
{
	//gint0_counter++;
	if ( LPC_GPIO_GROUP_INT0->CTRL & BIT0 )
	{
#if 0 // no use
		if ( LPC_GPIO_GROUP_INT0->CTRL & (0x1<<2) )
		{
			gint0_level_counter++;
		}
		else
		{
			gint0_edge_counter++;
		}
#endif
	LPC_GPIO_GROUP_INT0->CTRL |= BIT0;
	}
	
	//g_Repot_Process.Mouse_Report_In = 1;
	
  return;
}

/*****************************************************************************
** Function name:		GINT1_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
**
** Returned value:		None
**
*****************************************************************************/
void GINT1_IRQHandler(void)
{
	//gint1_counter++;
	if ( LPC_GPIO_GROUP_INT1->CTRL & BIT0 )
	{
#if 0
		if ( LPC_GPIO_GROUP_INT1->CTRL & (0x1<<2) )
		{
			gint1_level_counter++;
		}
		else
		{
			gint1_edge_counter++;
		}
#endif
	LPC_GPIO_GROUP_INT1->CTRL |= BIT0;
	}
  return;
}

/*****************************************************************************
** Function name:		nsAPI_GPIO_Init
**
** Descriptions:		Initialize GPIO, install the
**						GPIO interrupt handler
**
** parameters:			None
**
** Returned value:		true or false, return false if the VIC table
**						is full and GPIO interrupt handler can be
**						installed.
**
*****************************************************************************/
#if 0
void nsAPI_GPIO_Init( void )
{
	U8 i,u8GPIOOUTPin[] = {
	LED_DPI_LEVEL_5,
	LED_DPI_LEVEL_4,
	LED_DPI_LEVEL_3,
	LED_DPI_LEVEL_2,
	LED_DPI_LEVEL_1
	};
	
#if (UART_USE_PIO1_13_14 == 0)
	U8 u8GPIOInPin[] = {22,23,24,25,26,27,31,29,13,14,28,15,16};
#else
	U8 u8GPIOInPin[] = {22,23,24,25,26,27,31,29,28,15,16};
#endif



	g_GpioButtonEvent.ButtonStatus = GPIO_SCANKEY_NULL;
	g_GpioButtonEvent.PreQIEStatus = QIE_STOP;

	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT6;

	/* Enable AHB clock to the FlexInt, GroupedInt domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (BIT19| BIT23| BIT24);
	////////////////////
     // NDrv_GPIO_SetDir(PORT0,1,GPIOOUT);/*Test*/
	////////////////////

	
#if 1

	LPC_IOCON->TMS_PIO0_12 &= ~0x07;
	LPC_IOCON->TMS_PIO0_12 |= BIT0; //Func1

	for(i = 0; i < 5; i++){
		NDrv_GPIO_SetDir(PORT0,u8GPIOOUTPin[i],GPIOOUT);/*LED DPI Level 5~1*/
		nsAPI_GPIO_LEDSetting(PORT0,u8GPIOOUTPin[i], 1);
	}

	LPC_IOCON->PIO0_23 &= ~0x07;/*SSP:Motion Pin*/
	LPC_IOCON->PIO0_23 |= BIT4;
	NDrv_GPIO_SetDir(PORT0,23,GPIOIN);/*SSP:Motion Pin*/

	LPC_IOCON->PIO0_21 &= ~0x07;/*ZA*/
	LPC_IOCON->PIO0_21 |= BIT4;
	NDrv_GPIO_SetDir(PORT0,21,GPIOIN);/*SSP:Motion Pin*/

	LPC_IOCON->PIO1_22 &= ~0x07;/*ZB*/
	LPC_IOCON->PIO1_22 |= BIT4;
	LPC_IOCON->PIO1_23 &= ~0x07;/*Key - L*/
	LPC_IOCON->PIO1_23 |= BIT4;
	LPC_IOCON->PIO1_24 &= ~0x07;/*Key - M*/
	LPC_IOCON->PIO1_24 |= BIT4;
	LPC_IOCON->PIO1_25 &= ~0x07;/*Key - R*/
	LPC_IOCON->PIO1_25 |= BIT4;
	LPC_IOCON->PIO1_26 &= ~0x07;/*Key - 4*/
	LPC_IOCON->PIO1_26 |= BIT4;
	LPC_IOCON->PIO1_27 &= ~0x07;/*Key - 5*/
	LPC_IOCON->PIO1_27 |= BIT4;
	LPC_IOCON->PIO1_31 &= ~0x07;/*Key - 6*/ //(Modify)20120516 SW6 use PIN25 PIO1_31
	LPC_IOCON->PIO1_31 |= BIT4;
	LPC_IOCON->PIO1_29 &= ~0x07;/*Key - 7*/ //Factory Key -ReportRate SW7 //(Modify) 20120516 SW7 use PIN31 PIO1_29
	LPC_IOCON->PIO1_29 |= BIT4;
#if (UART_USE_PIO1_13_14 ==0)
	LPC_IOCON->PIO1_13 &= ~0x07;/*Key - 8*/ //Factory Key -LEDPWM SW8
	LPC_IOCON->PIO1_13 |= BIT4;
	LPC_IOCON->PIO1_14 &= ~0x07;/*Key - 9*/ //Factory Key-LED breathing SW9
	LPC_IOCON->PIO1_14 |= BIT4;
#endif
	LPC_IOCON->PIO1_28 &= ~0x07;/*Key - 10*/ //Factory Key -CPI SW10
	LPC_IOCON->PIO1_28 |= BIT4;
	LPC_IOCON->PIO1_15 &= ~0x07;/*Key - 11*/
	LPC_IOCON->PIO1_15 |= BIT4;
	LPC_IOCON->PIO1_16 &= ~0x07;/*Key - 12*/
	LPC_IOCON->PIO1_16 |= BIT4;


#if (UART_USE_PIO1_13_14 ==0)
	for(i = 0; i < 13; i++)
#else
	for(i = 0; i < 11; i++)
#endif
        NDrv_GPIO_SetDir(PORT1,u8GPIOInPin[i],GPIOIN);

#endif

	nsAPI_GPIO_Reset_ADNS9800();		// INIT SPI_1 for ADNS9000 SEL Pin Use

}
#else
void nsAPI_GPIO_Init( void )
{
	g_GpioButtonEvent.ButtonStatus = GPIO_SCANKEY_NULL;
	g_GpioButtonEvent.PreQIEStatus = QIE_STOP;

	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT6;

	/* Enable AHB clock to the FlexInt, GroupedInt domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (BIT19| BIT23| BIT24);
	////////////////////
     // NDrv_GPIO_SetDir(PORT0,1,GPIOOUT);/*Test*/
	////////////////////

#if 0
//#if 0
#ifdef P502
	/*Key - L*/
	//BUTTON_LEFT_PIN_CONFIG &= ~0x07;
	//BUTTON_LEFT_PIN_CONFIG |= BIT4;
	GPIOSetIOConfig(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN, GPIOIN);
	/*Key - M*/ 
	//BUTTON_MIDDLE_PIN_CONFIG &= ~0x07;
	//BUTTON_MIDDLE_PIN_CONFIG |= BIT4 ;
	GPIOSetIOConfig(BUTTON_MIDDLE_PORT, BUTTON_MIDDLE_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_MIDDLE_PORT, BUTTON_MIDDLE_PIN, GPIOIN);
	/*Key - R*/
	//BUTTON_RIGHT_PIN_CONFIG &= ~0x07;
	//BUTTON_RIGHT_PIN_CONFIG |= BIT4;
	GPIOSetIOConfig(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN, GPIOIN);
	/*DPI*/
	//BUTTON_DPI_PIN_CONFIG &= ~0x07;		
	//BUTTON_DPI_PIN_CONFIG |= ( BIT4  | BIT0);
	GPIOSetIOConfig(BUTTON_DPI_PORT, BUTTON_DPI_PIN, 1, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_DPI_PORT, BUTTON_DPI_PIN, GPIOIN);
	/*SIDE 1*/
	//BUTTON_SIDE1_PIN_CONFIG &= ~0x07;	
	//BUTTON_SIDE1_PIN_CONFIG |= BIT4;
	GPIOSetIOConfig(BUTTON_SIDE1_PORT, BUTTON_SIDE1_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_SIDE1_PORT, BUTTON_SIDE1_PIN, GPIOIN);
	/*SIDE 2*/
	//BUTTON_SIDE2_PIN_CONFIG &= ~0x07;	
	//BUTTON_SIDE2_PIN_CONFIG |= BIT4;
	GPIOSetIOConfig(BUTTON_SIDE2_PORT, BUTTON_SIDE2_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_SIDE2_PORT, BUTTON_SIDE2_PIN, GPIOIN);
	/*SIDE 3*/
	//BUTTON_SIDE3_PIN_CONFIG &= ~0x07;	
	//BUTTON_SIDE3_PIN_CONFIG |= BIT4;
	GPIOSetIOConfig(BUTTON_SIDE3_PORT, BUTTON_SIDE3_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_SIDE3_PORT, BUTTON_SIDE3_PIN, GPIOIN);
#else
	/*Key - L*/
	GPIOSetIOConfig(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN, GPIOIN);
	
	/*Key - M*/ 
	GPIOSetIOConfig(BUTTON_MIDDLE_PORT, BUTTON_MIDDLE_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_MIDDLE_PORT, BUTTON_MIDDLE_PIN, GPIOIN);
	
	/*Key - R*/
	GPIOSetIOConfig(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN, GPIOIN);
	
	/*DPI*/
	GPIOSetIOConfig(BUTTON_DPI_PORT, BUTTON_DPI_PIN, 1, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_DPI_PORT, BUTTON_DPI_PIN, GPIOIN);
	
	/*Left SIDE 1*/
	GPIOSetIOConfig(BUTTON_L_SIDE1_PORT, BUTTON_L_SIDE1_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_L_SIDE1_PORT, BUTTON_L_SIDE1_PIN, GPIOIN);
	
	/*Left SIDE 2*/
	GPIOSetIOConfig(BUTTON_L_SIDE2_PORT, BUTTON_L_SIDE2_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_L_SIDE2_PORT, BUTTON_L_SIDE2_PIN, GPIOIN);
	
	/*Right SIDE 1*/
	GPIOSetIOConfig(BUTTON_R_SIDE1_PORT, BUTTON_R_SIDE1_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_R_SIDE1_PORT, BUTTON_R_SIDE1_PIN, GPIOIN);
	
	/*Right SIDE 2*/
	GPIOSetIOConfig(BUTTON_L_SIDE2_PORT, BUTTON_L_SIDE2_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_R_SIDE2_PORT, BUTTON_R_SIDE2_PIN, GPIOIN);
	
#endif

#endif
	R_KeyStatus=0;							// for KeyScan INIT Ram Value
	R_KeyScan_Bounce = V_KeyScan_Bounce;	// INIT KeyScan Bounce

}
#endif

/*****************************************************************************
** Function name:		NDrv_GPIO_SetBitValue
**
** Descriptions:		Set/clear a bit in a specific position
**
** parameters:			port num, bit position, bit value
**
** Returned value:		None
**
*****************************************************************************/
void NDrv_GPIO_SetBitValue( U8 portNum, U8 bitPosi, U8 bitVal )
{
  #if 1
	if(bitVal)
	LPC_GPIO->SET[portNum] = 1 << bitPosi;
	else
	LPC_GPIO->CLR[portNum] = 1 << bitPosi;
 #else
	U32 regVal = 0;
 	regVal = LPC_GPIO->PIN[portNum];
	if (bitVal)
	{
	 	LPC_GPIO->PIN[portNum] = regVal | (1<<bitPosi);
	}
	else
	{
		LPC_GPIO->PIN[portNum] = regVal  & ~ (1<<bitPosi);
	}
 #endif
}

/*****************************************************************************
** Function name:		GPIOGetPinValue
**
** Descriptions:		Read Current state of port pin, PIN register value
**
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/

RetU32 NDrv_GPIO_GetPinValue( U8 portNum, U32 bitPosi )
{
	U32 regVal = 0;

	if(bitPosi == 0xFF)
	{
		regVal = LPC_GPIO->PIN[portNum];
	}
	else
	{
		//regVal = LPC_GPIO->PIN[portNum];
		//regVal=1<<bitPosi;
		if(LPC_GPIO->PIN[portNum] &  (1<<bitPosi))
		regVal = 1;
	}

	return (regVal);
}

/*****************************************************************************
** Function name:		NDrv_GPIO_SetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			portNum, bit position, direction (1 out, 0 input)
**
** Returned value:		None
**
*****************************************************************************/
void NDrv_GPIO_SetDir(U8 portNum, U8 bitPosi, U8 dir )
{
	if(dir)
	LPC_GPIO->DIR[portNum] |= (1 << bitPosi);
	else
	LPC_GPIO->DIR[portNum] &= ~(1 << bitPosi);
}

void nsAPI_GPIO_LEDSetting(U8 uPort, U32 uPosition, U8 uValue)
{
	NDrv_GPIO_SetBitValue(uPort,uPosition,uValue);
}

#if 0
void nsAPI_GPIO_KeyboardEventMointor(void)
{
	//U8 u8Temp;
	g_GpioKeyboard.ButtonStatus = GPIO_KEY_SAME;
	//GPIO_Check_Key();
	S_GPIO_Check_Key();	// IO Port 1

#if 0
	S_Scroll_Polling();		// IO Port 0		//2016_0407_Hsiang
#else
	Wheel_Polling();
#endif

	if(g_GpioKeyboard.ButtonStatus == GPIO_KEY_CHANGED)
	{
		g_StaticMachineEvent2Type = SM_EVENT2_BUTTON_HANDLER;
	}

	#ifdef V_SleepMode_Pin_Debug
	if(F_Togle)
	{
		F_Togle= 0x0;
		nsAPI_GPIO_LEDSetting(V_DPI_2_LED_Port,V_DPI_2_LED_Pin, V_LED_Active);		// LED Off				
	}
	else
	{
		F_Togle =0xff;
		nsAPI_GPIO_LEDSetting(V_DPI_2_LED_Port,V_DPI_2_LED_Pin, !V_LED_Active);	// LED On					
	}				
	#endif
	
	//u8Temp = nsAPI_GPIO_Get_QIE();
	//if(u8Temp != QIE_SAME)
	//g_StaticMachineEvent2Type = SM_EVENT2_BUTTON_HANDLER;
	
}
#else
#endif
#if 0
void nsAPI_GPIO_ButtonEventMointor(void)
{
	g_GpioKeyboard.ButtonStatus = GPIO_KEY_SAME;
	S_GPIO_Check_Key();	// IO Port 1
	Wheel_Polling();

	if(g_GpioKeyboard.ButtonStatus == GPIO_KEY_CHANGED)
	{
		g_StaticMachineEvent2Type = SM_EVENT2_BUTTON_HANDLER;
	}
	
}
#endif


void GPIO_Check_Key(void)
{
	U32 u32ButtonGPIO1 = 0;
	U8 Key_Loop;
	U8 GPIOPShift[] = {23,24,25,26,27,29,31,13,14,28,15,16};
	//U8 Ret = FALSE;
	//xprintf("Check_Key",0);
	
	u32ButtonGPIO1 = NDrv_GPIO_GetPinValue(PORT1,0xFF);
	
#if 0
       for(Key_Loop = 0; Key_Loop < 12;Key_Loop++)
       {
       
    		if(u32ButtonGPIO1 & (1UL << (GPIOPShift[Key_Loop])))
			g_Key_Matrix[Key_Loop].PHY_Status = 1;
		else
		    	g_Key_Matrix[Key_Loop].PHY_Status = 0;
		
       }
//#else
	if(u32ButtonGPIO1 & BIT23)
	{
		g_Key_Matrix[0].PHY_Status = 1;//BIT23;		//M L
	}
	else
	{
		g_Key_Matrix[0].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT24){
		g_Key_Matrix[1].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[1].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT25){
		g_Key_Matrix[2].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[2].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT26){
		g_Key_Matrix[3].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[3].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT27){
		g_Key_Matrix[4].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[4].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT29){
		g_Key_Matrix[5].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[5].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT31){
		g_Key_Matrix[6].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[6].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT13){
		g_Key_Matrix[7].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[7].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT14){
		g_Key_Matrix[8].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[8].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT28){
		g_Key_Matrix[9].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[9].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT15){
		g_Key_Matrix[10].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[10].PHY_Status = 0;//BIT23;		//M L
	}

	if(u32ButtonGPIO1 & BIT16){
		g_Key_Matrix[11].PHY_Status = 1;//BIT23;		//M L
	}
	else{
		g_Key_Matrix[11].PHY_Status = 0;//BIT23;		//M L
	}
#endif


	for(Key_Loop = 0; Key_Loop < 12; Key_Loop++)
	{
		
		#if UART_USE_PIO1_13_14
		
		if(Key_Loop == 7)
			continue;
		if(Key_Loop == 8)
			continue;
		
		#endif
		
		if(g_Key_Matrix[Key_Loop].Press==1)
		{
			
			#if (ENABLE_UART)

			xprintf("\r\n Press:%d",Key_Loop);
			
			#endif
			
			if(g_Key_Matrix[Key_Loop].PHY_Status)
            		{
                		if(g_Key_Matrix[Key_Loop].Debounce > 0)
                		{
				    //g_Key_Matrix[Key_Loop].Debounce -= 1; //use Systick
                		}
                		else
                		{
    					g_Key_Matrix[Key_Loop].Press = 0;
    					g_Key_Matrix[Key_Loop].Changed = 1;
    					g_GpioKeyboard.ButtonStatus = GPIO_KEY_CHANGED;
    					g_Key_Matrix[Key_Loop].Debounce = KEY_DEBOUNCE;
                		}
			}
            		else
            		{
            			#if 0
						
                		if(g_Key_Matrix[Key_Loop].Debounce > 0)
                		{
				    //g_Key_Matrix[Key_Loop].Debounce -= 1; //use Systick
			    	}
                		else
                		#endif
                		{
                    			g_Key_Matrix[Key_Loop].Debounce = KEY_DEBOUNCE;
                		}
            		}
		}
		else
		{
			#if (ENABLE_UART)
			
			//xprintf("\r\n UnPress:%d",Key_Loop);
			
			#endif

		    	if(!g_Key_Matrix[Key_Loop].PHY_Status)
            		{
                		if(g_Key_Matrix[Key_Loop].Debounce > 0)
                		{
				    //g_Key_Matrix[Key_Loop].Debounce -= 1; //use Systick
			    	}
                		else
                		{
					g_Key_Matrix[Key_Loop].Press = 1;
					g_Key_Matrix[Key_Loop].Changed = 1;
					g_GpioKeyboard.ButtonStatus = GPIO_KEY_CHANGED;
					g_Key_Matrix[Key_Loop].Debounce = KEY_DEBOUNCE;
			    	}
		   	 }
            		else
            		{
                		#if 0
						
                		if(g_Key_Matrix[Key_Loop].Debounce > 0)
                		{
				    //g_Key_Matrix[Key_Loop].Debounce -= 1; //use Systick
			    	}
                		else
                		#endif
                		{
                    			g_Key_Matrix[Key_Loop].Debounce = KEY_DEBOUNCE;
                		}
            		}
		}

		
	}
	//return Ret;

	
}





#if 0 // no use
void nsAPI_GPIO_Keyboard_Macro_Engine(void)
{
	switch(g_GpioButtonEvent.tButtonEvent.tLGDTBS.eMouseButtonGroupStatusMonitor)
	{
		case eKEYBOARD_INIT:
		g_GpioButtonEvent.tButtonEvent.tLGDTBS.eMouseButtonGroupStatusMonitor = eKEYBOARD_BUSY;
		g_GpioButtonEvent.tButtonEvent.tLGDTBS.eMouseButtonGroupSystickMonitor = eSYSTICK_MONITOR;

		g_GpioButtonEvent.tButtonEvent.tLGDTBS.tMouseButtonGroupTempDelay =\
		g_GpioButtonEvent.tButtonEvent.gdbMouseButtonGroupReleaseDelyTimes * 5;/*x50ms*/;
		break;

		case eKEYBOARD_FINISH:
		g_StaticMachineEvent2Type = 	SM_EVENT2_KEYBOARD_HANDLER;
		g_GpioButtonEvent.ButtonStatus = GPIO_SCANKEY_FW_RELEASE;

		g_GpioButtonEvent.tButtonEvent.tLGDTBS.eMouseButtonGroupSystickMonitor = eSYSTICK_IDLE;
		g_GpioButtonEvent.tButtonEvent.tLGDTBS.eMouseButtonGroupStatusMonitor = eKEYBOARD_NEXT;
		break;

		case eKEYBOARD_NEXT:
		if(g_GpioButtonEvent.tButtonEvent.gdbMouseButtonGroupTotalData !=\
		g_GpioButtonEvent.tButtonEvent.gdbMouseButtonGroupTotalDataIndex)
		{
			U8 u8GroupIndex,u8ButtonIndex;

			u8GroupIndex = nsAPI_Database_Button_Get_ProfileGroup_Index();
			u8ButtonIndex= nsAPI_GPIO_Button_Change(g_GpioButtonEvent.PreButtonEvent);

			if(u8GroupIndex >= ePROFILE_SIZE\
			||u8ButtonIndex >= eMOUSE_BUTTON_INDEX_SIZE)
			{
				//xprintf("\r\n GPIP_ChangingIndex erroe",0);
				return;
			}

			g_GpioButtonEvent.tButtonEvent = \
			nsAPI_GPIO_Button_Get_ProfileKey(u8GroupIndex,u8ButtonIndex,\
			g_GpioButtonEvent.tButtonEvent.gdbMouseButtonGroupTotalDataIndex+1);

			g_GpioButtonEvent.tButtonEvent.tLGDTBS.eMouseButtonGroupStatusMonitor = eKEYBOARD_INIT;

			g_GpioButtonEvent.ButtonStatus = GPIO_SCANKEY_PRH;
			g_StaticMachineEvent2Type = SM_EVENT2_KEYBOARD_HANDLER;
		}
		else
		{
			if(g_GpioButtonEvent.tButtonEvent.gdbMouseButtonGroupRecycle > 0 &&\
			g_GpioButtonEvent.bRecycleFlag == FALSE)
			{
				g_GpioButtonEvent.bRecycleFlag = TRUE;
				g_GpioButtonEvent.ButtonStatus = GPIO_SCANKEY_PRH;
				nsAPI_GPIO_ChangingIndex(g_GpioButtonEvent.PreButtonEvent,GIdx0);

				g_StaticMachineEvent2Type = SM_EVENT2_KEYBOARD_HANDLER;

			}
			else
			{

				g_StaticMachineEvent1Type = 	SM_EVENT1_MOUSE_MONITOR;
				g_GpioButtonEvent.bRecycleFlag = FALSE;
			}
			g_GpioButtonEvent.tButtonEvent.tLGDTBS.eMouseButtonGroupStatusMonitor =\
			eKEYBOARD_INIT;
		}
		break;

		case eKEYBOARD_IDLE:
		case eKEYBOARD_BUSY:
		default:
		break;
	}
}


void nsAPI_GPIO_Reset_ADNS9800(void)
{
	//LPC_IOCON->PIO1_19 &= ~0x07;		// Select for PIO1_19
	//LPC_IOCON->PIO1_19 |= BIT4;			// Select Pull_Up
	GPIO_FUNC_MODE(LPC_IOCON->PIO0_2, GPIO_PULL_UP);	//SSP0_SSEL
	NDrv_GPIO_SetDir(PORT0,2,GPIOOUT);
	//NDrv_GPIO_SetBitValue(PORT0,2,HIGH);
	
	NDrv_SSP_ADNS9800_SSEL(LOW);
	nsAPI_SSP_Sensor_Delsyus(50);/*50us*/
}
#endif
//(Modify) 20120516 forward have sometime 2 steps
#if 0
U8 nsAPI_GPIO_Get_QIE(void)
{
	U8 u8Ret = QIE_SAME;
	U32 u32Temp;
	if(QIEAction.QIE_DEBOUNCE > 0){
		QIEAction.QIE_DEBOUNCE -= 1;
		return QIE_SAME;
	}
	
	u32Temp = NDrv_GPIO_GetPinValue(PORT0,0xFF);
	u32Temp = ((u32Temp & (BIT22|BIT21)) >> 21) & 0x03;

	if(g_GpioButtonEvent.PreQIEStatus != u32Temp)
	{
	    //xprintf("\r\n u32Temp=%x",u32Temp);
	    //xprintf("\r\n PreQIEStatus=%x",g_GpioButtonEvent.PreQIEStatus);
		QIEAction.QIE_DEBOUNCE = 50;//200
		switch(g_GpioButtonEvent.PreQIEStatus)
		{
			case 0x00:
				switch(u32Temp)
				{
					case 0x00:
						u8Ret = QIE_STOP;
					break;

					case 0x01:
						if(QIEAction.QIE_DIR == QIE_FORWARD){
							u8Ret = QIE_FORWARD; //(Modify) 20120516 forward have sometime 2 steps , modify 20120808
							QIEAction.QIE_DIR = QIE_STOP;
						}
						else{
							QIEAction.QIE_DIR = QIE_FORWARD;
						}
					break;
					case 0x02:
						if(QIEAction.QIE_DIR == QIE_BACKWARD){
							//u8Ret = QIE_BACKWARD; //(Modify) 20120516 forward have sometime 2 steps
							QIEAction.QIE_DIR = QIE_STOP;
						}
						else{
							QIEAction.QIE_DIR = QIE_BACKWARD;
						}
					break;
				}
			break;

			case 0x01:
				switch(u32Temp)
				{
					case 0x00:
						if(QIEAction.QIE_DIR == QIE_BACKWARD){
							u8Ret = QIE_BACKWARD;
							QIEAction.QIE_DIR = QIE_STOP;
						}
						else{
							QIEAction.QIE_DIR = QIE_BACKWARD;
						}
					break;

					case 0x01:
						u8Ret = QIE_STOP;
					break;

					case 0x03:
						if(QIEAction.QIE_DIR == QIE_FORWARD){
							u8Ret = QIE_FORWARD;
							QIEAction.QIE_DIR = QIE_STOP;
						}
						else{
							QIEAction.QIE_DIR = QIE_FORWARD;
						}
					break;
				}
			break;

			case 0x02:
				switch(u32Temp)
				{
					case 0x00:
						if(QIEAction.QIE_DIR == QIE_FORWARD){
							u8Ret = QIE_FORWARD;
							QIEAction.QIE_DIR = QIE_STOP;
						}
						else{
							QIEAction.QIE_DIR = QIE_FORWARD;
						}
					break;

					case 0x02:
						if(QIEAction.QIE_DIR == QIE_BACKWARD){
							u8Ret = QIE_BACKWARD;
							QIEAction.QIE_DIR = QIE_STOP;
						}
						else{
							QIEAction.QIE_DIR = QIE_BACKWARD;
						}
					break;

					case 0x03:
						u8Ret = QIE_BACKWARD;
					break;
				}
			break;

			case 0x03:
				switch(u32Temp)
				{
					case 0x01:
					if(QIEAction.QIE_DIR == QIE_BACKWARD){
						u8Ret = QIE_BACKWARD;
						QIEAction.QIE_DIR = QIE_STOP;
					}
					else{
						QIEAction.QIE_DIR = QIE_BACKWARD;
					}
					break;

					case 0x02:
					if(QIEAction.QIE_DIR == QIE_FORWARD){
						u8Ret = QIE_FORWARD;
						QIEAction.QIE_DIR = QIE_STOP;
					}
					else{
						QIEAction.QIE_DIR = QIE_FORWARD;
					}
					break;

					case 0x03: u8Ret = QIE_STOP;
					break;
				}
			break;
		}
		g_GpioButtonEvent.PreQIEStatus = u32Temp;
	}

	QIEAction.QIE_STATUS= u8Ret;
	switch(u8Ret){
		case QIE_BACKWARD:
			g_Key_Matrix[12].Press = 1;
			g_Key_Matrix[12].Changed = 1;
		break;
		case QIE_FORWARD:
			g_Key_Matrix[13].Press = 1;
			g_Key_Matrix[13].Changed = 1;
		break;
	}
	return u8Ret;
}


void nsAPI_GPIO_LedDisable(BOOLEAN bEnable)
{
	U8 i,j,LED_DPI_LEVEL_GPIO[] = {
			LED_DPI_LEVEL_5,
			LED_DPI_LEVEL_4,
			LED_DPI_LEVEL_3,
			LED_DPI_LEVEL_2,
			LED_DPI_LEVEL_1
		};

	if(bEnable)
		i = 0;
	else
		i = 1;

	for(j = 0; j < 5; j++)
		nsAPI_GPIO_LEDSetting(PORT0,LED_DPI_LEVEL_GPIO[j], i);
}
#endif

#if 0
void nsAPI_PWM_ChangeTo_GPIO(void)
{
#if (UART_USE_PIO0_18_19 ==0)

	U8 i, u8GPIOInPin[] = {8,9,10,1,18,19,13,14,15};
#else
    U8 i, u8GPIOInPin[] = {8,9,10,1,13,14,15};
#endif
	//Timer16_0
	LPC_IOCON->PIO0_8 &=~0x1F;
	LPC_IOCON->PIO0_8 |= 0x10;
	LPC_IOCON->PIO0_9 &=~0x1F;
	LPC_IOCON->PIO0_9 |= 0x10;
	LPC_IOCON->SWCLK_PIO0_10 &= ~0x1F;
	LPC_IOCON->SWCLK_PIO0_10 |= 0x11;

	//Timer32_0
	LPC_IOCON->PIO0_1 &= ~0x1F;
	LPC_IOCON->PIO0_1 |= 0x10;
    #if (UART_USE_PIO0_18_19 ==0)
	LPC_IOCON->PIO0_18&= ~0x1F;
	LPC_IOCON->PIO0_18 |= 0x10;
	LPC_IOCON->PIO0_19 &= ~0x1F;
	LPC_IOCON->PIO0_19 |= 0x10;
    #endif
	//Timer32_1
	LPC_IOCON->TDO_PIO0_13 &= ~0x1F;
	LPC_IOCON->TDO_PIO0_13 |= 0x11;
	LPC_IOCON->TRST_PIO0_14&= ~0x1F;
	LPC_IOCON->TRST_PIO0_14 |= 0x11;
	LPC_IOCON->SWDIO_PIO0_15 &= ~0x1F;
	LPC_IOCON->SWDIO_PIO0_15 |= 0x11;

	for(i = 0; i < 9; i++){
		NDrv_GPIO_SetDir(PORT0,u8GPIOInPin[i],GPIOOUT);
		nsAPI_GPIO_LEDSetting(PORT0,u8GPIOInPin[i], 0);
	}
}
#endif
#if 0
/*****************************************************************************
** Function name:		GPIOSetGroupedInterrupt
**
** Descriptions:		Set interrupt logic, sense, eventPattern, etc.
**						logic: AND or OR, 0 is OR, 1 is AND
**						sensePattern: edge or level, 0 is edge, 1 is level
**						event/polarity: 0 is active low/falling, 1 is high/rising.
**
** parameters:			group #, bit pattern, logic, sense, event(polarity) pattern
**
** Returned value:		None
**
*****************************************************************************/
void GPIOSetGroupedInterrupt( uint32_t groupNum, uint32_t *bitPattern, uint32_t logic, uint32_t sense, uint32_t *eventPattern )
{
#if 1
	LPC_GPIO_GROUP_INTx_Type * LPC_GPIO_GROUP_INTx_BASE = NULL;
	IRQn_Type u8GINTx_IRQn;

	switch(groupNum)
	{
		case GROUP0: LPC_GPIO_GROUP_INTx_BASE = LPC_GPIO_GROUP_INT0;
		u8GINTx_IRQn = GINT0_IRQn;
		break;
		case GROUP1: LPC_GPIO_GROUP_INTx_BASE = LPC_GPIO_GROUP_INT1;
		u8GINTx_IRQn = GINT1_IRQn;
		break;
		default:
		return;
	}

	if(sense == 0)
	{
		LPC_GPIO_GROUP_INTx_BASE->CTRL &= ~(BIT2);/* Edge trigger */
	}
	else
	{
		LPC_GPIO_GROUP_INTx_BASE->CTRL |= (BIT2);/* Level trigger */
	}
	LPC_GPIO_GROUP_INTx_BASE->CTRL |= (logic << 1);
	LPC_GPIO_GROUP_INTx_BASE->PORT_POL[0] = *((U32 *)(eventPattern + 0));
	LPC_GPIO_GROUP_INTx_BASE->PORT_POL[1] = *((U32 *)(eventPattern + 1));
	LPC_GPIO_GROUP_INTx_BASE->PORT_ENA[0] = *((U32 *)(bitPattern + 0));
	LPC_GPIO_GROUP_INTx_BASE->PORT_ENA[1] = *((U32 *)(bitPattern + 1));
	/* as soon as enabled, an edge may be generated       */
	/* clear interrupt flag and NVIC pending interrupt to */
	/* workaround the potential edge generated as enabled */
	LPC_GPIO_GROUP_INTx_BASE->CTRL |= BIT0;
	NVIC_ClearPendingIRQ(u8GINTx_IRQn);
	NVIC_EnableIRQ(u8GINTx_IRQn);
#else
	switch(groupNum)
	{
		case GROUP0:
		if(sense == 0)
		{
			LPC_GPIO_GROUP_INT0->CTRL &= ~(BIT2);/* Edge trigger */
		}
		else
		{
			LPC_GPIO_GROUP_INT0->CTRL |= (BIT2);/* Level trigger */
		}
		LPC_GPIO_GROUP_INT0->CTRL |= (logic << 1);
		LPC_GPIO_GROUP_INT0->PORT_POL[0] = *((U32 *)(eventPattern + 0));
		LPC_GPIO_GROUP_INT0->PORT_POL[1] = *((U32 *)(eventPattern + 1));
		LPC_GPIO_GROUP_INT0->PORT_ENA[0] = *((U32 *)(bitPattern + 0));
		LPC_GPIO_GROUP_INT0->PORT_ENA[1] = *((U32 *)(bitPattern + 1));
		/* as soon as enabled, an edge may be generated       */
		/* clear interrupt flag and NVIC pending interrupt to */
		/* workaround the potential edge generated as enabled */
		LPC_GPIO_GROUP_INT0->CTRL |= BIT0;
		NVIC_ClearPendingIRQ(GINT0_IRQn);
		NVIC_EnableIRQ(GINT0_IRQn);
		break;

		case GROUP1:
		if(sense == 0)
		{
			LPC_GPIO_GROUP_INT1->CTRL &= ~BIT2;	/* Edge trigger */
		}
		else
		{
			LPC_GPIO_GROUP_INT1->CTRL |= BIT2;	/* Level trigger. */
		}
		LPC_GPIO_GROUP_INT1->CTRL |= (logic << 1);
		LPC_GPIO_GROUP_INT1->PORT_POL[0] = *((U32 *)(eventPattern + 0));
		LPC_GPIO_GROUP_INT1->PORT_POL[1] = *((U32 *)(eventPattern + 1));
		LPC_GPIO_GROUP_INT1->PORT_ENA[0] = *((U32 *)(bitPattern + 0));
		LPC_GPIO_GROUP_INT1->PORT_ENA[1] = *((U32 *)(bitPattern + 1));
		/* as soon as enabled, an edge may be generated       */
		/* clear interrupt flag and NVIC pending interrupt to */
		/* workaround the potential edge generated as enabled */
		LPC_GPIO_GROUP_INT1->CTRL |= BIT0;
		NVIC_ClearPendingIRQ(GINT1_IRQn);
		NVIC_EnableIRQ(GINT1_IRQn);
		break;
		default:
		break;
	}

#endif
}
void ALL_GPIO_INIT_CONFIG(void)
{

	
	GPIOSetIOConfig(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN, GPIOIN);
	
	GPIOSetIOConfig(BUTTON_MIDDLE_PORT, BUTTON_MIDDLE_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_MIDDLE_PORT, BUTTON_MIDDLE_PIN, GPIOIN);
	
	GPIOSetIOConfig(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN, GPIOIN);

	GPIOSetIOConfig(BUTTON_DPI_PORT, BUTTON_DPI_PIN, 1, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_DPI_PORT, BUTTON_DPI_PIN, GPIOIN);
#ifdef P502
	GPIOSetIOConfig(BUTTON_SIDE1_PORT, BUTTON_SIDE1_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_SIDE1_PORT, BUTTON_SIDE1_PIN, GPIOIN);

	GPIOSetIOConfig(BUTTON_SIDE2_PORT, BUTTON_SIDE2_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_SIDE2_PORT, BUTTON_SIDE2_PIN, GPIOIN);

	GPIOSetIOConfig(BUTTON_SIDE3_PORT, BUTTON_SIDE3_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_SIDE3_PORT, BUTTON_SIDE3_PIN, GPIOIN);
#else
	GPIOSetIOConfig(BUTTON_L_SIDE1_PORT, BUTTON_L_SIDE1_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_L_SIDE1_PORT, BUTTON_L_SIDE1_PIN, GPIOIN);

	GPIOSetIOConfig(BUTTON_L_SIDE2_PORT, BUTTON_L_SIDE2_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_L_SIDE2_PORT, BUTTON_L_SIDE2_PIN, GPIOIN);

	GPIOSetIOConfig(BUTTON_R_SIDE1_PORT, BUTTON_R_SIDE1_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_R_SIDE1_PORT, BUTTON_R_SIDE1_PIN, GPIOIN);

	GPIOSetIOConfig(BUTTON_R_SIDE2_PORT, BUTTON_R_SIDE2_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(BUTTON_R_SIDE2_PORT, BUTTON_R_SIDE2_PIN, GPIOIN);
#endif

	GPIOSetIOConfig(WHEEL_ZA_PORT, WHEEL_ZA_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(WHEEL_ZA_PORT, WHEEL_ZA_PIN, GPIOIN);			// ZA Set as Input

	GPIOSetIOConfig(WHEEL_ZB_PORT, WHEEL_ZB_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(WHEEL_ZB_PORT, WHEEL_ZB_PIN, GPIOIN);			// ZB Set as Input
	


}
#endif
#if 0
/*****************************************************************************
** Function name:		GPIOSetIOConfig
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			portNum, bit position, direction (1 out, 0 input)
** Returned value:		None
**
*****************************************************************************/
void GPIOSetIOConfig(uint8_t io_port, uint8_t io_num, uint8_t io_fun, uint8_t io_mode)
{
  if(io_port == PORT0)
  {
    switch(io_num)
    {
      case 0 :
      	LPC_IOCON->RESET_PIO0_0 = (io_fun | io_mode);  
        break;	 	

      case 1 :
      	LPC_IOCON->PIO0_1 = (io_fun | io_mode);    
        break;	 	
        
      case 2 :
      	LPC_IOCON->PIO0_2 = (io_fun | io_mode);    
        break;	 	        

      case 3 :
      	LPC_IOCON->PIO0_3 = (io_fun | io_mode);    
        break;	 	        

      case 4 :
      	LPC_IOCON->PIO0_4 = (io_fun | io_mode);  
        break;	 	        

      case 5 :
      	LPC_IOCON->PIO0_5 = (io_fun | io_mode);  
        break;	 	        

      case 6 :
      	LPC_IOCON->PIO0_6 = (io_fun | io_mode);  
        break;	 	

      case 7 :
      	LPC_IOCON->PIO0_7 = (io_fun | io_mode);  
        break;	 	
        
      case 8 :
      	LPC_IOCON->PIO0_8 = (io_fun | io_mode);  
        break;	 	        

      case 9 :
      	LPC_IOCON->PIO0_9 = (io_fun | io_mode);  
        break;	 	        

      case 10 :
      	LPC_IOCON->SWCLK_PIO0_10 = (io_fun | io_mode);  
        break;	 	        

      case 11 :
      	LPC_IOCON->TDI_PIO0_11 = (io_fun | io_mode) | IOCON_DIGMODE_EN;    
        break;	 	        
        
      case 12 :
      	LPC_IOCON->TMS_PIO0_12 = (io_fun | io_mode) | IOCON_DIGMODE_EN;    
        break;	 	

      case 13 :
      	LPC_IOCON->TDO_PIO0_13 = (io_fun | io_mode) | IOCON_DIGMODE_EN;    
        break;	 	
        
      case 14 :
      	LPC_IOCON->TRST_PIO0_14 = (io_fun | io_mode) | IOCON_DIGMODE_EN;    
        break;	 	        

      case 15 :
      	LPC_IOCON->SWDIO_PIO0_15 = (io_fun | io_mode) | IOCON_DIGMODE_EN;    
        break;	 	        

      case 16 :
      	LPC_IOCON->PIO0_16 = (io_fun | io_mode) | IOCON_DIGMODE_EN;   
        break;	 	        

      case 17 :
      	LPC_IOCON->PIO0_17 = (io_fun | io_mode);  
        break;	 	        
        
      case 18 :
      	LPC_IOCON->PIO0_18 = (io_fun | io_mode);  
        break;	 	

      case 19 :
      	LPC_IOCON->PIO0_19 = (io_fun | io_mode);  
        break;	 	
        
      case 20 :
      	LPC_IOCON->PIO0_20 = (io_fun | io_mode);  
        break;	 	        

      case 21 :
      	LPC_IOCON->PIO0_21 = (io_fun | io_mode);  
        break;	 	        

      case 22 :
      	LPC_IOCON->PIO0_22 = (io_fun | io_mode) | IOCON_DIGMODE_EN;    
        break;	 	        

      case 23 :
      	LPC_IOCON->PIO0_23 = (io_fun | io_mode) | IOCON_DIGMODE_EN;    
        break;	 	        
                            	
    	default :
    		break;   	
    }
  }
  else if(io_port == PORT1)
  {
	  switch(io_num)
		{
      case 0 :
      	LPC_IOCON->PIO1_0 = (io_fun | io_mode);  
        break;	 	

      case 1 :
      	LPC_IOCON->PIO1_1 = (io_fun | io_mode);  
        break;	 	
        
      case 2 :
      	LPC_IOCON->PIO1_2 = (io_fun | io_mode);  
        break;	 	        

      case 3 :
      	LPC_IOCON->PIO1_3 = (io_fun | io_mode);  
        break;	 	        

      case 4 :
      	LPC_IOCON->PIO1_4 = (io_fun | io_mode);  
        break;	 	        

      case 5 :
      	LPC_IOCON->PIO1_5 = (io_fun | io_mode);  
        break;	 	        

      case 6 :
      	LPC_IOCON->PIO1_6 = (io_fun | io_mode);  
        break;	 	

      case 7 :
      	LPC_IOCON->PIO1_7 = (io_fun | io_mode);  
        break;	 	
        
      case 8 :
      	LPC_IOCON->PIO1_8 = (io_fun | io_mode);  
        break;	 	        

      case 9 :
      	LPC_IOCON->PIO1_9 = (io_fun | io_mode);  
        break;	 	        

      case 10 :
      	LPC_IOCON->PIO1_10 = (io_fun | io_mode);  
        break;	 	        

      case 11 :
      	LPC_IOCON->PIO1_11 = (io_fun | io_mode);  
        break;	 	        
        
      case 12 :
      	LPC_IOCON->PIO1_12 = (io_fun | io_mode);  
        break;	 	

      case 13 :
      	LPC_IOCON->PIO1_13 = (io_fun | io_mode);  
        break;	 	
        
      case 14 :
      	LPC_IOCON->PIO1_14 = (io_fun | io_mode);  
        break;	 	        

      case 15 :
      	LPC_IOCON->PIO1_15 = (io_fun | io_mode);  
        break;	 	        

      case 16 :
      	LPC_IOCON->PIO1_16 = (io_fun | io_mode);  
        break;	 	        

      case 17 :
      	LPC_IOCON->PIO1_17 = (io_fun | io_mode);  
        break;	 	        
        
      case 18 :
      	LPC_IOCON->PIO1_18 = (io_fun | io_mode);  
        break;	 	

      case 19 :
      	LPC_IOCON->PIO1_19 = (io_fun | io_mode);  
        break;	 	
        
      case 20 :
      	LPC_IOCON->PIO1_20 = (io_fun | io_mode);  
        break;	 	        

      case 21 :
      	LPC_IOCON->PIO1_21 = (io_fun | io_mode);  
        break;	 	        

      case 22 :
      	LPC_IOCON->PIO1_22 = (io_fun | io_mode);  
        break;	 	        

      case 23 :
      	LPC_IOCON->PIO1_23 = (io_fun | io_mode);  
        break;	 	        

      case 24 :
      	LPC_IOCON->PIO1_24 = (io_fun | io_mode);  
        break;	 	

      case 25 :
      	LPC_IOCON->PIO1_25 = (io_fun | io_mode);  
        break;	 	
        
      case 26 :
      	LPC_IOCON->PIO1_26 = (io_fun | io_mode);  
        break;	 	        

      case 27 :
      	LPC_IOCON->PIO1_27 = (io_fun | io_mode);  
        break;	 	        

      case 28 :
      	LPC_IOCON->PIO1_28 = (io_fun | io_mode);  
        break;	 	        

      case 29 :
      	LPC_IOCON->PIO1_29 = (io_fun | io_mode);  
        break;	 	        

      case 30 :
      	LPC_IOCON->PIO1_30 = (io_fun | io_mode);  
        break;	 	

      case 31 :
      	LPC_IOCON->PIO1_31 = (io_fun | io_mode);  
        break;	 	
                            	
    	default :
    		break;   	  	
    }	
  }
} 
#endif
#endif

