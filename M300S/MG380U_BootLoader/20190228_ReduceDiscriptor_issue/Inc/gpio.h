/****************************************************************************
 *   $Id:: gpio.h 6172 2011-01-13 18:22:51Z usb00423                        $
 *   Project: NXP LPC11xx software example
 *
 *   Description:
 *     This file contains definition and prototype for GPIO.
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"
#include "MouseDatabase.h"
#include "systick.h"

#ifdef  __GPIO_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif


#if 0
#define _DEBUG_GPIO_(x) x
#else
#define _DEBUG_GPIO_(x)
#endif

#define IOCON_MODE_INACT        (0x0 << 3)		/*!< No addition pin function */
#define IOCON_MODE_PULLDOWN     (0x1 << 3)		/*!< Selects pull-down function */
#define IOCON_MODE_PULLUP       (0x2 << 3)		/*!< Selects pull-up function */
#define IOCON_DIGMODE_EN        (0x1 << 7)		/*!< Enables digital function (analog pins only) */


#define PORT0 0
#define PORT1 1

#define GROUP0 0
#define GROUP1 1

#define GPIOIN 0
#define GPIOOUT 1

#define GPIO_SCANKEY_NULL 0
#define GPIO_SCANKEY_PRH BIT0
#define GPIO_SCANKEY_RELEASE BIT1
#define GPIO_SCANKEY_FW_RELEASE BIT2
#define GPIO_KEY_CHANGED	0
#define GPIO_KEY_SAME		1


#define LED_STATUS_OFF      0
#define LED_STATUS_LOW      1
#define LED_STATUS_MEDIUM   2
#define LED_STATUS_HIGH     3

//LED_BREATHING;
#define LED_TIMER32_BASE       1
#define BREATHE_USE_SYSTICK     1

#define BREATHE_TIMER_BASE          10 //ms

#define LED_BREATHING_OFF           0 //unit 0.01sec
#define LED_BREATHING_LOW           (2000/BREATHE_TIMER_BASE)// 2 sec //2000msec
#define LED_BREATHING_MEDIUM        (1500/BREATHE_TIMER_BASE)// 1.5sec //1500msec
#define LED_BREATHING_HIGH          (1000/BREATHE_TIMER_BASE)// 1 sec //1000msec
#if 0
#define LED_BREATHING32_OFF         0 //unit 0.1sec
#define LED_BREATHING32_LOW         (2000/BREATHE_TIMER_BASE)// 2 sec //2000msec
#define LED_BREATHING32_MEDIUM      (1500/BREATHE_TIMER_BASE)// 1.5sec //1500msec
#define LED_BREATHING32_HIGH        (1000/BREATHE_TIMER_BASE)// 1 sec //1000msec
#endif
#define LED_BREATHING_B_TIME        0 // 0 sec
#define LED_BREATHING_C_TIME        (500/BREATHE_TIMER_BASE)// 0.5 sec //500msec
#define LED_BREATHING_C_TIME32      (500/BREATHE_TIMER_BASE)// 0.5 sec //500msec

#define LED_PWM_LOW_COUNTER     1
#define LED_PWM_MID_COUNTER     1
#define LED_PWM_HIGH_COUNTER    1

#define LED_BREATHING_STEP_A    0 // Low to High
#define LED_BREATHING_STEP_B    1 // keep High
#define LED_BREATHING_STEP_C    2 // High to Low
#define LED_BREATHING_STEP_D    3 // keep High

//Set DPI Level LED
//Port0_Pin
#define LED_DPI_LEVEL_5     20
#define LED_DPI_LEVEL_4     17
#define LED_DPI_LEVEL_3     12
#define LED_DPI_LEVEL_2     7
#define LED_DPI_LEVEL_1     2
//Set DPI Level LED

#define GPIO_INACTIVE 0
#define GPIO_PULL_DOWN 1
#define GPIO_PULL_UP 2
#define GPIO_REPEATE 3

#define GPIO_FUNC_MODE(x, y)	\
	do{							\
		x &=~0x07;				\
		x |= (y<<3);					\
	}while(0);

//#define GPIO0_PR (0xFFFFFFFF)
//#define GPIO1_PR (BIT27|BIT26|BIT25|BIT24|BIT23)

typedef enum _QIEStatusIndex_
{
    QIE_STOP = 0x00,
    QIE_FORWARD = 0xFF,
    QIE_BACKWARD = 0x01,
    QIE_SAME = 0xAA,
}QIEStatusIndex;

