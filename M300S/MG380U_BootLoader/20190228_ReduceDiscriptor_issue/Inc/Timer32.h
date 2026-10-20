/****************************************************************************
 *   $Id:: timer32.h 6956 2011-03-23 23:03:25Z usb00423                     $
 *   Project: NXP LPC11xx software example
 *
 *   Description:
 *     This file contains definition and prototype for 32-bit timer 
 *     configuration.
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
#ifndef __TIMER32_H__ 
#define __TIMER32_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"
#include "Timer16.h"

#ifdef  __TIMER32_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif

//(Add) 20120503
#define TIME_CT32B0_COUNT TIME_CT16B0_COUNT/*199999*/								//0 To 199999 = 200000
//#define TIME_CT32B0_PR          15									//0 To 0 = 1
#define TIME_CT32B1_COUNT TIME_CT32B0_COUNT							//0 To 199999 = 200000
//#define TIME_CT32B1_PR TIME_CT32B0_PR									//0 To 0 = 1

#define TIME_CT32B0_COUNT_OFF       TIME_CT32B0_COUNT
#define TIME_CT32B0_COUNT_LOW       ((TIME_CT32B0_COUNT/3)<<1)
#define TIME_CT32B0_COUNT_MEDIUM    (TIME_CT32B0_COUNT/3)
#define TIME_CT32B0_COUNT_HIGH      0

#define TIME_CT32B1_COUNT_OFF       TIME_CT32B1_COUNT
#define TIME_CT32B1_COUNT_LOW       ((TIME_CT32B1_COUNT/3)<<1)
#define TIME_CT32B1_COUNT_MEDIUM    (TIME_CT32B1_COUNT/3)
#define TIME_CT32B1_COUNT_HIGH      0

#if 1  // no use
/*LED COLOR SETTING*/
#define TIMER_COLOR_RED BIT0
#define TIMER_COLOR_GREEN BIT1
#define TIMER_COLOR_BLUE BIT2
#define TIMER_COLOR_YELLOW BIT3
#define TIMER32_COLOR_EMPTY 0x00

#define EMC0	4
#define EMC1	6
#define EMC2	8
#define EMC3	10

#define MATCH0 BIT0
#define MATCH1 BIT1
#define MATCH2 BIT2
#define MATCH3 BIT3

#define Timer32_0 0
#define Timer32_1 1

#define BREATHING_FREQ_GAP 100
#define BREATHING_DELAY_TIMER 10
#define BREATHING_STEP 3

#define PWM_MATCH_ENABLE (MATCH0|MATCH1|MATCH2|MATCH3)
//#define PWM_MATCH_ENABLE ( MATCH1)
#define Timer32_IntervalmS 1000UL 
#define Timer32_IntervaluS 1UL 
//#define Timer32_IntervalnS 1UL 
#define Timer32_MHz_Prescale (SystemCoreClock/1000000UL)



typedef enum _Timer32_LED_Type_
{
	LED_NORMAL = 0,
	LED_BREATHING   
}Timer32_LED_Type;

#define BREATHING_STEPSTATUS_NORMAL BIT0
#define BREATHING_STEPSTATUS_FRAMECAPTURE BIT1

typedef __packed struct _LPC11Uxx_TIMER32_LED_COLOR_SETTING_
{
	U8 Timer32_Color;
	U32 Timer32_Color_Red;
	U32 Timer32_Color_Green;
	U32 Timer32_Color_Blue;	
}LPC11Uxx_TIMER32_LED_COLOR_SETTING;


typedef __packed struct _LPC11Uxx_TIMER32_LED_SETTING_
{
	Timer32_LED_Type LEDStatusType;/* 0 -> Normal : 1 -> Breathing */	
	U8 BreathingStatusType;/*1 -> Inc : 0 -> Dec*/
	U8 BreathingStep; 
	U8 BreathingStepStatus; 
	U32 BreathingDelayCount;
	U32 CurBreathingFreq;
	U32 PreBreathingFreq;
	LPC11Uxx_TIMER32_LED_COLOR_SETTING Timer32Color;
}LPC11Uxx_TIMER32_LED_SETTING;

INTERFACE LPC11Uxx_TIMER32_LED_SETTING g_Timer32_LED_Setting; 

INTERFACE void TIMER32_0_IRQHandler(void);
INTERFACE void TIMER32_1_IRQHandler(void);
INTERFACE void NDrv_Timer32_Enable(U8 timer_num);
INTERFACE void NDrv_Timer32_Disable(U8 timer_num);
INTERFACE void NDrv_Timer32_Reset(U8 timer_num);
INTERFACE void nsAPI_Timer32_Setting_MatchFunction(uint8_t timer_num, uint8_t match_enable, uint8_t location);
INTERFACE void nsAPI_Timer32PWM_Init(U8 timer_num, U32 period, U8 match_enable);
INTERFACE void NDrv_Timer32PWM_Breathing(void);
INTERFACE RetU32 nsAPI_Timer32PWM_GetCurBreathingFreqValue(void);
INTERFACE void nsAPI_Timer32PWM_Breathing_ReSetting(U8 u8Breathing);
INTERFACE void nsAPI_Timer32_Setting_Color(U8 u8Color,U32 u32period);
INTERFACE void nsAPI_Timer32_B0M3_PWM_Init(U32 period);
#endif
#if 1
INTERFACE void InitCTnBx(U8 u8CT32Bx,U32 Time);
#else
INTERFACE void InitCT32B0(/*unsigned int PR ,*/unsigned int Time);
INTERFACE void InitCT32B1(/*unsigned int PR ,*/unsigned int Time);
#endif
#undef INTERFACE
#ifdef __cplusplus
}
#endif
#endif /* end __TIMER32_H */

