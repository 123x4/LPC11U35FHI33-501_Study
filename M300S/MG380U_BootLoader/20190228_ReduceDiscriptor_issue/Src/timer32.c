/****************************************************************************
 *   $Id:: timer32.c 6951 2011-03-23 22:11:07Z usb00423                     $
 *   Project: NXP LPC11xx 32-bit timer example
 *
 *   Description:
 *     This file contains 32-bit timer code example which include timer
 *     initialization, timer interrupt handler, and related APIs for
 *     timer setup.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semi+conductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#define __TIMER32_C__

#include "LPC11Uxx.h"
#include "timer32.h"

#include "Gpio.h"
#if 0
U32 temp=0;
U32 u32Timer32_0_period;
U32 u32Timer32_0_period_START;
U32 u32Timer32_0_period_STOP;
U8 invt=0;
U8 times=0;
#else
U32 u32_Timer32_0_period;
U32 u32_Timer32_0_period_START;
U32 u32_Timer32_0_period_STOP;
U8 	u32_opposite=0;
U8 	u32_times=0;
U16 u32_duty_x=0;
U16 u32_duty_temp;
#endif
extern void LED_AUDIO_ENABLE(void);
//#include "gpio.h" //(Add) 20120503
//#include "Main.h"
//#include "uart.h"
void InitCT32B1(unsigned int PR ,unsigned int Time) ;

/******************************************************************************
** Function name:		TIMER32_0_IRQHandler
**
** Descriptions:		Timer/CounterX and captureX interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
#if 0
void TIMER32_0_IRQHandler(void)
{
	if ( LPC_CT32B0->IR & BIT1 )
	{
		//ISSI3731_Software_showdown(FALSE);
		LPC_CT32B0->IR = BIT1;
	}
	if ( LPC_CT32B0->IR & BIT3 )
	{
    		
	
		if(invt==0){
			if(u32Timer32_0_period < 1998)
				if(times==5){
					u32Timer32_0_period+=2;
					times=0;
				}else
					times++;
			else
				invt=1;
		}
		else{
			if(u32Timer32_0_period >2)
				if(times==5){
					u32Timer32_0_period-=2;
					times=0;
				}else
					times++;
			else
				invt=0;
		}
		//LPC_CT32B0->MR3 = u32Timer32_0_period/2 ;
		LPC_CT32B0->MR1 = u32Timer32_0_period ;
		//LED_AUDIO_ENABLE();
	
		LPC_CT32B0->IR = BIT3;
	}
		
}
#else
void TIMER32_0_IRQHandler(void)
{
	if ( LPC_CT32B0->IR & BIT3 )
	{
		#if 0
		if(u32_times<11)
		{
			u32_times++;
		}
		else
		{
			u32_times=0;
			u32_opposite=0;
			if(u32_duty_x<94)
			{
				u32_duty_temp = u32_duty_x;
			}
			else if(u32_duty_x<188)
			{
				u32_duty_temp =u32_duty_x - 94;
				u32_opposite=1;
			}
			else
			{
				u32_duty_temp=0;
			}
			if(u32_duty_temp<64)
			{			
				u32_duty_temp=256-4*u32_duty_temp;
			}
			else
			{
				u32_duty_temp=0;
			}
			if(!u32_opposite)
				u32_duty_temp =256-u32_duty_temp;
			//duty_temp=256-duty_temp;
			LPC_CT32B0->MR1 = u32_Timer32_0_period*u32_duty_temp/256 ;
			if(u32_duty_x<188)
				u32_duty_x++;		
			else
				u32_duty_x=0;
		}
		#endif
		//LPC_CT32B0->TC = 1;
    LPC_CT32B0->IR = BIT3 ;
	}
}



#endif
/******************************************************************************
** Function name:		TIMER32_1_IRQHandler
**
** Descriptions:		Timer/CounterX and captureX interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
#if 1
void TIMER32_1_IRQHandler(void)
{
    if( LPC_CT32B1->IR & BIT3)
    {
        LPC_CT32B1->IR = BIT3;
//	S_KeyScan_BounceCheck();		// Base for 1mSec
//	if(LED_Rainbow_Timer)
//		LED_Rainbow_Timer--;
//	if(LED_Breath_Timer)
//		LED_Breath_Timer--;
//	if(LED_Colorcycle_Timer)
//		LED_Colorcycle_Timer--;
//	
//		


//	if(g_Key_Matrix_Attr.fKeyDelay == 1){
//		g_Key_Matrix_Attr.KeyDelay++;
//		g_Key_Matrix_Attr.fKeyDelay = 0;
//	}
//	else{
//		g_Key_Matrix_Attr.fKeyDelay = 1;
//	}		
    }
}
#endif
/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
#if 1// no use
void NDrv_Timer32_Enable(U8 timer_num)
{
	if(timer_num == 0)
	LPC_CT32B0->TCR = 1;
	else
	LPC_CT32B1->TCR = 1;
}
//-----------------------------------------------------------------------------
void NDrv_Timer32_B0_Enable(void)
{
	LPC_CT32B0->TCR = 1;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void NDrv_Timer32_Disable(U8 timer_num)
{
	if(timer_num == 0)
	LPC_CT32B0->TCR = 0;
	else
	LPC_CT32B1->TCR = 0;
}
void NDrv_Timer32_B0_Disable(void)
{
	LPC_CT32B0->TCR = 0;

}
/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void NDrv_Timer32_Reset(U8 timer_num)
{
	U32 regVal;

	if(timer_num == 0)
	{
		regVal = LPC_CT32B0->TCR;
		regVal |= BIT1;
		LPC_CT32B0->TCR = regVal;
	}
	else
	{
		regVal = LPC_CT32B1->TCR;
		regVal |= BIT1;
		LPC_CT32B1->TCR = regVal;
	}
}
//-----------------------------------------------------------------------------
void NDrv_Timer32_B0_Reset(void)
{
	U32 regVal;
	
	regVal = LPC_CT32B0->TCR;
	//regVal |= BIT1;

	LPC_CT32B0->TCR = 0;
	LPC_CT32B0->TC = 1;
	LPC_CT32B0->TCR |= BIT1;
	while (LPC_CT32B0->TC != 0) {}
	LPC_CT32B0->TCR = regVal;
}
#endif
/******************************************************************************
** Function name:		set_timer_match
**
** Descriptions:		Set timer match based on location
**
** parameters:			timer number: 0~1, location 0~2
** Returned value:		None
**
******************************************************************************/
#if 1 // mask , no use
void nsAPI_Timer32_Setting_MatchFunction(U8 timer_num, U8 match_enable, U8 location)
{
	if(timer_num == 0)
	{
		if(match_enable & BIT0)
		{
			if(location == 0)
			{
				LPC_IOCON->PIO0_18 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->PIO0_18 |= BIT1;/* Timer0_32 MAT0 */
			}
			else if(location == 1)
			{
				LPC_IOCON->PIO1_24 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->PIO1_24 |= BIT0;/* Timer0_32 MAT0 */
			}
		}

		if(match_enable & BIT1)
		{
			if ( location == 0 )
			{
				LPC_IOCON->PIO0_19 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->PIO0_19 |= BIT1;/* Timer0_32 MAT1 */
			}
			else if ( location == 1 )
			{
				LPC_IOCON->PIO1_25 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->PIO1_25 |= BIT0;/* Timer0_32 MAT1 */
			}
		}

		if(match_enable & BIT2)
		{
			if ( location == 0 )
			{
				LPC_IOCON->PIO0_1 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->PIO0_1 |= BIT1;/* Timer0_32 MAT2 */
			}
			else if ( location == 1 )
			{
				LPC_IOCON->PIO1_26 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->PIO1_26 |= BIT0;/* Timer0_32 MAT2 */
			}
		}

		if(match_enable & BIT3)
		{
			if ( location == 0 )
			{
				LPC_IOCON->TDI_PIO0_11 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->TDI_PIO0_11 |= (BIT1|BIT0);/* Timer0_32 MAT3 */
			}
			else if ( location == 1 )
			{
				LPC_IOCON->PIO1_27 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->PIO1_27 |= BIT0;/* Timer0_32 MAT3 */
			}
		}
	}
	else if(timer_num == 1)
	{
		if(match_enable & BIT0)
		{
			if(location == 0)
			{
				//LPC_IOCON->PIO1_0 &= ~0x07;
				//LPC_IOCON->PIO1_0 |= 0x01;/* Timer1_32 MAT0 */
			}
			else if(location == 1)
			{
				LPC_IOCON->TDO_PIO0_13 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->TDO_PIO0_13 |= (BIT1|BIT0);/* Timer1_32 MAT0 */
			}
		}

		if(match_enable & BIT1)
		{
			if(location == 0)
			{
				//LPC_IOCON->PIO1_1 &= ~0x07;
				//LPC_IOCON->PIO1_1 |= 0x01;/* Timer1_32 MAT1 */
			}
			else if(location == 1)
			{
				LPC_IOCON->TRST_PIO0_14 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->TRST_PIO0_14 |= (BIT1|BIT0);/* Timer1_32 MAT1 */
			}
		}

		if(match_enable & BIT2)
		{
			if(location == 0)
			{
				//LPC_IOCON->PIO1_2 &= ~0x07;
				//LPC_IOCON->PIO1_2 |= 0x01;/* Timer1_32 MAT2 */
			}
			else if ( location == 1 )
			{
#if __SWD_DISABLED
				LPC_IOCON->SWDIO_PIO0_15 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->SWDIO_PIO0_15 |= (BIT1|BIT0);/* Timer1_32 MAT2 */
#endif
			}
		}

		if(match_enable & BIT3)
		{
			if(location == 0)
			{
				//LPC_IOCON->PIO1_3 &= ~0x07;
				//LPC_IOCON->PIO1_3 |= 0x01;/* Timer1_32 MAT3 */
			}
			else if(location == 1)
			{
				LPC_IOCON->PIO0_16 &= ~(BIT2|BIT1|BIT0);
				LPC_IOCON->PIO0_16 |= BIT1;/* Timer1_32 MAT3 */
			}
		}
	}
}
//----------------------------------------------------------------------------------------
void nsAPI_Timer32_B0M3_Setting_MatchFunction(void)
{
	LPC_IOCON->PIO1_27 &= ~(BIT3|BIT2|BIT1|BIT0);
	LPC_IOCON->PIO1_27 |= BIT0;/* Timer0_32 MAT3 */
}
//----------------------------------------------------------------------------------------


