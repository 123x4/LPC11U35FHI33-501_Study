#include <stdbool.h>
//#include "LPC11Uxx.h"/* LPC11xx Peripheral Registers */

#define V_bcd_FWVer		0x0001 //bcdDevice in by USBVIEW
#define EEPROM_BUTTON_INDEX 0
#define EEPROM_BUTTON_INDEX_LENGTH 8
#define MS_RIGHT_BUTTON_CLICK 		0x01F1
#define MS_MIDDLE_BUTTON_CLICK 	0x01F2
#define MS_LEFT_BUTTON_CLICK 		0x01F0
#define MS_4_BUTTON_CLICK			0x01E4
#define MS_5_BUTTON_CLICK			0x01E5
#define MS_DPI1_BUTTON_CLICK		0x01E6
#define MS_DPI2_BUTTON_CLICK		0x01E7

#define MS_LEFT_BUTTON 	0x01
#define MS_RIGHT_BUTTON 	0x02
#define MS_MIDDLE_BUTTON 	0x04
#define MS_4_BUTTON		0x08
#define MS_5_BUTTON		0x10
#define MS_DPI1_BUTTON		0xD1
#define MS_DPI2_BUTTON		0xD2

extern U8 CUR_PROFILE;
//------------------------------------------------------------------------------------------
struct Led_Info
{
	U8 Effect;
	U8 Brigntness;
	U8 color[3];
};
//------------------------------------------------------------------------------------------
struct Led_Effect_Timers_Info
{
	U16 breath;
	U16 colorcycle;
	U16 colorwave;
};
//------------------------------------------------------------------------------------------
struct Led_Effect_t
{	
	bool Is_All_Effect;
	struct Led_Effect_Timers_Info Times;
	struct Led_Info All;
	struct Led_Info Logo;
	struct Led_Info Wheel;
	struct Led_Info LightPipe;	
};
//------------------------------------------------------------------------------------------
struct Button_Counts_t
{
    	U16 	left;
    	U16 	right;
	U16 	middle;
	U16 	dpi1;
	U16 	dpi2;
	U16 	key4;
	U16 	key5;
};
//------------------------------------------------------------------------------------------
struct KeyPress_t
{
	bool left_button;
	bool righe_button;
	bool middle_button;
	bool dpi1_button;
	bool dpi2_button;
	bool button_4;
	bool button_5;
};
//------------------------------------------------------------------------------------------
struct KeyType_t
{
	//0:MS, 1: KB, 2:Comsume
	U8 InType;
	U8 Value;
};
//------------------------------------------------------------------------------------------
struct Button_Func_t
{
    	struct KeyType_t 	left;
    	struct KeyType_t 	right;
	struct KeyType_t 	middle;
	struct KeyType_t 	dpi1;
	struct KeyType_t 	dpi2;
	struct KeyType_t 	key4;
	struct KeyType_t 	key5;
} ;
//------------------------------------------------------------------------------------------
struct Mouse_Status_t
{
	U16 ver;
    	U8 	current_profile;
	U8 	profile_size;
  	struct KeyPress_t 	key_status;			
	U8 	dpi1_value;							// real value/100
	U8 	dpi2_value;
	U16 	acceleration_value;
	U16 	deceleration_value;
	U16 	polling_rate;
	U16 	response;
	struct Button_Counts_t counts;
};
//------------------------------------------------------------------------------------------
struct Sensor_Calibration_Param_t
{
	U8 min_squal_value;
	U8 pixel_threshold_value;
};
//------------------------------------------------------------------------------------------
struct Mousepad_t
{
	struct Sensor_Calibration_Param_t ROG_Whetstone;
	struct Sensor_Calibration_Param_t ROG_Sheath;
	struct Sensor_Calibration_Param_t Cloth;
	struct Sensor_Calibration_Param_t Plastic;
	struct Sensor_Calibration_Param_t Glass;
	struct Sensor_Calibration_Param_t Metal;
	bool Ignore;
};
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
#if 0
static U8 MS_Button_Default_Function[]=
{
	0x08, 0x10, 0xD1, 0xD2, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04
};
#endif

