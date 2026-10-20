/******************************************************************************
 * @file     system_LPC11Uxx.c
 * @purpose  CMSIS Cortex-M3 Device Peripheral Access Layer Source File
 *           for the NXP LPC13xx Device Series
 * @version  V1.10
 * @date     24. November 2010
 *
 * @note
 * Copyright (C) 2009-2010 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/
#define __SYSTEM_LPC11Uxx_C__

#include "system_LPC11Uxx.h"
#include "Mouse.h"
#include "MouseDatabase.h"
#include "LPC11Uxx.h"
#include "Timer16.h"
#include "timer32.h"
#include "Uart.h"
#include "Gpio.h"
#include "Usbhw.h"
#include "Systick.h"
#include "Ssp.h"
#include "I2c.h"

#include "wheel.h"

extern  LPC11Uxx_DATABASE_MOUSE_GENSETTING g_dMouse;
void System_Config_Init(void)
{
	U8 i;

	LPC_SYSCON->PDRUNCFG &= ~BIT5;/*Power-up System Osc*/
	LPC_SYSCON->SYSOSCCTRL = SYSOSCCTRL_Val;
	for (i = 0; i < 200; i++) __NOP();

	LPC_SYSCON->SYSPLLCLKSEL = SYSPLLCLKSEL_Val;/*Select PLL Input*/
	LPC_SYSCON->SYSPLLCLKUEN = 0x01;/*Update Clock Source*/
	LPC_SYSCON->SYSPLLCLKUEN = 0x00;/*Toggle Update Register*/
	LPC_SYSCON->SYSPLLCLKUEN = 0x01;
	while(!(LPC_SYSCON->SYSPLLCLKUEN & BIT0)){}/*Wait Until Updated*/

	LPC_SYSCON->SYSPLLCTRL = SYSPLLCTRL_Val;/*Main Clock is PLL Out*/
	LPC_SYSCON->PDRUNCFG &= ~BIT7;/*Power-up SYSPLL*/
	while(!(LPC_SYSCON->SYSPLLSTAT & BIT0)){}/*Wait Until PLL Locked*/

	LPC_SYSCON->MAINCLKSEL = MAINCLKSEL_Val;/*Select PLL Clock Output*/
	LPC_SYSCON->MAINCLKUEN = 0x01;/*Update MCLK Clock Source*/
	LPC_SYSCON->MAINCLKUEN = 0x00;/*Toggle Update Register*/
	LPC_SYSCON->MAINCLKUEN = 0x01;
	while (!(LPC_SYSCON->MAINCLKUEN & BIT0)){}/*Wait Until Updated*/

	LPC_SYSCON->SYSAHBCLKDIV  = SYSAHBCLKDIV_Val;
	LPC_SYSCON->PDRUNCFG &= ~BIT10;/* Power-up USB PHY*/
	LPC_SYSCON->PDRUNCFG &= ~BIT8;/* Power-up USB PLL*/
	LPC_SYSCON->USBPLLCLKSEL = USBPLLCLKSEL_Val;/* Select PLL Input*/
	LPC_SYSCON->USBPLLCLKUEN = 0x01;/*Update Clock Source*/
	LPC_SYSCON->USBPLLCLKUEN = 0x00;/*Toggle Update Register*/
	LPC_SYSCON->USBPLLCLKUEN = 0x01;

	while (!(LPC_SYSCON->USBPLLCLKUEN & BIT0)){}/*Wait Until Updated*/
	LPC_SYSCON->USBPLLCTRL = USBPLLCTRL_Val;

	while (!(LPC_SYSCON->USBPLLSTAT & BIT0)){}/*Wait Until PLL Locked*/
	LPC_SYSCON->USBCLKSEL = 0x00;/*Select USB PLL*/
	LPC_SYSCON->USBCLKSEL = USBCLKSEL_Val;/* Select USB Clock */
	LPC_SYSCON->USBCLKDIV = USBCLKDIV_Val;/* Set USB clock divider */

	/* System clock to the IOCON needs to be enabled or
	most of the I/O related peripherals won't work. */
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT16;

	SystemCoreClock = (__SYS_OSC_CLK * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1));
	SystemCoreClock /= LPC_SYSCON->SYSAHBCLKDIV;
	
}
/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 
 *         Initialize the System.
 */
 #if 0