#ifdef P502
//#if 1
//#define BUTTON_LEFT_PIN_CONFIG 	LPC_IOCON->PIO1_24
	#define BUTTON_LEFT_PIN 			24
	#define BUTTON_LEFT_PORT 			PORT1
	//#define BUTTON_MIDDLE_PIN_CONFIG 	LPC_IOCON->PIO0_19
	#define BUTTON_MIDDLE_PIN 			19
	#define BUTTON_MIDDLE_PORT 		PORT0
	//#define BUTTON_RIGHT_PIN_CONFIG 	LPC_IOCON->PIO0_7
	#define BUTTON_RIGHT_PIN 			7
	#define BUTTON_RIGHT_PORT 			PORT0
	//#define BUTTON_SIDE1_PIN_CONFIG 	LPC_IOCON->PIO1_13
	#define BUTTON_SIDE1_PIN 			13
	#define BUTTON_SIDE1_PORT 			PORT1
	//#define BUTTON_SIDE2_PIN_CONFIG 	LPC_IOCON->PIO1_14
	#define BUTTON_SIDE2_PIN 			14
	#define BUTTON_SIDE2_PORT 			PORT1
	//#define BUTTON_SIDE3_PIN_CONFIG 	LPC_IOCON->PIO1_16
	#define BUTTON_SIDE3_PIN 			16
	#define BUTTON_SIDE3_PORT 			PORT1
	//#define BUTTON_DPI_PIN_CONFIG 		LPC_IOCON->TMS_PIO0_12
	#define BUTTON_DPI_PIN 				12
	#define BUTTON_DPI_PORT 			PORT0
#else
	#define BUTTON_LEFT_PIN 			19
	#define BUTTON_LEFT_PORT 			PORT0

	#define BUTTON_MIDDLE_PIN 			14
	#define BUTTON_MIDDLE_PORT 		PORT1

	#define BUTTON_RIGHT_PIN 			16
	#define BUTTON_RIGHT_PORT 			PORT1

	#define BUTTON_L_SIDE1_PIN 		13
	#define BUTTON_L_SIDE1_PORT 		PORT1

	#define BUTTON_L_SIDE2_PIN 		14
	#define BUTTON_L_SIDE2_PORT 		PORT0

	#define BUTTON_R_SIDE1_PIN 		13
	#define BUTTON_R_SIDE1_PORT 		PORT0

	#define BUTTON_R_SIDE2_PIN 		12
	#define BUTTON_R_SIDE2_PORT 		PORT0

	#define BUTTON_DPI_PIN 				11
	#define BUTTON_DPI_PORT 			PORT0
#endif

#if 0 // no use
typedef enum _eMOUSE_SYSTICK_MONITOR_TYPE_
{
	eSYSTICK_IDLE = 0,
	eSYSTICK_MONITOR,
	eSYSTICK_FINISH,
	eSYSTICK_SIZE,
	eSYSTICK_EMPTY = 0xFF
}eMOUSE_SYSTICK_MONITOR_TYPE;

typedef enum _eMOUSE_KEYBOARD_MONITOR_TYPE_
{
	eKEYBOARD_IDLE = 0,
	eKEYBOARD_INIT,
	eKEYBOARD_BUSY,
	eKEYBOARD_FINISH,
	eKEYBOARD_NEXT,
	eKEYBOARD_SIZE,
	eKEYBOARD_EMPTY = 0xFF
}eMOUSE_KEYBOARD_MONITOR_TYPE;


typedef __packed struct _LPC11Uxx_GPIO_DATABASE_TEMP_BUTTON_STATUS_
{
	eMOUSE_SYSTICK_MONITOR_TYPE eMouseButtonGroupSystickMonitor;
	eMOUSE_KEYBOARD_MONITOR_TYPE eMouseButtonGroupStatusMonitor;
	U32 tMouseButtonGroupTempDelay;
}LPC11Uxx_GPIO_DATABASE_TEMP_BUTTON_STATUS;


typedef __packed struct _LPC11Uxx_GPIO_DATABASE_TEMP_BUTTON_
{
	U8  gdbMouseButtonProfile;
	U8  gdbMouseButtonIndex;
	U8  gdbMouseButtonAssignmentType;

	U8  gdbMouseButtonGroupTotalData;/*Button Data total group*/
	U8  gdbMouseButtonGroupTotalDataIndex;/*Button Data total group index*/
	U8  gdbMouseButtonGroupData0;
	U8  gdbMouseButtonGroupData1;
	U8  gdbMouseButtonGroupReleaseDelyTimes;/*For release timer*//*1 -> 50ms ~ 255 -> 12750 ms */
	U8  gdbMouseButtonGroupRecycle;/*Recycle times*/

	LPC11Uxx_GPIO_DATABASE_TEMP_BUTTON_STATUS tLGDTBS;

}LPC11Uxx_GPIO_DATABASE_TEMP_BUTTON;
#endif