//#define S_SSP0_Ex_Flash_SSEL(x) NDrv_GPIO_SetBitValue(PORT0,11,x)	

///==============================================================




///-------------------------------------------------------------------------------- LED
/*
#define V_LED_Active			0			// LowActive

#define V_DPI_1_LED_Port	1
#define V_DPI_1_LED_Pin		13

#define V_DPI_2_LED_Port	1
#define V_DPI_2_LED_Pin		14

#define V_DPI_3_LED_Port	1
#define V_DPI_3_LED_Pin		15

///-------------------------------------------------------------------------------- KeyIn Define
#define V_SW_L_Port			1
#define V_SW_L_Pin			24

#define V_SW_M_Port			0
#define V_SW_M_Pin			19

#define V_SW_R_Port			0
#define V_SW_R_Pin			7

#define V_SW_DPI_Port		1
#define V_SW_DPI_Pin		26

#define V_Side_L1_Port		1
#define V_Side_L1_Pin		27

#define V_Side_L2_Port		1
#define V_Side_L2_Pin		29

#define V_Side_R1_Port		1
#define V_Side_R1_Pin		31

#define V_Side_R2_Port		1
#define V_Side_R2_Pin		28

*/
//#define V_KeyScan_Bounce	5		// Base On XXmSec
#define V_KeyScan_Bounce	10		// Base On 1mSec
//#define V_KeyScan_Bounce	500		// for Debug Use


#define V_SW_L		0x0001
#define V_SW_R		0x0002
#define V_SW_M		0x0004

#if 0
#if 1
#define V_SW_1_USB		0x0010
#define V_SW_2_USB		0x0008
#else
#define V_SW_L1_USB		0x0008
#define V_SW_L2_USB		0x0010
#endif

#define V_SW_3_USB		0x0020
#endif
#define V_SW_DPI_USB	0x0080	

#define V_SW_DPI_1		0x0080


#ifdef P502
#define V_SW_DPI_2		0x0020
#define V_Side_1  		0x0008//V_SW_2_USB
#define V_Side_2			0x0010//V_SW_1_USB
#else
#define V_L_Side_1  		0x0008//V_SW_2_USB
#define V_L_Side_2		0x0010//V_SW_1_USB

#define V_R_Side_1  		0x0008//V_SW_2_USB
#define V_R_Side_2		0x0010//V_SW_1_USB
#endif

//#define V_Side_3 	0x0020//V_SW_3_USB


#ifdef P502
#define V_SW_L_Bit		BIT24
#define V_SW_R_Bit		BIT7
#define V_SW_M_Bit		BIT19
#define V_SW_DPI_1_Bit	BIT12
#define V_SW_DPI_2_Bit	BIT16
#define V_Side_1_Bit 		BIT13
#define V_Side_2_Bit 		BIT14
#define PORT0_ALL_BUTTON (BIT7 | BIT12 | BIT19)
#define PORT1_ALL_BUTTON (BIT13 | BIT14 | BIT16 | BIT24)
#else
#define V_SW_L_Bit			BIT19
#define V_SW_R_Bit			BIT16
#define V_SW_M_Bit			BIT14
#define V_SW_DPI_1_Bit		BIT11
#define V_L_Side_1_Bit 		BIT13
#define V_L_Side_2_Bit 		BIT14
#define V_R_Side_1_Bit 		BIT12
#define V_R_Side_2_Bit 		BIT11
#define PORT0_ALL_BUTTON 	( BIT11 | BIT12 | BIT13 | BIT14 | BIT19)
#define PORT1_ALL_BUTTON 	( BIT13 | BIT14 |BIT16)
#endif
//#define V_Side_3_Bit 	BIT16