void nsAPI_Timer32PWM_Breathing_ReSetting(U8 u8Breathing)
{
	g_Timer32_LED_Setting.LEDStatusType = LED_BREATHING;
	g_Timer32_LED_Setting.BreathingStatusType = 0;
	g_Timer32_LED_Setting.BreathingDelayCount = 0;
	g_Timer32_LED_Setting.BreathingStep = BREATHING_STEP*u8Breathing;

	g_Timer32_LED_Setting.CurBreathingFreq = \
	g_Timer32_LED_Setting.PreBreathingFreq -BREATHING_FREQ_GAP;
}

/******************************************************************************
** Function name:		init_timer32PWM
**
** Descriptions:		Initialize timer as PWM
**
** parameters:			timer number, period and match enable:
**						match_enable[0] = PWM for MAT0
**						match_enable[1] = PWM for MAT1
**						match_enable[2] = PWM for MAT2
** Returned value:		None
**
******************************************************************************/
void nsAPI_Timer32PWM_Init(U8 timer_num, U32 period, U8 match_enable)
{
	NDrv_Timer32_Disable(timer_num);
	
	g_Timer32_LED_Setting.LEDStatusType = LED_BREATHING;
	g_Timer32_LED_Setting.BreathingStatusType = 0;
	g_Timer32_LED_Setting.BreathingDelayCount = 0;
	g_Timer32_LED_Setting.BreathingStepStatus = 0;
	g_Timer32_LED_Setting.BreathingStep = BREATHING_STEP;
	g_Timer32_LED_Setting.Timer32Color.Timer32_Color = TIMER_COLOR_RED;

	if(timer_num == 1)
	{
		U32 u32Timer32_1_period;

		LPC_SYSCON->SYSAHBCLKCTRL |= BIT10;

		/* Setup the external match register */
		LPC_CT32B1->EMR = (1 << EMC2)|(2 << EMC1)|(1 << EMC0);

		/* Setup the outputs */
		/* If match0 is enabled, set the output, use location 0 for test. */
		nsAPI_Timer32_Setting_MatchFunction(timer_num,match_enable,1);

		LPC_CT32B1->PWMC |= match_enable;
		/* Setup the match registers */
		/* set the period value to a global variable */
		u32Timer32_1_period = period*Timer32_IntervalmS;

		LPC_CT32B1->MR0 = u32Timer32_1_period / 2;
		LPC_CT32B1->MR1 = u32Timer32_1_period / 2;
		LPC_CT32B1->MR2 = u32Timer32_1_period / 2;
		LPC_CT32B1->MR3 = u32Timer32_1_period;
		LPC_CT32B1->MCR = BIT10;/* Reset on MR3 */
	}
	else
	{
		//U32 u32Timer32_0_period;
		/* Some of the I/O pins need to be clearfully planned if
		you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
		LPC_SYSCON->SYSAHBCLKCTRL |= BIT9;

		LPC_CT32B0->PR = Timer32_MHz_Prescale;

		/* Setup the external match register */
		LPC_CT32B0->EMR = (3 << EMC2)|(3 << EMC1)|(3 << EMC0);

		/* Setup the outputs */
		/* If match0 is enabled, set the output, use location 0 for test. */
		nsAPI_Timer32_Setting_MatchFunction(timer_num, match_enable,1);

		/* Enable the selected PWMs and enable Match3 */
		LPC_CT32B0->PWMC = match_enable;

		/* Setup the match registers */
		/* set the period value to a global variable */
		u32_Timer32_0_period = period*Timer32_IntervalmS;

		if(u32_Timer32_0_period < BREATHING_FREQ_GAP)
		u32_Timer32_0_period = BREATHING_FREQ_GAP;

		LPC_CT32B0->MR0 = u32_Timer32_0_period -BREATHING_FREQ_GAP;
		LPC_CT32B0->MR1 = u32_Timer32_0_period -BREATHING_FREQ_GAP;
		LPC_CT32B0->MR2 = u32_Timer32_0_period -BREATHING_FREQ_GAP;
		LPC_CT32B0->MR3 = u32_Timer32_0_period;
		LPC_CT32B0->MCR = BIT10;	/* Reset on MR3 */

		g_Timer32_LED_Setting.CurBreathingFreq = u32_Timer32_0_period -BREATHING_FREQ_GAP;
		g_Timer32_LED_Setting.PreBreathingFreq = u32_Timer32_0_period;
	}

	NDrv_Timer32_Reset(timer_num);
	NDrv_Timer32_Enable(timer_num);
}
#if 0
//----------------------------------------------------------------------------------------------------------------
void NDrv_Timer32PWM_Breathing(void)
{
	S32 s32Timer32_period = g_Timer32_LED_Setting.CurBreathingFreq;

       if(g_Timer32_LED_Setting.BreathingStatusType)
       {
		if(s32Timer32_period <  (g_Timer32_LED_Setting.PreBreathingFreq -BREATHING_FREQ_GAP*2))
		{
			s32Timer32_period += (g_Timer32_LED_Setting.BreathingStep*BREATHING_STEP);

			nsAPI_Timer32_Setting_Color(g_Timer32_LED_Setting.Timer32Color.Timer32_Color,\
			s32Timer32_period);

			LPC_CT32B0->MR0 = g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Red;/*Green*/
			LPC_CT32B0->MR1 = g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Green;/*Red*/
			LPC_CT32B0->MR2 = g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Blue;/*Blue*/

			g_Timer32_LED_Setting.CurBreathingFreq = s32Timer32_period;

			if((g_Main_Setting.MouseEvent & MAIN_FLAG_MOUSE_SENSOR_FRAME_CAPTURE_EVENT) ==\
			MAIN_FLAG_MOUSE_SENSOR_FRAME_CAPTURE_EVENT)
			g_Timer32_LED_Setting.BreathingDelayCount = 0;
			else
			g_Timer32_LED_Setting.BreathingDelayCount = BREATHING_DELAY_TIMER*5;
		}
		else
		{
			if(g_Timer32_LED_Setting.BreathingDelayCount > 0)
			g_Timer32_LED_Setting.BreathingDelayCount --;
			else
			g_Timer32_LED_Setting.BreathingStatusType = 0;
		}
       }
       else
       {
             if((s32Timer32_period - (g_Timer32_LED_Setting.BreathingStep*BREATHING_STEP)) > 0)
		{
			s32Timer32_period -= (g_Timer32_LED_Setting.BreathingStep*BREATHING_STEP);

			nsAPI_Timer32_Setting_Color(g_Timer32_LED_Setting.Timer32Color.Timer32_Color,\
			s32Timer32_period);

			LPC_CT32B0->MR0 = g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Red;/*Green*/
			LPC_CT32B0->MR1 = g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Green;/*Red*/
			LPC_CT32B0->MR2 = g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Blue;/*Blue*/

			g_Timer32_LED_Setting.CurBreathingFreq = s32Timer32_period;

			if((g_Main_Setting.MouseEvent & MAIN_FLAG_MOUSE_SENSOR_FRAME_CAPTURE_EVENT) ==\
			MAIN_FLAG_MOUSE_SENSOR_FRAME_CAPTURE_EVENT)
			g_Timer32_LED_Setting.BreathingDelayCount = 0;
			else
			g_Timer32_LED_Setting.BreathingDelayCount = BREATHING_DELAY_TIMER*8;
		}
		else
		{
			if(g_Timer32_LED_Setting.BreathingDelayCount > 0)
			g_Timer32_LED_Setting.BreathingDelayCount --;
			else
			{
			g_Timer32_LED_Setting.BreathingStatusType = 1;
#if 1/*Demo only*/
			if(g_Timer32_LED_Setting.Timer32Color.Timer32_Color < TIMER_COLOR_YELLOW)
			g_Timer32_LED_Setting.Timer32Color.Timer32_Color <<= 1;
			else
			g_Timer32_LED_Setting.Timer32Color.Timer32_Color = TIMER_COLOR_RED;
#endif
			}
		}
       }
}
#endif
#if 0
//-----------------------------------------------------------------------------
void nsAPI_Timer32_Setting_Color(U8 u8Color,U32 u32period)
{
	switch(u8Color)
	{
		case TIMER_COLOR_RED:
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Red = u32period;
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Green = TIMER32_COLOR_EMPTY;
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Blue = TIMER32_COLOR_EMPTY;
		break;

		case TIMER_COLOR_GREEN:
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Red = TIMER32_COLOR_EMPTY;
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Green = u32period;
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Blue = TIMER32_COLOR_EMPTY;
		break;

		case TIMER_COLOR_BLUE:
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Red = TIMER32_COLOR_EMPTY;
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Green = TIMER32_COLOR_EMPTY;
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Blue = u32period;
		break;

		case TIMER_COLOR_YELLOW:
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Red = u32period;
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Green = u32period;
		g_Timer32_LED_Setting.Timer32Color.Timer32_Color_Blue = TIMER32_COLOR_EMPTY;
		break;
	}
}
#endif
#endif // mask , no use

