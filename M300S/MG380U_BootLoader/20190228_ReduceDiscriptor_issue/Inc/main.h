/*----------------------------------------------------------------------------
 *      Name:    MAIN.H
 *      Purpose: USB HID Demo Definitions
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
 *---------------------------------------------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"
#include "usbcfg.h"

#ifdef _MAIN_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define P502 1

#if 0
#define _DEBUG_MAIN_(x) x
#else
#define _DEBUG_MAIN_(x) 
#endif

///==========================================
//#define M_Code	
#ifdef M_Code

#define M_Macro_Key
#define M_EVENT6_7
#endif

#ifdef M_FW_UPDATE
#define M_EVENT4_5
#endif
///==========================================

#define EP1_IN BIT0
#define EP1_OUT BIT1
#define EP2_IN BIT2
#define EP2_OUT BIT3
#define EP3_IN BIT4
#define EP3_OUT BIT5
#define EP4_IN BIT6
#define EP4_OUT BIT7
#define BL_UPGRADE	0xAC

extern unsigned char FlashData[256] ;
typedef enum _STATIC_MACHINE_EVENT1_TYPE_
{
	SM_EVENT1_IDLE = 0, 	
	SM_EVENT1_MOUSE_MONITOR,	
	SM_EVENT1_KEYBOARD_MONITOR,	
	SM_EVENT1_TYPE_NUMS
}STATIC_MACHINE_EVENT1_TYPE;

typedef enum _STATIC_MACHINE_EVENT2_TYPE_
{
	SM_EVENT2_IDLE = 0, 	
	SM_EVENT2_BUTTON_HANDLER,
	SM_EVENT2_TYPE_NUMS
}STATIC_MACHINE_EVENT2_TYPE;

typedef enum _STATIC_MACHINE_EVENT3_TYPE_
{
	SM_EVENT3_IDLE = 0, 	
	SM_EVENT3_MACRO_HANDLER,
	SM_EVENT3_TYPE_NUMS
}STATIC_MACHINE_EVENT3_TYPE;

typedef enum _STATIC_MACHINE_EVENT4_TYPE_		//Set request
{
	SM_EVENT4_IDLE = 0,
	SM_EVENT4_GET_FIRMWAVE_VERSION =0x41,
	SM_EVENT4_FIRMWARE_UPDATE = 0x50,	
	SM_EVENT4_FIRMWARE_DATA = 0x51,
	SM_EVENT4_EXIT_BOOTLOADER = 0x52
}STATIC_MACHINE_EVENT4_TYPE;

typedef enum _STATIC_MACHINE_EVENT5_TYPE_
{
	SM_EVENT5_IDLE = 0, 	
	SM_EVENT5_GET_FIRMWAVE_VERSION =0x41,
	SM_EVENT5_FIRMWARE_UPDATE = 0x50,	
	SM_EVENT5_FIRMWARE_DATA = 0x51,
	SM_EVENT5_EXIT_BOOTLOADER = 0x52,
	SM_EVENT5_Other_Command
}STATIC_MACHINE_EVENT5_TYPE;

typedef enum _CMD_STATUS_
{
SUCCESS=0x00,
BUSY=0x01,
FAILED=0x02,
NOTSUPPORT=0x03
}CMD_STATUS;

INTERFACE STATIC_MACHINE_EVENT1_TYPE g_StaticMachineEvent1Type;		//Sensor Key monitor
INTERFACE STATIC_MACHINE_EVENT2_TYPE g_StaticMachineEvent2Type;		//Key parser
INTERFACE STATIC_MACHINE_EVENT3_TYPE g_StaticMachineEvent3Type;		//Macro key
#ifdef M_EVENT4_5
INTERFACE STATIC_MACHINE_EVENT4_TYPE g_StaticMachineEvent4Type;		//Set report
INTERFACE STATIC_MACHINE_EVENT5_TYPE g_StaticMachineEvent5Type;		//Get report
INTERFACE  CMD_STATUS    DCMD_STATUS;
//INTERFACE  CMD_ID        HCMD_ID;
#endif
#ifdef M_EVENT6_7
INTERFACE STATIC_MACHINE_EVENT6_TYPE g_StaticMachineEvent6Type;		//Set report
INTERFACE STATIC_MACHINE_EVENT7_TYPE g_StaticMachineEvent7Type;		//Get report
#endif




//typedef enum _CMD_ID_
//{
//FW_UPDATE=0x50,
//FW_DATA=0x51,
//EXITBOOLOADER=0x52
//}CMD_ID;

typedef enum _GIndex_
{
    GIdx0 = 0,
    GIdx1,
    GIdx2,
    GIdx3,
    GIdx4,
    GIdx5,
    GIdx6,
    GIdx7,
    GIdx8,
    GIdxLen =  GIdx8,
}GIndex;

typedef enum _PROFILEIDX_
{
	PROFILE_DEFAULT=0,
	PROFILE_1,
	PROFILE_2,
	PROFILE_3,
	PROFILE_ALL,
	LED_EFFECT_ALWAYS=0,
	LED_EFFECT_BREATH,
	LED_EFFECT_COLOR_CYCLE,
	LED_EFFECT_RANDON,
	LED_REGION_LOGO=0,
	LED_REGION_WHEEL,
	LED_REGION_SIDE,
	LED_REGION_ALL
}PROFILEIDX;

#define MAIN_FLAG_MOUSE_QIE_IN_REPORT BIT1
#define MAIN_FLAG_MOUSE_SENSOR_IN_REPORT BIT2
#define MAIN_FLAG_MOUSE_SENSOR_FRAME_CAPTURE_EVENT BIT3

//2016_0411_Hsiang
#define MAIN_FLAG_MOUSE_REPORT_READY	BIT0	
#define MAIN_FLAG_MOUSE_BUTTON_EVENT	BIT1	
#define MAIN_FLAG_MOUSE_QIE_EVENT		BIT2
#define MAIN_FLAG_MOUSE_EVENT			BIT3


/* HID Demo Variables */
#define MOUSE_IN_REPORT_SIZE 6
#define MEDIA_IN_REPORT_SIZE 8
#define SYSTEM_IN_REPORT_SIZE	1
#define KEYBOARD_IN_REPORT_SIZE 9					//EP2    hawk
#define SHORTCUT_IN_REPORT_SIZE 8