///-------------------------------------------------------------------------------- Sensor Define
/*
#define V_Sensor_Motion_Port		0
#define V_Sensor_Motion_Pin		19
#define V_SensorINIT_Delay		300		// Base 1mSec
#define V_DPI_1200		24		//12(0x0C)
#define V_DPI_2400		48		//24(0x18)
#define V_DPI_4800		96		//48(0x30)
#define V_DPI_8200		164		//82(0x52)
#define V_DPI_1600		32
#define V_DPI_2000		40
#define V_DPI_3200		64
#define V_Default_DPI_1	V_DPI_1200
#define V_Default_DPI_2	V_DPI_1600
#define V_Default_DPI_3	V_DPI_2000
#define V_Default_DPI_4	V_DPI_3200
#define V_DPI_LED_Timer		3000		// for DPI Change LED On 3Sec
//#define V_DPI_LED_Timer		1500		// for DPI Change LED On 3Sec
#define V_DPI_1200_LED		0x04
#define V_DPI_2400_LED		0x06
#define V_DPI_4800_LED		0x03
#define V_DPI_8200_LED		0x07
*/
///-------------------------------------------------------------------------------- Scroll Define
//#define V_Z1_Port	0
//#define V_Z1_Pin		17

//#define V_Z2_Port	0
//#define V_Z2_Pin		18

#if 0 
///-------------------------------------------------------------------------------- PWM Define
#define V_PWM_ShowBase			10
//#define V_PWM_ShowTimer		(3*V_PWM_ShowBase)-1	//10(base)*3 = 20mSec  Base On V_PWM_Timer_Base
#define V_PWM_ShowTimer		(1*V_PWM_ShowBase)-1	//10(base)*3 = 20mSec  Base On V_PWM_Timer_Base

#define V_PWM_Level				100


//#define V_PWM_BasePeriod		1000
#define V_PWM_BasePeriod		2000
#define V_PWM_Timer_Base		480*V_PWM_BasePeriod	// 10mSec


// High Plus Active
//#define V_PWM_Timer_Off			V_PWM_Timer_Base

#define V_PWM_Timer_100		1

#define V_PWM_Delta				V_PWM_Timer_Off/V_PWM_Level

#define V_PWM_Timer_75			V_PWM_Timer_Base/4
#define V_PWM_Timer_50			V_PWM_Timer_Base/2
#define V_PWM_Timer_25			(V_PWM_Timer_Base/4)*3

///-------------------------------------------------------

#define VL_PWM_ShowBase		(16-1)
#define VL_PWM_ShowBase_FOR_LEVEL_9  7
#define VL_PWM_PeriodMax		768000				// Max Off
//#define VL_PWM_PeriodMax		720000	// 15mSec

#define VL_PWM_Level			256

#define V_PWM_Timer_Off			VL_PWM_PeriodMax

#define V_PWM_Timer_80			(VL_PWM_PeriodMax*(1-0.8))

///-------------------------------------------------------------------------------- EX-Flash Define
#define V_Page_Size	256
#define V_Line_Num	16

#define V_Page_0	0x00000000
#define V_Page_1	0x00000100
#define V_Page_2	0x00000200

#define V_Sector_0	0x00000000

#define V_Sector_1	0x00001000	
#define V_Sector_2	0x00002000	
#define V_Sector_3	0x00003000	
#define V_Sector_4	0x00004000	
#define V_Sector_5	0x00005000	

#define V_Flash_3000		V_Sector_1
#define V_Flash_4000		V_Sector_2
#define V_Flash_5000		V_Sector_3
#define V_Flash_6000		V_Sector_4
#define V_Flash_7000		V_Sector_5


#define AppReqUpgradeID 0xAC	
#define AppNonUpgradeID 0xCA

#define CV_FWUpData_Index			0x02
#define CV_FW_EndAddrHigh_Index	0x03
#define CV_FW_EndAddrLow_Index	0x04

#define CV_DPI_Index				0x06
#define CV_Sensor_ReportRate		0x08
#define CV_YLogo_SleepIndex			0x0A

#define CV_DPI_4Level_Index			0x0C
#define CV_DPI_4_Level_1_Index		0x0C
#define CV_DPI_4_Level_2_Index		0x0D
#define CV_DPI_4_Level_3_Index		0x0E
#define CV_DPI_4_Level_4_Index		0x0F

#define CV_DPI_Num					1

#define CV_YLogo_Light_Level			0x10
#define CV_YLogo_Light_PWM			0x12

