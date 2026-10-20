/****************************************************************************
 *   $Id:: timer16.c 6950 2011-03-23 22:09:44Z usb00423                     $
 *   Project: NXP LPC11xx 16-bit timer example
 *
 *   Description:
 *     This file contains 16-bit timer code example which include timer 
 *     initialization, timer interrupt handler, and related APIs for 
 *     timer setup.
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
#define __TIMER16_C__

#include "LPC11Uxx.h"
#include "timer16.h"
#include "Gpio.h"
#include "Led.h"
extern RgbColor m_HsvToRgb(HsvColor hsv);
//#include "gpio.h" //(Add)20120503
//#include "Main.h"
U16 u16Timer16_0_period;
U16 u16Timer16_0_period_START;
U16 u16Timer16_0_period_STOP;
U8 u16_opposite=0;
U8 u16_times=0;
U16 duty_x=0;
U16 duty_temp;

/******************************************************************************
** Function name:		TIMER_0_IRQHandler
**
** Descriptions:		Timer/CounterX and CaptureX interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/

#if 1
void TIMER16_1_IRQHandler(void)
{

    if ( LPC_CT16B1->IR & BIT1 )
    {
	if(u16_times<11){
		u16_times++;
	}else{
		u16_times=0;
		u16_opposite=0;
		if(duty_x<94){
			duty_temp = duty_x;
		}else if(duty_x<188){
			duty_temp =duty_x - 94;
			u16_opposite=1;
		}else{
			duty_temp=0;
		}
		if(duty_temp<64){			
			duty_temp=256-4*duty_temp;
		}else{
			duty_temp=0;
		}
		if(!u16_opposite)
			duty_temp =256-duty_temp;
		//duty_temp=256-duty_temp;
		LPC_CT16B1->MR0 = u16Timer16_0_period*duty_temp/256 ;
		if(duty_x<188)
			duty_x++;		
		else
			duty_x=0;
	}
	
        LPC_CT16B1->IR = BIT1 ;

    }
}
#endif
/******************************************************************************
** Function name:		TIMER_1_IRQHandler
**
** Descriptions:		Timer/CounterX and CaptureX interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
/*
void NDrv_Timer16_Enable(U8 timer_num)
{
	if(timer_num == 0) 
	LPC_CT16B0->TCR = 1; 
	else 
	LPC_CT16B1->TCR = 1;  
}
*/
/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
/*
void NDrv_Timer16_Reset(uint8_t timer_num)
{
	uint32_t regVal;

	if(timer_num == 0)
	{
		regVal = LPC_CT16B0->TCR;
		regVal |= 0x02;
		LPC_CT16B0->TCR = regVal;
	}
	else
	{
		regVal = LPC_CT16B1->TCR;
		regVal |= 0x02;
		LPC_CT16B1->TCR = regVal;
	}
}
*/
/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
/*
void NDrv_Timer16_Disable(U8 timer_num)
{
	if(timer_num == 0) 
	LPC_CT16B0->TCR = 0; 
	else 
	LPC_CT16B1->TCR = 0; 
}*/
/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer number and timer interval
** Returned value:		None
** 
******************************************************************************/
#if 0
void nsAPI_Timer16_Init(U8 timer_num, U32 timer) 
{
     #if 0 // no use
	if(timer_num == Timer16_0)
	{
		NVIC_DisableIRQ(TIMER_16_0_IRQn);
		NDrv_Timer16_Reset(Timer16_0);

		/* Some of the I/O pins need to be clearfully planned if
		you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
		LPC_SYSCON->SYSAHBCLKCTRL |= BIT7; 			

		LPC_CT16B0->PR = Timer16_MHz_Prescale;
		LPC_CT16B0->MR0 = timer*Timer16_IntervalmS;
		//LPC_CT16B0->MR1 = timer*Timer16_IntervalmS;

		/* Capture 0 and 1 on rising edge, interrupt enable. */
		LPC_CT16B0->CCR = (BIT0|BIT2);
		LPC_CT16B0->MCR = (BIT0|BIT1);/*Interrupt and Reset on MR0 and MR1*/		

		/* Enable the TIMER0 Interrupt */
		NVIC_EnableIRQ(TIMER_16_0_IRQn);

		NDrv_Timer16_Enable(Timer16_0); 			
	}
	else if(timer_num ==  Timer16_1)	
	{
		NVIC_DisableIRQ(TIMER_16_1_IRQn);
		NDrv_Timer16_Reset(Timer16_1);
		/* Some of the I/O pins need to be clearfully planned if
		you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
		LPC_SYSCON->SYSAHBCLKCTRL |= BIT8;
		LPC_CT16B1->PR = Timer16_MHz_Prescale;	  

		LPC_CT16B1->MR0 = timer*Timer16_IntervalmS;
		//LPC_CT16B1->MR1 = timer*Timer16_IntervalmS;	

		/* Capture 0 and 1 on rising edge, interrupt enable. */
		LPC_CT16B1->CCR = (BIT0|BIT2);
		LPC_CT16B1->MCR = (BIT0|BIT2);/* Interrupt and Reset on MR0 and MR1 */

		/* Enable the TIMER1 Interrupt */
		NVIC_EnableIRQ(TIMER_16_1_IRQn);

		NDrv_Timer16_Enable(Timer16_1);    
	}
	#else
	
	if(timer_num ==  Timer16_1)	
	{
		NVIC_DisableIRQ(TIMER_16_1_IRQn);
		NDrv_Timer16_Reset(Timer16_1);
		/* Some of the I/O pins need to be clearfully planned if
		you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
		LPC_SYSCON->SYSAHBCLKCTRL |= BIT8;
		LPC_CT16B1->PR = Timer16_MHz_Prescale;	  

		LPC_CT16B1->MR0 = timer*Timer16_IntervalmS;
		//LPC_CT16B1->MR1 = timer*Timer16_IntervalmS;	

		/* Capture 0 and 1 on rising edge, interrupt enable. */
		LPC_CT16B1->CCR = (BIT0|BIT2);
		LPC_CT16B1->MCR = (BIT0|BIT2);/* Interrupt and Reset on MR0 and MR1 */

		/* Enable the TIMER1 Interrupt */
		NVIC_EnableIRQ(TIMER_16_1_IRQn);

		NDrv_Timer16_Enable(Timer16_1);    
	}		
	#endif
}