#if 0
void InitCTnBx(U8 u8CT32Bx,U32 Time)
{
	LPC11Uxx_SYSTICK_TIMER_BREATHING_SETTING * ptBreathing_Setting = NULL;
	LPC_CTxxBx_Type * LPC_CTx_BASE = NULL;
	//IRQn_Type tIRQ_Type;

	switch(u8CT32Bx)
	{
		case 0:
		LPC_SYSCON->SYSAHBCLKCTRL |= BIT9;
		LPC_IOCON->PIO0_1 &= ~0x07 ;
		LPC_IOCON->PIO0_1 |= 0x02 ;	
    #if (UART_USE_PIO0_18_19 ==0)
		LPC_IOCON->PIO0_18 &= ~0x07 ;
		LPC_IOCON->PIO0_18 |= 0x02 ;
		LPC_IOCON->PIO0_19 &= ~0x07 ;
        LPC_IOCON->PIO0_19 |= 0x02 ;
    #endif
		ptBreathing_Setting = &g_Systick_Timer32_Breathing0_Setting;
		LPC_CTx_BASE = LPC_CT32B0;
		//tIRQ_Type = TIMER_32_0_IRQn;
		break;

		case 1:
		LPC_SYSCON->SYSAHBCLKCTRL |= BIT10;
		LPC_IOCON->TDO_PIO0_13 &= ~0x07 ;
		LPC_IOCON->TDO_PIO0_13 |= 0x03 ;
		LPC_IOCON->TRST_PIO0_14 &= ~0x07 ;
		LPC_IOCON->TRST_PIO0_14 |= 0x03 ;
		LPC_IOCON->SWDIO_PIO0_15 &= ~0x07 ;
		LPC_IOCON->SWDIO_PIO0_15 |= 0x03 ;
		ptBreathing_Setting = &g_Systick_Timer32_Breathing1_Setting;
		LPC_CTx_BASE = LPC_CT32B1;
		//tIRQ_Type = TIMER_32_1_IRQn;
		break;

		case 2:
		LPC_SYSCON->SYSAHBCLKCTRL |= BIT7;
		LPC_IOCON->PIO0_8 &= ~0x07;
		LPC_IOCON->PIO0_8 |= 0x02;
		LPC_IOCON->PIO0_9 &= ~0x07;
		LPC_IOCON->PIO0_9 |= 0x02;
		LPC_IOCON->SWCLK_PIO0_10 &= ~0x07;
		LPC_IOCON->SWCLK_PIO0_10 |= 0x03;
		ptBreathing_Setting = &g_Systick_Timer16_Breathing_Setting;
		LPC_CTx_BASE = LPC_CT16B0;
		//tIRQ_Type = TIMER_16_0_IRQn;
		break;
		default:
		return;
	}

	LPC_CTx_BASE->MR3 = Time ;
    #if (UART_USE_PIO0_18_19 ==0)
	LPC_CTx_BASE->MR0 = Time;
	LPC_CTx_BASE->MR1 =Time;
	LPC_CTx_BASE->MR2 = Time;
	LPC_CTx_BASE->PWMC = 7 ;
	LPC_CTx_BASE->MCR = (3 << 9) ;
	LPC_CTx_BASE->TCR = 1 ;
    #else
    if (u8CT32Bx != 0)
    {
        LPC_CTx_BASE->MR0 = Time;
    	LPC_CTx_BASE->MR1 =Time;

    	LPC_CTx_BASE->MR2 = Time;
    	LPC_CTx_BASE->PWMC = 7 ;
    	LPC_CTx_BASE->MCR = (3 << 9) ;
    	LPC_CTx_BASE->TCR = 1 ;
    }
    #endif

	ptBreathing_Setting->u16RGB[COLOR_TYPE_R] = Time;
	ptBreathing_Setting->u16RGB[COLOR_TYPE_G] = Time;
	ptBreathing_Setting->u16RGB[COLOR_TYPE_B] = Time;

	ptBreathing_Setting->u16RGBMax[COLOR_TYPE_R] = Time;
	ptBreathing_Setting->u16RGBMax[COLOR_TYPE_G] = Time;
	ptBreathing_Setting->u16RGBMax[COLOR_TYPE_B] = Time;

	ptBreathing_Setting->u8RGBStatus[COLOR_TYPE_R] = LED_BREATHING_STEP_D;
	ptBreathing_Setting->u8RGBStatus[COLOR_TYPE_G] = LED_BREATHING_STEP_D;
	ptBreathing_Setting->u8RGBStatus[COLOR_TYPE_B] = LED_BREATHING_STEP_D;

	//NVIC_EnableIRQ(tIRQ_Type);
}