typedef struct _LPC11Uxx_GPIO_KEYBOARD_SETTING_
{
	U8 PreQIEStatus;
	U8 KeyValue;
	U16 ButtonStatus;
}LPC11Uxx_GPIO_KEYBOARD_SETTING;

typedef struct _GPIO_KEYBOARD_
{
	U8 KeyValue;
	U8 PreKeyValue;
	U8 ButtonStatus;
	U8 Debounce;
	U8 X_DPI_Press;
	U8 Y_DPI_Press;
}GPIO_KEYBOARD;

typedef struct _QIEAction_
{
	U8 QIE_DEBOUNCE;
	U8 QIE_DIR;
	U8 QIE_STATUS;
}QIE_ACTION;

INTERFACE QIE_ACTION QIEAction;
//INTERFACE U8 g_u8LED_Status;


INTERFACE LPC11Uxx_GPIO_KEYBOARD_SETTING g_GpioButtonEvent;
INTERFACE GPIO_KEYBOARD g_GpioKeyboard;

INTERFACE void FLEX_INT0_IRQHandler(void);
INTERFACE void FLEX_INT1_IRQHandler(void);
INTERFACE void FLEX_INT2_IRQHandler(void);
INTERFACE void FLEX_INT3_IRQHandler(void);
INTERFACE void FLEX_INT4_IRQHandler(void);
INTERFACE void FLEX_INT5_IRQHandler(void);
INTERFACE void FLEX_INT6_IRQHandler(void);
INTERFACE void FLEX_INT7_IRQHandler(void);
INTERFACE void GINT0_IRQHandler(void);
INTERFACE void GINT1_IRQHandler(void);
INTERFACE void nsAPI_GPIO_Init( void );
INTERFACE void NDrv_GPIO_SetBitValue(U8 portNum, U8 bitPosi, U8 bitVal );
INTERFACE RetU32 NDrv_GPIO_GetPinValue( U8 portNum, U32 bitPosi);
INTERFACE void NDrv_GPIO_SetDir( U8 portNum, U8 bitPosi, U8 dir );
INTERFACE void GPIOSetIOConfig(uint8_t io_port, uint8_t io_num, uint8_t io_fun, uint8_t io_mode);
INTERFACE void nsAPI_GPIO_LEDSetting(U8 uPort, U32 uPosition, U8 uValue);
//INTERFACE RetU8 NDrv_GPIO_ScanButton(void);
//INTERFACE void nsAPI_GPIO_MouseButtonMonitor(void);
//INTERFACE void nsAPI_GPIO_ChangingIndex(U16 u16IPreButtonEvent,U8 u8ButtonClassIndex);
INTERFACE void nsAPI_GPIO_Reset_ADNS9800(void);
//INTERFACE U8 nsAPI_GPIO_Get_QIE(void);
//INTERFACE void nsAPI_GPIO_QIE_Monitor(void);
//INTERFACE void nsAPI_GPIO_Macrokey_Monitor(void);
//INTERFACE eMOUSE_MOUSE_BUTTON_INDEX nsAPI_GPIO_Button_Change(U16 u16Button);
//INTERFACE LPC11Uxx_GPIO_DATABASE_TEMP_BUTTON nsAPI_GPIO_Button_Get_ProfileKey(U8 u8ProfileGroupIndex,U8 u8ButtonIndex, U8 u8ButtonClassIndex);
//INTERFACE RetU8 nsAPI_GPIO_Button_Change_Index(U16 u16Key);
//INTERFACE void nsAPI_GPIO_ChangingIndexRelease(U8 u8AssignmentType);
//INTERFACE void nsAPI_GPIO_Keyboard_Macro_Engine(void);
INTERFACE void nsAPI_GPIO_ButtonEventMointor(void);
INTERFACE void GPIO_Check_Key(void);
INTERFACE void nsAPI_GPIO_LedDisable(BOOLEAN bEnable);
INTERFACE void nsAPI_PWM_ChangeTo_GPIO(void);
INTERFACE void GPIOSetGroupedInterrupt( uint32_t groupNum, uint32_t *bitPattern, uint32_t logic,uint32_t sense, uint32_t *eventPattern ); //(Add) 20120525

#undef INTERFACE
#ifdef __cplusplus
}
#endif
#endif /* end __GPIO_H */