#define CV_Breathing_Enabe			0x14
#endif

#if 1

#define V_ReportRate_1000	0
#define V_ReportRate_500	2
#define V_ReportRate_250	4
#define V_ReportRate_125	8

#else
#define V_ReportRate_500	0
//#define V_ReportRate_500	2
#define V_ReportRate_250	3
#define V_ReportRate_125	7
#define V_ReportRate_100	9
#endif



#define V_EP0Buffer_Size	8
#if 0
#define V_FWUp_ID			0x07

#define V_ACK_Command		0xAC
#define V_FWUp_ACKFail		0xFF

#define V_FWUp_Start		0x01
#define V_FWUp_DataSend	0x02

#define V_FWVer_Read		0x03

#define V_DPI_Read			0x11

#define V_ReportRate_Read	0x12

#define V_YLogoLight_Read	0x13
#define V_YLogoPWM_Read	0x14
#define V_YLogoSleep_Read	0x15

#define V_LevelDPI_Read		0x16

#define V_FlashPage0_Erase 			0x5A
#define V_FlashPage0_Erase_Data		0xA5

#define V_DPI_Change		0xB1
#define V_SensorReportRate	0xB2

#define V_LED_Light			0xC1
#define V_PWM_Change		0xC2

#define V_Y_LED_Sleep		0xC3
#define V_LevelDPI_Change	0xC4

#define V_Breathing_Enabe	0xC5

#define V_Set_All		0xF0


#define V_PWM_Change_MIN	10
#define V_PWM_Change_MAX	250

#define V_LevelIndex			5
#define V_LevelIndex_CNT	5


#define V_YLogo_Sleep_Sec	1000

#define V_YLogo_Sleep_Check		0
#define V_YLogo_Sleep_Disable	1
#define V_YLogo_Sleep_Enable	2	

#define V_PWM_Level_Info		20


#define V_PWM_Timer_DIV		2

#endif

/*
///	base timer 16 mSec
#define V_PWM_Timer_CNT_0		1250*0.77	//20Sec	20 000 = 1250
#define V_PWM_Timer_CNT_1		1125*0.77	//18Sec	18 000 = 1125
#define V_PWM_Timer_CNT_2		1000*0.82	//16Sec	16 000 = 1000
#define V_PWM_Timer_CNT_3		875*0.84	//14Sec	14 000 = 875
#define V_PWM_Timer_CNT_4		750*0.84	//12Sec	12 000 = 750
#define V_PWM_Timer_CNT_5		625*0.82		//10Sec	10 000 = 625
#define V_PWM_Timer_CNT_6		500*0.77	//8Sec	8 000 = 500
#define V_PWM_Timer_CNT_7		375*0.75		//6Sec	6 000 = 375
#define V_PWM_Timer_CNT_8		187*0.77		// 3Sec	3 000 = 187
#define V_PWM_Timer_CNT_9		5*0.77		// 0.5Sec	0 500 = 31


#define V_PWM_Level_0			400*0.77
#define V_PWM_Level_1			350*0.77
#define V_PWM_Level_2			300*0.82
#define V_PWM_Level_3			250*0.84
#define V_PWM_Level_4			250*0.84
#define V_PWM_Level_5			200*0.82
#define V_PWM_Level_6			150*0.77
#define V_PWM_Level_7			90*0.75
#define V_PWM_Level_8			65*0.77
#define V_PWM_Level_9			20*0.77
*/

#define AppID_Adr 0x00003100			// For FW Ver Read Use



//--------------------------------------------------------------------------------
#define CMD_WriteData 	0x51
#define CMD_RWMarco 	0x53
#define CMD_GetInfo 		0x12
#define CMD_ProfileFlash 	0x50
//#define CMD_WRITEABLE_DATA 	0x51
//#define CMD_GETINFO			0x12
//Writable Data
#define PROFILE_CHOOSE	0x00
#define MARCO_CHOOSE	0x18
#define KEY_CHANGE		0x21
#define EFFECT_CHOOSE	0x28
#define SET_FUNCTION	0x31


//--------------------------------------------------------------------------------

