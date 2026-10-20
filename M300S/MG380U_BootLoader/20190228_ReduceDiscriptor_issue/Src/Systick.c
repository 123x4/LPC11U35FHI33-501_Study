#define __SYSTICK_C__

#include "LPC11Uxx.h"
#include "core_cm0.h"
#include "Mouse.h"
#include "Gpio.h"
#include "Uart.h"
#include "Systick.h"
#include "Timer16.h"
#include "Timer32.h"
#include "Main.h"
#include "Ssp.h"
#include "Keytbl.h"
#include "Led.h"


#define SYS_TICK_MS     10
bool tick_flag=0;
extern bool LED_COLOR_CHANGE_FLAG;
extern U16 LED_Rainbow_Timer;
extern U16 LED_Colorcycle_Timer;
extern U16 LED_Breath_Timer;

//extern bool toggle_flag;
void SysTick_Handler(void)
{
	//S_DPIChange_LED();	//2016_0523_Hsiang
//	S_KeyScan_BounceCheck();		// Base for 1mSec
	if(LED_Rainbow_Timer)
		LED_Rainbow_Timer--;
	if(LED_Breath_Timer)
		LED_Breath_Timer--;
	if(LED_Colorcycle_Timer)
		LED_Colorcycle_Timer--;
	
	if(g_Key_Matrix_Attr.fKeyDelay == 1){
		g_Key_Matrix_Attr.KeyDelay++;
		g_Key_Matrix_Attr.fKeyDelay = 0;
	}
	else{
		g_Key_Matrix_Attr.fKeyDelay = 1;
	}

	//NDrv_GPIO_SetBitValue(PORT1, 26, LOW);
}

#if 0
void nsAPI_Systick_Timer_Breathing(LPC11Uxx_SYSTICK_TIMER_BREATHING_SETTING * ptTimer_Breathing,U8 u8RGB )
{
	if(ptTimer_Breathing->u8RGBStatus[u8RGB] == LED_BREATHING_STEP_A)// Low to High
	{
		if(ptTimer_Breathing->u16BreathingCounterRGB[u8RGB] > 0)
		{
			ptTimer_Breathing->u16BreathingCounterRGB[u8RGB]--;
			ptTimer_Breathing->u16RGB[u8RGB] = \
			ptTimer_Breathing->u16RGB[u8RGB] - ((TIME_CT32B0_COUNT_OFF - ptTimer_Breathing->u16RGBMax[u8RGB]) / ptTimer_Breathing->u16BreathingMax);
		}
		else
		{
			ptTimer_Breathing->u16BreathingCounterRGB[u8RGB] = LED_BREATHING_B_TIME;
			ptTimer_Breathing->u8RGBStatus[u8RGB] = LED_BREATHING_STEP_B;
		}
	}
	else if (ptTimer_Breathing->u8RGBStatus[u8RGB] == LED_BREATHING_STEP_B)//Keep High , must be add counter
	{
		ptTimer_Breathing->u16RGB[u8RGB] = ptTimer_Breathing->u16RGBMax[u8RGB];

		if(ptTimer_Breathing->u16BreathingCounterRGB[u8RGB] > 0)
		{
			ptTimer_Breathing->u16BreathingCounterRGB[u8RGB]--;
		}
		else
		{
			ptTimer_Breathing->u16BreathingCounterRGB[u8RGB] = LED_BREATHING_C_TIME;
			ptTimer_Breathing->u8RGBStatus[u8RGB] = LED_BREATHING_STEP_C;
		}
	}
	else if (ptTimer_Breathing->u8RGBStatus[u8RGB] == LED_BREATHING_STEP_C)// High to Low //Fix 0.5sec
	{
		if (ptTimer_Breathing->u16BreathingCounterRGB[u8RGB] > 0)
		{
			ptTimer_Breathing->u16BreathingCounterRGB[u8RGB]--;
			ptTimer_Breathing->u16RGB[u8RGB] = \
			ptTimer_Breathing->u16RGB[u8RGB] + ((TIME_CT32B0_COUNT_OFF - ptTimer_Breathing->u16RGBMax[u8RGB]) / LED_BREATHING_C_TIME);
		}
		else
		{
			ptTimer_Breathing->u16BreathingCounterRGB[u8RGB] = ptTimer_Breathing->u16BreathingMax;
			ptTimer_Breathing->u8RGBStatus[u8RGB] = LED_BREATHING_STEP_A;
		}
	}
}
#endif
//-------------------------------------------------------------------------------------------------------
void nsAPI_SysTick_Init(void)
{
	NVIC_DisableIRQ(SysTick_IRQn);

	if(SysTick_Config(SYSTICK_TIMER/*ms*/))/* Setup SysTick Timer for 1 msec interrupts  */
	{
		while(1){}/*Systick Error*/
	}

//	NVIC_EnableIRQ(SysTick_IRQn);
}