//----------------------------------------------------------------------------

void Timer_INIT(void)
{
	/* Enable timer 1 clock */
	Chip_TIMER_Init(LPC_TIMER32_0);
	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER32_0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 3);
	//Chip_TIMER_SetMatch(LPC_TIMER32_0, 1, (timerFreq / TICKRATE_HZ2));
	Chip_TIMER_SetMatch(LPC_TIMER32_0, 3, TIME_CT32B0_COUNT);	
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 3);
	Chip_TIMER_Enable(LPC_TIMER32_0);
}
#endif
/******************************************************************************
** Function name:		init_timer32PWM
**
** Descriptions:		Initialize timer as PWM
**
** parameters:			timer number, period and match enable:
**						match_enable[0] = PWM for MAT0
**						match_enable[1] = PWM for MAT1
**						match_enable[2] = PWM for MAT2
** Returned value:		None
**
******************************************************************************/
#if 0
void nsAPI_Timer32_B0M3_PWM_Init(U32 period)
{
	//U32 u32Timer32_0_period;
	NVIC_DisableIRQ(TIMER_32_0_IRQn);
	NDrv_Timer32_Disable(Timer32_0);

	/* Some of the I/O pins need to be clearfully planned if
   	 you use below module because JTAG and TIMER CAP/MAT pins are muxed. */

	/* Some of the I/O pins need to be clearfully planned if
	you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT9;

	LPC_CT32B0->PR = Timer32_MHz_Prescale;

	/* Setup the external match register */
	//LPC_CT32B0->EMR = (3 << EMC3);
	//LPC_CT32B0->EMR = (3 << EMC2)|(3 << EMC1)|(3 << EMC0) |(3 << EMC3) |MATCH3|MATCH1;
	LPC_CT32B0->EMR = (3 << EMC0)|(3 << EMC3) |MATCH3|MATCH0;

	/* Setup the outputs */
	/* If match3 is enabled, set the output, use location 0 for test. */
	LPC_IOCON->PIO1_24 &= ~0x07;
	LPC_IOCON->PIO1_24 |= 0x01;		/* Timer0_32 MAT0 */

	/* Enable the selected PWMs and enable Match3 */
	LPC_CT32B0->PWMC = PWM_MATCH_ENABLE;

	/* Setup the match registers */
	/* set the period value to a global variable */
	u32Timer32_0_period = period*Timer32_IntervaluS;
	//temp=u32Timer32_0_period/2;
	//BREATHING_FREQ_GAP=u32Timer32_0_period/2;
	LPC_CT32B0->MR0 = u32Timer32_0_period*2 ;
	LPC_CT32B0->MR3 = u32Timer32_0_period*11;
	LPC_CT32B0->MCR = BIT10 | BIT9 ;//| BIT3;	/* Reset on MR3 */

	
	
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
	
	NDrv_Timer32_Enable(Timer32_0); 		
}
//#else
void nsAPI_Timer32_B0M3_PWM_Init(U32 period)
{
	#if 0
	//U32 u32Timer32_0_period;

	NVIC_DisableIRQ(TIMER_32_0_IRQn);
	LPC_CT32B0->TCR = 0;

	/* Some of the I/O pins need to be clearfully planned if
   	 you use below module because JTAG and TIMER CAP/MAT pins are muxed. */

	/* Some of the I/O pins need to be clearfully planned if
	you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT9;

	LPC_CT32B0->PR = Timer32_MHz_Prescale;

	/* Setup the external match register */
	//LPC_CT32B0->EMR = (3 << EMC3);
	//LPC_CT32B0->EMR = (3 << EMC2)|(3 << EMC1)|(3 << EMC0) |(3 << EMC3) |MATCH3|MATCH1;
	//LPC_CT32B0->EMR = (3 << EMC1)|(3 << EMC3) |MATCH1|MATCH3;
	LPC_CT32B0->EMR = (3 << EMC3) | MATCH3;
	//LPC_CT32B0->EMR = (3 << EMC1)|MATCH1;

	/* Setup the outputs */
	/* If match3 is enabled, set the output, use location 0 for test. */
	LPC_IOCON->PIO1_25 &= ~0x07;
	LPC_IOCON->PIO1_25 |= 0x01;		/* Timer1_16 MAT1 */

	/* Enable the selected PWMs and enable Match3 */
	LPC_CT32B0->PWMC = PWM_MATCH_ENABLE;

	/* Setup the match registers */
	/* set the period value to a global variable */
	u32_Timer32_0_period = Timer16_IntervaluS*2500;
	u32_duty_x= period;
	u32_opposite=0;
	if(u32_duty_x<94){
		u32_duty_temp = u32_duty_x;
	}else if(u32_duty_x<188){
		u32_duty_temp =u32_duty_x - 94;
		u32_opposite=1;
	}else{
		u32_duty_temp=0;
	}
	if(u32_duty_temp<64){			
		u32_duty_temp=256-4*u32_duty_temp;
	}else{
		u32_duty_temp=0;
	}
	if(!u32_opposite)
		u32_duty_temp =256-u32_duty_temp;
	LPC_CT32B0->MR3 = u32_Timer32_0_period;
	//LPC_CT32B0->MR1 = u32_Timer32_0_period*u32_duty_temp/256 ;	
	//LPC_CT32B0->MR1 = u32_Timer32_0_period/4 ;	
	
	LPC_CT32B0->MCR = BIT9 | BIT10;//| BIT3;	/* Reset on MR3 */
	u32_times=0;
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
	
	LPC_CT32B0->TCR = 1;		
	
	#else
	
	//U32 u32Timer32_0_period;

	NVIC_DisableIRQ(TIMER_32_0_IRQn);
	LPC_CT32B0->TCR = 0;

	/* Some of the I/O pins need to be clearfully planned if
   	 you use below module because JTAG and TIMER CAP/MAT pins are muxed. */

	/* Some of the I/O pins need to be clearfully planned if
	you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT9;

	LPC_CT32B0->PR = Timer32_MHz_Prescale;

	/* Setup the external match register */
	//LPC_CT32B0->EMR = (3 << EMC3);
	//LPC_CT32B0->EMR = (3 << EMC2)|(3 << EMC1)|(3 << EMC0) |(3 << EMC3) |MATCH3|MATCH1;
	//LPC_CT32B0->EMR = (3 << EMC1)|(3 << EMC3) |MATCH1|MATCH3;
	LPC_CT32B0->EMR = (2 << EMC3) | MATCH3;
	//LPC_CT32B0->EMR = (3 << EMC1)|MATCH1;

	/* Setup the outputs */
	/* If match3 is enabled, set the output, use location 0 for test. */
	LPC_IOCON->PIO1_27 &= ~0x07;
	LPC_IOCON->PIO1_27 |= 0x01;		/* Timer0_32 MAT3 */

	/* Enable the selected PWMs and enable Match3 */
	LPC_CT32B0->PWMC = PWM_MATCH_ENABLE;

	/* Setup the match registers */
	/* set the period value to a global variable */
	u32_Timer32_0_period = Timer16_IntervaluS*2500;
	u32_duty_x= period;
	u32_opposite=0;
	if(u32_duty_x<94){
		u32_duty_temp = u32_duty_x;
	}else if(u32_duty_x<188){
		u32_duty_temp =u32_duty_x - 94;
		u32_opposite=1;
	}else{
		u32_duty_temp=0;
	}
	if(u32_duty_temp<64){			
		u32_duty_temp=256-4*u32_duty_temp;
	}else{
		u32_duty_temp=0;
	}
	if(!u32_opposite)
		u32_duty_temp =256-u32_duty_temp;
	LPC_CT32B0->MR3 = u32_Timer32_0_period/2;
	//LPC_CT32B0->MR1 = u32_Timer32_0_period*u32_duty_temp/256 ;	
	//LPC_CT32B0->MR1 = u32_Timer32_0_period/4 ;	
	
	//LPC_CT32B0->MCR = BIT9;// | BIT10;//| BIT3;	/* Reset on MR3 */
	u32_times=0;
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
	LPC_CT32B0->TCR = 1;
	#endif
	
}
//#endif