void InitCT16B0(/*unsigned int PR ,*/unsigned int Time)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT7 ;                           //Set AHB Clock To The CT16B0 Domain
	//-----
	LPC_IOCON->PIO0_8 &= ~0x07 ;
	LPC_IOCON->PIO0_8 |= 0x02 ;										//MAT0
	LPC_IOCON->PIO0_9 &= ~0x07 ;
	LPC_IOCON->PIO0_9 |= 0x02 ;										//MAT1
#if 1
	LPC_IOCON->SWCLK_PIO0_10 &= ~0x07 ;
	LPC_IOCON->SWCLK_PIO0_10 |= 0x03 ;								//MAT2
#endif
	//-----
	LPC_CT16B0->MR3 = Time;
	LPC_CT16B0->MR0 = Time;// 0 ;
	LPC_CT16B0->MR1 = Time; // 0 ;
	LPC_CT16B0->MR2 = Time;// 0 ;
	//LPC_CT16B0->PR = PR ;	
	LPC_CT16B0->PWMC = 7 ;
	LPC_CT16B0->MCR = ( 3 << 9 ) ;
	LPC_CT16B0->TCR = 1 ;

	g_Systick_Timer16_Breathing_Setting.u16RGB[COLOR_TYPE_R] = Time;
	g_Systick_Timer16_Breathing_Setting.u16RGB[COLOR_TYPE_G] = Time;	
	g_Systick_Timer16_Breathing_Setting.u16RGB[COLOR_TYPE_B] = Time;

	g_Systick_Timer16_Breathing_Setting.u16RGBMax[COLOR_TYPE_R] = Time;
	g_Systick_Timer16_Breathing_Setting.u16RGBMax[COLOR_TYPE_G] = Time;
	g_Systick_Timer16_Breathing_Setting.u16RGBMax[COLOR_TYPE_B] = Time;	