void nsAPI_System_Init(void)
{
	//U8 i;

	#if 0  //2016_0504_Hsiang
	LPC_SYSCON->PDRUNCFG &= ~BIT5;/*Power-up System Osc*/
	LPC_SYSCON->SYSOSCCTRL = SYSOSCCTRL_Val;
	for (i = 0; i < 200; i++) __NOP();

	LPC_SYSCON->SYSPLLCLKSEL = SYSPLLCLKSEL_Val;/*Select PLL Input*/
	LPC_SYSCON->SYSPLLCLKUEN = 0x01;/*Update Clock Source*/
	LPC_SYSCON->SYSPLLCLKUEN = 0x00;/*Toggle Update Register*/
	LPC_SYSCON->SYSPLLCLKUEN = 0x01;
	while(!(LPC_SYSCON->SYSPLLCLKUEN & BIT0)){}/*Wait Until Updated*/

	LPC_SYSCON->SYSPLLCTRL = SYSPLLCTRL_Val;/*Main Clock is PLL Out*/
	LPC_SYSCON->PDRUNCFG &= ~BIT7;/*Power-up SYSPLL*/
	while(!(LPC_SYSCON->SYSPLLSTAT & BIT0)){}/*Wait Until PLL Locked*/

	LPC_SYSCON->MAINCLKSEL = MAINCLKSEL_Val;/*Select PLL Clock Output*/
	LPC_SYSCON->MAINCLKUEN = 0x01;/*Update MCLK Clock Source*/
	LPC_SYSCON->MAINCLKUEN = 0x00;/*Toggle Update Register*/
	LPC_SYSCON->MAINCLKUEN = 0x01;
	while (!(LPC_SYSCON->MAINCLKUEN & BIT0)){}/*Wait Until Updated*/

	LPC_SYSCON->SYSAHBCLKDIV  = SYSAHBCLKDIV_Val;
	LPC_SYSCON->PDRUNCFG &= ~BIT10;/* Power-up USB PHY*/
	LPC_SYSCON->PDRUNCFG &= ~BIT8;/* Power-up USB PLL*/
	LPC_SYSCON->USBPLLCLKSEL = USBPLLCLKSEL_Val;/* Select PLL Input*/
	LPC_SYSCON->USBPLLCLKUEN = 0x01;/*Update Clock Source*/
	LPC_SYSCON->USBPLLCLKUEN = 0x00;/*Toggle Update Register*/
	LPC_SYSCON->USBPLLCLKUEN = 0x01;

	while (!(LPC_SYSCON->USBPLLCLKUEN & BIT0)){}/*Wait Until Updated*/
	LPC_SYSCON->USBPLLCTRL = USBPLLCTRL_Val;

	while (!(LPC_SYSCON->USBPLLSTAT & BIT0)){}/*Wait Until PLL Locked*/
	LPC_SYSCON->USBCLKSEL = 0x00;/*Select USB PLL*/
	LPC_SYSCON->USBCLKSEL = USBCLKSEL_Val;/* Select USB Clock */
	LPC_SYSCON->USBCLKDIV = USBCLKDIV_Val;/* Set USB clock divider */

	/* System clock to the IOCON needs to be enabled or
	most of the I/O related peripherals won't work. */
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT16;


	SystemCoreClock = (__SYS_OSC_CLK * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1));
	SystemCoreClock /= LPC_SYSCON->SYSAHBCLKDIV;
	#endif
	System_Config_Init();

///-----------------------------------------------------------------APP INIT

#if 0
	#if (ENABLE_UART)
		nsAPI_UART_Init(BR);
		xprintf("\r\n nsAPI_UART_Init",0);	
	#endif

	
	nsAPI_I2C_Init();
	
       #if (ENABLE_UART)
		xprintf("\r\n nsAPI_I2C_Init",0);
	#endif
#endif	

	//nsAPI_I2C_Init();
	//S_DATA_INIT();	//2016_0401_Hsiang

       //nsAPI_Database_Init();
       
	nsAPI_GPIO_Init();
	wheel_init();		//2016_0505_Hsiang
	//S_GPIO_INIT();

	//S_PWM_INIT();

       //#if (ENABLE_UART)
	//	xprintf("\r\n nsAPI_GPIO_Init",0);
	//#endif