#define CONSUMER_IN_REPORT_SIZE 3 //EP2			hawk
#define CUSTOMER_IN_REPORT_SIZE 64 //EP3		hawk
#define CUSTOMER_OUT_REPORT_SIZE 64 //EP3  hawk

#define FWUPDATE_IN_REPORT_SIZE 64  //EP4
#define FWUPDATE_OUT_REPORT_SIZE 64 //EP4
#define MACRO_PRESS_ANY			0
#define MACRO_RELEASE_KEY		1


#define MAX_PROFILE	3
#define MAX_MARCO	0xFFFF
#define EEPROM_SIZE 0x1000
#define EERPOM_CHECK_ADDR 4030



#define KEY_EFFECT_CHOOSE 0x28


//#define P503 1
	
typedef __packed struct _LPC11Uxx_MAIN_SETTING_
{	
	U8 MouseInReport[MOUSE_IN_REPORT_SIZE];
	U8 MediaInReport[MEDIA_IN_REPORT_SIZE];
	U8 SystemInReport[SYSTEM_IN_REPORT_SIZE];
	U8 KeyboardInReport[KEYBOARD_IN_REPORT_SIZE];
	U8 ShortcutInReport[SHORTCUT_IN_REPORT_SIZE];
	U8 ConsumerInReport[CONSUMER_IN_REPORT_SIZE];

	U8 FeatureInReport[16];	
	//U8 FeatureInReport[MOUSE_IN_REPORT_SIZE];
	//U8 FeatureInReport[USB_PACKET_SIZE];
	U8 InReport[CUSTOMER_IN_REPORT_SIZE];		// hawk
	U8 OutReport[CUSTOMER_OUT_REPORT_SIZE];	// hawk
  U8 FirmwareUpdateInReport[FWUPDATE_IN_REPORT_SIZE];
  U8 FirmwareUpdateOutReport[FWUPDATE_OUT_REPORT_SIZE];
	//U8 InReportSize;
	U16 EPInOutReq;
	//U8  EP0_SetReq;
	//U8  EP0_GetReq;
	U8 InReportSize;						//2016_0411_Hsiang
	U16 MouseEvent;						//2016_0411_Hsiang
	U16 Total_Size;
	U16 ADR_Offset;
	U8 SensorReset;
	U8 MacroStopMode;
   	 U8 OS_ModeType;
}LPC11Uxx_MAIN_SETTING;