extern bool BUTTON_USER_DEFINE_FLAG;
extern bool DRIVER_ENABLE_FLAG;
extern U8 USB_BUFFER[];
extern U8 USB_IN_BUFFER[];
///==============================================================
void S_Scroll_Polling(void);
void S_KeyScan_BounceCheck(void);
void S_GPIO_Check_Key(void);
void S_GPIO_INIT(void);
void S_PWM_INIT(void);
void S_PWM_Fedding(void);
void S_DATA_INIT(void);
void S_Sensor_MotionCheck(void);
//void S_DPI_Change(void);	//2016_0506 Hsaing

void S_EX_Flash_Erase_Sector(U32 V_Addr);
void S_EX_Flash_Read_Page(U32 V_Addr);
void S_EX_Flash_Write_Page(U32 V_Addr);
void S_SPI0_INIT(void);

//void DS_LED_Toggle(void);
void S_Idle_Delay_uSec(uint16_t V_Value);
void DS_Flash_Debug_Test(U8 V_Command);	// For Debug Use

void DS_OutReport_Show(void);
void DS_EP0Buffer_Show(void);
void DS_TempFlashBuffer_Show(void);
void DS_Flash_Show(void);

void DS_FWDownLoad_FlashShow(void);

void S_USBReport_GetFromHost(void);
void S_USBReport_SendtoHost(void);


void S_DPIChange_LED(void);
void S_DPI_LED_Off(void);

void S_PWM_Disable(void);
//void S_PWM_Enable(void);
void S_PWM_Enable_WakeUp(void);

U8 S_Y_SleepCheck(void);
void S_Y_Sleep_ValueSet(void);

void S_DPI_4Level_INIT(void);
void S_DPI_4Level_Print(void);

void S_Y_PWM_Running_Check(void);
void S_Y_Sleep_ReCheck(void);

void S_IO_Suspend_Setting(void);
void S_IO_ReSetting(void);

//U16 S_PWMTimer_Get(U8 R_Index);
void S_PWMTimer_Get(U8 R_Index);
void S_Y_Logo_Setting(U8 R_Light,U8 R_PWM);


void S_USB_SetReport_App_Command_Handler(U8 app_cmd,U8* R_FlashWirte);

extern void Wheel_polling(void);

extern void Key_Change_Process(U16 src, U16 dst);

extern void Load_Mouse_Param_Default(void);
extern void Load_Default_Mouse_status(void);
extern void Set_ALL_Profile_Param_to_Default(void);
extern void Load_Profile_to_CurParam(void);
extern void Set_CurParam_to_CurProfile(void);
extern void Choose_Profile(U8 profile_no);



extern void Clean_Button_Counts(void);
extern void Test_Func(void);
extern void Get_Device_Info(void);
extern void Get_Key_Status(U16 key);
extern void Get_Key_Counts(U16 key);
extern void Get_Effect_Info(U8 region);
extern void Profile_Init(void);
extern void Set_CurParam_from_CurProfile(void);

extern U16 Mouse_Status_length;
extern U16 Mouse_Button_Func_length;
extern U16 Mouse_Led_Effect_Param_length;
extern U8 USB_INTERRUPT_IN_SIZE;

extern U16 Profile_Length;
extern U16 Profile_1_Loc;
extern U16 Profile_2_Loc;
extern U16 Profile_3_Loc;

//------------------------------------------------------------------------------------------

#define EEPROM_POLLING_RATE_LOC  20

//------------------------------------------------------------------------------------------
/*
const U8 KEY_TABLE[]=
{
	0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
 	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
 	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
 	0x30, 0x31, 0x00, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
 	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
 	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
 	0x60, 0x61, 0x62, 0x63, 0x00, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
 	0x70, 0x71, 0x72, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const U8 Special_KEY_TABLE[]=
{
	0x08, 0x10, 0xD1, 0xD2, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x00
};
*/
//extern struct Mouse_Status_t Cur_Mouse_Status;
//extern struct Button_Func_t Cur_Button_Func;
//extern struct Led_Effect_t Cur_Led_Effect;