#if 0	
	nsAPI_I2C_Read_Database(RM_REPORT_RATE_ADR,&g_dMouse.SensorSetting.dbReportRate,1);
	if((g_dMouse.SensorSetting.dbReportRate!=0x01)&&(g_dMouse.SensorSetting.dbReportRate!=0x02)&&(g_dMouse.SensorSetting.dbReportRate!=0x04))
	{
		g_dMouse.SensorSetting.dbReportRate = ANS9800_DEFAULT_REPORT_RATE;
		nsAPI_I2C_Write_Database(RM_REPORT_RATE_ADR,&g_dMouse.SensorSetting.dbReportRate,1);
	}
#endif	


	//nsAPI_Timer32PWM_Init(Timer32_0,2/*KHz*/,PWM_MATCH_ENABLE);

	#if 0	// PWM IO INIT Setting 
	#if (UART_USE_PIO0_18_19 ==0)	// No Use
		InitCTnBx(0,TIME_CT32B0_COUNT); //CB32B0
	#endif
	
	InitCTnBx(1,TIME_CT32B0_COUNT); //CB32B1
	InitCTnBx(2,TIME_CT16B0_COUNT); //CB16B0
	#endif

	
       //nsAPI_Timer16_Init(Timer16_1,100/*ms*/);
	nsAPI_SysTick_Init();	   		// Base Timer for 1mSec
	
	   
       //#if (ENABLE_UART)
	//	xprintf("\r\n nsAPI_SysTick_Init",0);
	//	xprintf("\r\n Timer Init",0);
	//#endif

/*
while(1){

	S_Idle_Delay_uSec(1);
		nsAPI_GPIO_LEDSetting(V_DPI_3_LED_Port,V_DPI_3_LED_Pin, V_LED_Active);		// LED On			
	S_Idle_Delay_uSec(1);
		nsAPI_GPIO_LEDSetting(V_DPI_3_LED_Port,V_DPI_3_LED_Pin, !V_LED_Active);		// LED On			
	S_Idle_Delay_uSec(1);
		nsAPI_GPIO_LEDSetting(V_DPI_3_LED_Port,V_DPI_3_LED_Pin, V_LED_Active);		// LED On		
	S_Idle_Delay_uSec(1);	
		nsAPI_GPIO_LEDSetting(V_DPI_3_LED_Port,V_DPI_3_LED_Pin, !V_LED_Active);		// LED On				
//}
*/
	   
	//g_SSP_ADNS9800_Setting.sSensorInit = 0;  //2016_0401_Hsiang
	rSuspendFlag = 0;

	nsAPI_SSP0_Init();		//2016_0406_Hsiang
	#if (ENABLE_UART)
		//xprintf("\r\n nsAPI_SSP_Init...",0);
		//xprintf("\r\n nsAPI_SSP_Init",0);	
	#endif
	
	//S_DPI_Change();					// DPI INIT Run

	//nsAPI_Database_Init();
       //#if (ENABLE_UART)
	//xprintf("\r\n nsAPI_Database_Init",0);
	//#endif

	//S_SPI0_INIT();	// ININ SPI0 for EX-Flash Use		//2016_0401_Hsiang

#ifdef V_EX_Flash_DebugEnable
	xprintf("\r\n EX_Flash_Print_Enable",0);
#endif

	nsAPI_USB_Init();/* USB Initialization */		
	

	//S_PWM_INIT();	
	

}
#else
void nsAPI_System_Init(void)
{


	System_Config_Init();
 
///-----------------------------------------------------------------APP INIT

#if 0
	nsAPI_I2C_Init();
	//S_DATA_INIT();	//2016_0401_Hsiang

       //nsAPI_Database_Init();
       
	nsAPI_GPIO_Init();
	Wheel_Init();		//2016_0505_Hsiang
	//S_PWM_INIT();
//	nsAPI_SysTick_Init();	   		// Base Timer for 1mSec
	InitCT32B1(47 , 999) ;
	   
	//g_SSP_ADNS9800_Setting.sSensorInit = 0;  //2016_0401_Hsiang
	rSuspendFlag = 0;

	nsAPI_SSP0_Init();
#endif	
	//S_DPI_Change();					// DPI INIT Run

	//S_SPI0_INIT();	// ININ SPI0 for EX-Flash Use		//2016_0401_Hsiang


	
	
	

}

#endif
