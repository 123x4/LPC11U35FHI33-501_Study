#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"

#ifdef __SYSTICK_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define SYSTICK_TIMER (SystemCoreClock /1000)/*1ms*/
#define SYSTICK_TIMER_10MS  (SYSTICK_TIMER*10) //10ms
#define SYSTICK_TIMER_5MS  (SYSTICK_TIMER*5) //5ms

/*
typedef enum _BREATHING_COLOR_TYPE_
{
	COLOR_TYPE_R = 0,
	COLOR_TYPE_G,
	COLOR_TYPE_B,
	COLOR_TYPE_NUMS
}BREATHING_COLOR_TYPE;


typedef  struct _LPC11Uxx_SYSTICK_TIMER_BREATHING_SETTING_
{
	U16 u16RGB[COLOR_TYPE_NUMS];
	U16 u16RGBMax[COLOR_TYPE_NUMS];
	U16 u16BreathingSetting;
	U16 u16BreathingMax;
	U16 u16BreathingCounterRGB[COLOR_TYPE_NUMS];
	U8 u8RGBStatus[COLOR_TYPE_NUMS];
}LPC11Uxx_SYSTICK_TIMER_BREATHING_SETTING;
*/
//INTERFACE LPC11Uxx_SYSTICK_TIMER_BREATHING_SETTING g_Systick_Timer16_Breathing_Setting;
//INTERFACE LPC11Uxx_SYSTICK_TIMER_BREATHING_SETTING g_Systick_Timer32_Breathing0_Setting;
//INTERFACE LPC11Uxx_SYSTICK_TIMER_BREATHING_SETTING g_Systick_Timer32_Breathing1_Setting;




INTERFACE void SysTick_Handler(void);
INTERFACE void nsAPI_SysTick_Init(void);
//INTERFACE void nsAPI_Systick_Timer_Breathing(LPC11Uxx_SYSTICK_TIMER_BREATHING_SETTING * ptTimer_Breathing,U8 u8RGB);

//INTERFACE void nsAPI_SysTick_Macro_Button_Event_Handle(void);
#undef INTERFACE
#ifdef __cplusplus
}
#endif
#endif /* end __SYSTICK_H */