#endif


void InitCT32B1(unsigned int PR ,unsigned int Time)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10) ;							//Set AHB Clock To The CT16B0 Domain
	//-----
// 	LPC_IOCON->TMS_PIO0_12 &= ~0x07 ;
// 	LPC_IOCON->TMS_PIO0_12 |= 0x03 ;                                //CAP0
// 	LPC_IOCON->TDO_PIO0_13 &= ~0x07 ;
// 	LPC_IOCON->TDO_PIO0_13 |= 0x03 ;								//MAT0
// 	LPC_IOCON->TRST_PIO0_14 &= ~0x07 ;
// 	LPC_IOCON->TRST_PIO0_14 |= 0x03 ;								//MAT1
// 	LPC_IOCON->SWDIO_PIO0_15 &= ~0x07 ;
// 	LPC_IOCON->SWDIO_PIO0_15 |= 0x03 ;								//MAT2
// 	LPC_IOCON->PIO0_16 &= ~0x07 ;
// 	LPC_IOCON->PIO0_16 |= 0x02 ;									//MAT3

// 	LPC_IOCON->PIO1_0 &= ~0x07 ;
// 	LPC_IOCON->PIO1_0 |= 0x01 ;								        //MAT0
// 	LPC_IOCON->PIO1_1 &= ~0x07 ;
// 	LPC_IOCON->PIO1_1 |= 0x01 ;									    //MAT1
// 	LPC_IOCON->PIO1_2 &= ~0x07 ;
// 	LPC_IOCON->PIO1_2 |= 0x01 ;								        //MAT2
// 	LPC_IOCON->PIO1_3 &= ~0x07 ;
// 	LPC_IOCON->PIO1_3 |= 0x01 ;                                     //MAT3
// 	LPC_IOCON->PIO1_4 &= ~0x07 ;
// 	LPC_IOCON->PIO1_4 |= 0x01 ;                                     //CAP0
// 	LPC_IOCON->PIO1_5 &= ~0x07 ;
// 	LPC_IOCON->PIO1_5 |= 0x02 ;                                     //CAP1    
	//-----
	LPC_CT32B1->PR = PR ;
    LPC_CT32B1->MR0 = Time ;
	LPC_CT32B1->MR1 = Time ;
	LPC_CT32B1->MR2 = Time ;
	LPC_CT32B1->MR3 = Time ;
// 	LPC_CT32B1->PWMC = 0x0F ;                                       //Set PWM Out (BIT0:MAT0 , BIT1:MAT1 , BIT2:MAT2 , BIT3:MAT3)
    LPC_CT32B1->MCR = (3<<9) ;                                      //Setup Interrupt Fg & Reset TC
    LPC_CT32B1->TCR = 1 ;
    NVIC_EnableIRQ(TIMER_32_1_IRQn) ;
    return ;
}