#if 0	
	g_Systick_Timer16_Breathing_Setting.u16BreathingSetting = LED_BREATHING_OFF;
	g_Systick_Timer16_Breathing_Setting.u16BreathingMax = LED_BREATHING_OFF;	
#endif	
	g_Systick_Timer16_Breathing_Setting.u8RGBStatus[COLOR_TYPE_R] = LED_BREATHING_STEP_D;
	g_Systick_Timer16_Breathing_Setting.u8RGBStatus[COLOR_TYPE_G] = LED_BREATHING_STEP_D;
	g_Systick_Timer16_Breathing_Setting.u8RGBStatus[COLOR_TYPE_B] = LED_BREATHING_STEP_D;

	NVIC_EnableIRQ(TIMER_16_0_IRQn) ;   
}
#endif
#if 0
void nsAPI_Timer16_B0M3_PWM_Init(U32 period)
{
	//U32 u32Timer32_0_period;

	NVIC_DisableIRQ(TIMER_16_1_IRQn);
	LPC_CT16B1->TCR = 0;

	/* Some of the I/O pins need to be clearfully planned if
   	 you use below module because JTAG and TIMER CAP/MAT pins are muxed. */

	/* Some of the I/O pins need to be clearfully planned if
	you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT8;

	LPC_CT16B1->PR = Timer16_MHz_Prescale;

	/* Setup the external match register */
	//LPC_CT32B0->EMR = (3 << EMC3);
	//LPC_CT32B0->EMR = (3 << EMC2)|(3 << EMC1)|(3 << EMC0) |(3 << EMC3) |MATCH3|MATCH1;
	LPC_CT16B1->EMR = (3 << EMC0)|(3 << EMC1) |MATCH1|MATCH0;
	//LPC_CT32B0->EMR = (3 << EMC1)|MATCH1;

	/* Setup the outputs */
	/* If match3 is enabled, set the output, use location 0 for test. */
	LPC_IOCON->PIO0_21 &= ~0x07;
	LPC_IOCON->PIO0_21 |= 0x01;		/* Timer1_16 MAT1 */

	/* Enable the selected PWMs and enable Match3 */
	LPC_CT16B1->PWMC = CT16B0_PWM_MATCH_ENABLE;

	/* Setup the match registers */
	/* set the period value to a global variable */
	u16Timer16_0_period = Timer16_IntervaluS*2500;
	duty_x= period;
	u16_opposite=0;
	if(duty_x<94){
		duty_temp = duty_x;
	}else if(duty_x<188){
		duty_temp =duty_x - 94;
		u16_opposite=1;
	}else{
		duty_temp=0;
	}
	if(duty_temp<64){			
		duty_temp=256-4*duty_temp;
	}else{
		duty_temp=0;
	}
	if(!u16_opposite)
		duty_temp =256-duty_temp;
	LPC_CT16B1->MR1 = u16Timer16_0_period;
	LPC_CT16B1->MR0 = u16Timer16_0_period*duty_temp/256 ;	
	
	LPC_CT16B1->MCR = BIT3 | BIT4 ;	/* Reset & interrup on MR1 */
	u16_times=0;
	NVIC_EnableIRQ(TIMER_16_1_IRQn);
	
	LPC_CT16B1->TCR = 1;		
	
}

void PWM_TIMER_ENABLE(bool enable)
{
	if(enable)
	{
		NVIC_DisableIRQ(TIMER_16_1_IRQn);
		LPC_CT16B1->TCR &= ~0x01;					// Disable Timer Enable
		LPC_CT16B1->TC =0;							// Reset TC Count				
		LPC_CT16B1->MR[1] = 768000;		//		
		LPC_CT16B1->MR[0] = 768000;		//
		LPC_CT16B1->PWMC &= ~(BIT1|BIT0);			// Disable PW0 / PW1 / PW3

	}else{
		nsAPI_Timer16_B0M3_PWM_Init(0);
	}
}
#endif