INTERFACE LPC11Uxx_MAIN_SETTING g_Main_Setting;

typedef struct _SENSOR_DATA_
{
	U8	X1;
	U8	X2;
	U8	Y1;
	U8	Y2;
	U8	Changed;
}SENSOR_DATA;

typedef struct _REPROT_PROCESS_
{
	U8	Mouse_Report_In;
	U8	Media_Report_In;
	U8	System_Report_In;
	U8	Keyboard_Report_In;
	U8	Macro_Keyboard_Report_In;
	U8 	Shortcut_Report_In;
	U8	Consumer_Report_In;

	U8	Feature_Report_In;
  U8  FirmwareUpdate_Report_In;
}REPORT_PROCESS;

typedef struct _KEY_MATRIX_ATTR_
{
	U16 KeyDelay;
	U8	fKeyDelay;
	U8 KeyAction;
	U8 KeyTableNum;
	U8 KeyLoop;
	U8 DecKeyLoop;
}KEY_MATRIX_ATTR;

typedef struct _SET_MATRIX_A_
{
	U8	Key_Group;
	U8	Action;
	U8	M_Data_Byte;
}SET_MATRIX_A;

typedef struct _SET_TEMP_INSTANT_
{
	U8 u8InstantProfile;
	U8 u8InstantKyeNum;
	U8 u8InstantKyeEnable;
}SET_TEMP_INSTANT;




INTERFACE SET_MATRIX_A	g_Set_Matrix;
INTERFACE SENSOR_DATA	g_Sensor_Data;
INTERFACE REPORT_PROCESS	g_Repot_Process;
INTERFACE KEY_MATRIX_ATTR	g_Key_Matrix_Attr;
INTERFACE SET_TEMP_INSTANT  g_Instant_Key;


/* HID Demo Functions */
INTERFACE void nsAPI_Main_FirmwareUpdateInReportHandler(void);
INTERFACE void nsAPI_Main_GetInReport(void);
INTERFACE void nsAPI_Main_SetOutReport(void); 
INTERFACE void nsAPI_Main_MouseInReportHandler(void);
INTERFACE void nsAPI_Main_MediaInReportHandler(void);
INTERFACE void nsAPI_Main_SystemInReportHandler(void);
INTERFACE void nsAPI_Main_KeyboardInReportHandler(void);
INTERFACE void nsAPI_Main_ShortcutInReportHandler(void);
INTERFACE void nsAPI_Main_MouseButtonEventHandler(void);
INTERFACE void nsAPI_Main_KeyboardButtonEventHandler(void);
INTERFACE void nsAPI_Main_GetInMouseQIEReport(S8 s8Temp);
INTERFACE void nsAPI_Main_Sensor_InReport(U8 * u8pTemp);
INTERFACE void nsAPI_Main_ConsumerInReportHandler(void);
INTERFACE void nsAPI_Main_Clr_Mouse_Event(void);
INTERFACE void nsAPI_Main_Clr_EP1Event(void);
INTERFACE void nsAPI_Main_Clr_KeyboardEvent(void);
INTERFACE void nsAPI_Main_Clr_ConsumerEvent(void);
INTERFACE void nsAPI_Main_Clr_EP3Event(void);  //HAWK
#ifdef M_EVENT4_5
INTERFACE void nsAPI_Main_Clr_EP4Event(void);
#endif

INTERFACE void Mapp_Main_Init(void);
INTERFACE void MacroKey_Handler(void);
INTERFACE void MarcoKey_Release_Key(void);
INTERFACE void Mapp_StaticMachineEvent1(void);
INTERFACE void Mapp_StaticMachineEvent2(void);
INTERFACE void Mapp_StaticMachineEvent3(void);
#ifdef M_EVENT4_5
INTERFACE void Mapp_StaticMachineEvent4(void);
INTERFACE void Mapp_StaticMachineEvent5(void);
#endif
#ifdef M_EVENT6_7
INTERFACE void Mapp_StaticMachineEvent6(void);
INTERFACE void Mapp_StaticMachineEvent7(void);
#endif
void boot_jump(void);
void nsAPI_Main_FirmwareReportHandler(void);
extern U16 BUTTON_MAP_DEFAULT[];
extern U16 BUTTON_MAP_NORMAL[];
extern U8 USB_BUFFER[];




#undef INTERFACE
#ifdef __cplusplus
}
#endif
#endif //__MAIN_H__
