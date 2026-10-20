


#include <stdbool.h>
#include <string.h>
#include "LPC11Uxx.h"/* LPC11xx Peripheral Registers */
#include "Uart.h"

#include "Main.h"
//#include "Mouse.h"
#include "gpio.h"
#include "ssp.h"
#include "pmu.h"		// for PMW INIT
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "wdt.h"		// for Reset Use

#include "Pixart_Sensor.h"
#include "Wheel.h"
#include "eepromAPI.h"

//extern  LPC11Uxx_DATABASE_MOUSE_GENSETTING g_dMouse;

U8	F_KeyScan_Change =0;
U16 R_KeyStatus =0;
U16 R_KeyStatus_Old =0;
U16 R_KeyScan_Bounce =0;


void S_Idle_Delay_uSec(uint16_t V_Value){	// Base 100uSec


	uint16_t	R_Idle_1=0;
	uint16_t 	R_Idle_2 =0; 
	
	for(R_Idle_2=0;R_Idle_2<V_Value;R_Idle_2++){
		
		for(R_Idle_1=0;R_Idle_1<790;R_Idle_1++);	// Base 100uSec
	}
		
}
#if 0
//--------------------------------------------------------------------------------
#define FW_VERSION 0x0100
#define Profile_Numbers 3
U8 CUR_PROFILE=0;
U8 USB_INTERRUPT_IN_SIZE=0;
U8 	USB_BUFFER[64]={0};
U8 	USB_IN_BUFFER[64]={0};
//--------------------------------------------------------------------------------
const struct Mouse_Status_t DEFAULT_MOUSE_STATUS=
{
	.ver=FW_VERSION,
	.current_profile=0,
	.profile_size=3,
	.key_status=
	{
		.left_button=FALSE,
		.righe_button=FALSE,
		.middle_button=FALSE,
		.dpi1_button=FALSE,
		.dpi2_button=FALSE,
		.button_4=FALSE,
		.button_5=FALSE,
	},
	.dpi1_value=0,
	.dpi2_value=0,
	.acceleration_value=0,
	.deceleration_value=0,
	.polling_rate=0,
	.response=0,
	.counts=
	{
		.left=0x0000,
		.right=0x0000,
		.middle=0x0000,
		.dpi1=0x0000,
		.dpi2=0x0000,
		.key4=0x0000,
		.key5=0x0000,
	},
};
//--------------------------------------------------------------------------------
const struct Button_Func_t DEFAULT_MOUSE_BUTTON_FUNC=
{
	.left=0x01,
	.right=0x02,
	.middle=0x04,
	.dpi1=0x20,
	.dpi2=0x40,
	.key4=0x08,
	.key5=0x10,
};
//--------------------------------------------------------------------------------
const struct Led_Effect_t DEFAULT_MOUSE_LED_EFFECT=
{
	.Is_All_Effect=0,
	.Times=
	{
		.breath=60,
		.colorcycle=60,
		.colorwave=60,
	},
	.Logo=
	{
		.Effect=0,
		.Brigntness=0,
		.color={0,0,0},
	},
	.Wheel=
	{
		.Effect=0,
		.Brigntness=0,
		.color={0,0,0},
	},
	.LightPipe=
	{
		.Effect=0,
		.Brigntness=0,
		.color={0,0,0},
	},
	.All=
	{
		.Effect=0,
		.Brigntness=0,
		.color={0,0,0},
	},
};
//--------------------------------------------------------------------------------
const struct Mousepad_t DEFAULT_Mousepad=
{
	.ROG_Whetstone=
	{
		.min_squal_value=0,
		.pixel_threshold_value=0,
	},
	.ROG_Sheath=
	{
		.min_squal_value=0,
		.pixel_threshold_value=0,
	},
	.Cloth=
	{
		.min_squal_value=0,
		.pixel_threshold_value=0,
	},
	.Plastic=
	{
		.min_squal_value=0,
		.pixel_threshold_value=0,
	},
	.Glass=
	{
		.min_squal_value=0,
		.pixel_threshold_value=0,
	},
	.Metal=
	{
		.min_squal_value=0,
		.pixel_threshold_value=0,
	},
};
//--------------------------------------------------------------------------------
const struct Sensor_Calibration_Param_t DEAULT_Sensor_Calibration_Param=
{
	.min_squal_value=0x10,
	.pixel_threshold_value=0x0A,
};
//--------------------------------------------------------------------------------
 struct Mouse_Status_t Cur_Mouse_Status;
 struct Button_Func_t Cur_Button_Func;
 struct Led_Effect_t Cur_Led_Effect;
 struct Sensor_Calibration_Param_t Cur_Sensor_Calibration_Param;

U16 Mouse_Status_length = sizeof(struct Mouse_Status_t);						//0x24
U16 Mouse_Button_Func_length = sizeof(struct Button_Func_t);				//0x0E
U16 Mouse_Led_Effect_Param_length = sizeof(struct Led_Effect_t);			//0x15
U16 Mouse_Sensor_Calibration_Param_length=2;

U16 Profile_Length=0;
U16 Profile_1_Loc=0;
U16 Profile_2_Loc=0;
U16 Profile_3_Loc=0;

/*
#define Profile_Length 71+2
#define Profile_1_Loc 1+Profile_Length
#define Profile_2_Loc Profile_1_Loc+Profile_Length
#define Profile_3_Loc Profile_2_Loc+Profile_Length
*/
void Profile_Init(void)
{
	Profile_Length=Mouse_Status_length+ Mouse_Button_Func_length+ Mouse_Led_Effect_Param_length+ Mouse_Sensor_Calibration_Param_length;
	Profile_1_Loc=1+Profile_Length;
	Profile_2_Loc=Profile_1_Loc+Profile_Length;
	Profile_3_Loc=Profile_2_Loc+Profile_Length;
}
//--------------------------------------------------------------------------------
void Load_Default_Mouse_Seansor_Calibration_Param(void)
{
	Cur_Sensor_Calibration_Param=DEAULT_Sensor_Calibration_Param;
}
//--------------------------------------------------------------------------------
void Load_Default_Mouse_status(void)
{
	//memcpy(DEFAULT_MOUSE_STATUS, Cur_Mouse_Status, Mouse_Status_length);
	Cur_Mouse_Status=DEFAULT_MOUSE_STATUS;
}
//--------------------------------------------------------------------------------
void Load_Default_Mouse_Button_Func(void)
{
	//memcpy(DEFAULT_MOUSE_BUTTON_FUNC, Cur_Button_Func, Mouse_Button_Func_length);
	Cur_Button_Func=DEFAULT_MOUSE_BUTTON_FUNC;
}
//--------------------------------------------------------------------------------
void Load_Default_Mouse_Led(void)
{	
	//memcpy(DEFAULT_MOUSE_LED_EFFECT, Cur_Led_Effect, Mouse_Led_Effect_Param_length);
	Cur_Led_Effect=DEFAULT_MOUSE_LED_EFFECT;
}
#if 1
//--------------------------------------------------------------------------------
void Clean_Button_Counts(void)
{
	Cur_Mouse_Status.counts.left=0;
	Cur_Mouse_Status.counts.right=0;
	Cur_Mouse_Status.counts.middle=0;
	Cur_Mouse_Status.counts.dpi1=0;
	Cur_Mouse_Status.counts.dpi2=0;
	Cur_Mouse_Status.counts.key4=0;
	Cur_Mouse_Status.counts.key5=0;
	Cur_Mouse_Status.counts=DEFAULT_MOUSE_STATUS.counts;
}
//--------------------------------------------------------------------------------
void Set_CurProfile_From_Default_Param(void)
{
	Load_Default_Mouse_status();
	Load_Default_Mouse_Button_Func();
	Load_Default_Mouse_Led();
	Load_Default_Mouse_Seansor_Calibration_Param();
	if(CUR_PROFILE==1)
		Cur_Led_Effect.LightPipe.color[0]=255;	//R
	else if(CUR_PROFILE==2)
	{
		Cur_Led_Effect.LightPipe.color[0]=255;	//R
		Cur_Led_Effect.LightPipe.color[2]=255;	//B
	}
	else if(CUR_PROFILE==3)
		Cur_Led_Effect.LightPipe.color[2]=255;	//B
}
//--------------------------------------------------------------------------------
// 將預設值寫入profile內(不確定為什麼要先寫入buffer才能再從buffer槫槫槫寫到EEPROM)
//--------------------------------------------------------------------------------
void Set_Cur_Profile_Param_From_Default(U8 profile_no)
{
	U32 loc;
	U8 BUFFER[36];
	U8 *ptr;

	Load_Default_Mouse_status();
	Load_Default_Mouse_Button_Func();
	Load_Default_Mouse_Led();
	Load_Default_Mouse_Seansor_Calibration_Param();
	if(CUR_PROFILE==1)
	{
		loc=Profile_1_Loc;
		Cur_Led_Effect.LightPipe.color[0]=255;	//R
		Cur_Led_Effect.LightPipe.color[1]=0;		//G
		Cur_Led_Effect.LightPipe.color[2]=0;		//B
	}
	else if(CUR_PROFILE==2)
	{	
		loc=Profile_2_Loc;
		Cur_Led_Effect.LightPipe.color[0]=255;	//R
		Cur_Led_Effect.LightPipe.color[1]=0;		//G
		Cur_Led_Effect.LightPipe.color[2]=255;	//B
	}
	else if(CUR_PROFILE==3)
	{
		loc=Profile_3_Loc;
		Cur_Led_Effect.LightPipe.color[0]=0;		//R
		Cur_Led_Effect.LightPipe.color[1]=0;		//G
		Cur_Led_Effect.LightPipe.color[2]=255;	//B
	}

	ptr=(U8*)&Cur_Mouse_Status;
	memcpy(BUFFER, ptr, Mouse_Status_length);
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER, (U32)Mouse_Status_length );
		
	loc+=(U32)Mouse_Status_length;
	ptr=(U8*)&Cur_Button_Func;
	memcpy(BUFFER, ptr, Mouse_Button_Func_length);
	writeEEPROM( (uint8_t*)  loc, (uint8_t*) BUFFER,  (U32)Mouse_Button_Func_length );
		
	loc+=Mouse_Button_Func_length;
	ptr=(U8*)&Cur_Led_Effect;
	memcpy(BUFFER, ptr, Mouse_Led_Effect_Param_length);
	writeEEPROM( (uint8_t*)  loc, (uint8_t*) BUFFER,  (U32)Mouse_Led_Effect_Param_length );
	
	loc+=Mouse_Sensor_Calibration_Param_length;
	ptr=(U8*)&Cur_Sensor_Calibration_Param;
	memcpy(BUFFER, ptr, Mouse_Sensor_Calibration_Param_length);
	writeEEPROM( (uint8_t*)  loc, (uint8_t*) BUFFER,  (U32)Mouse_Sensor_Calibration_Param_length );
	
	
}
//--------------------------------------------------------------------------------
// 將指定的Profile資料寫入Ram
//--------------------------------------------------------------------------------
void Load_Profile_to_CurParam(void)
{
	U32 loc;
	
	if(CUR_PROFILE==1)
		loc=Profile_1_Loc;
	else if(CUR_PROFILE==2)
		loc=Profile_2_Loc;
	else if(CUR_PROFILE==3)
		loc=Profile_3_Loc;
	readEEPROM( (uint8_t*) loc, (uint8_t*) &Cur_Mouse_Status,  (U32)Mouse_Status_length );
	loc+=Mouse_Status_length;
	readEEPROM( (uint8_t*)  loc, (uint8_t*) &Cur_Button_Func,  (U32)Mouse_Button_Func_length );
	loc+=Mouse_Button_Func_length;
	readEEPROM( (uint8_t*)  loc, (uint8_t*) &Cur_Led_Effect,  (U32)Mouse_Led_Effect_Param_length );
	loc+=Mouse_Sensor_Calibration_Param_length;
	readEEPROM( (uint8_t*)  loc, (uint8_t*) &Cur_Sensor_Calibration_Param,  (U32)Mouse_Sensor_Calibration_Param_length );
}
//--------------------------------------------------------------------------------
// 將預設值寫入profile內(不確定為什麼要先寫入buffer才能再從buffer槫槫槫寫到EEPROM)
//--------------------------------------------------------------------------------
void Set_ALL_Profile_Param_to_Default(void)
{
	U32 loc;
	U8 BUFFER[36];
	U8 *ptr;
	
	Load_Default_Mouse_status();
	Load_Default_Mouse_Button_Func();
	Load_Default_Mouse_Led();

	if(CUR_PROFILE==1)
	{
		Cur_Led_Effect.LightPipe.color[0]=255;	//R
		Cur_Led_Effect.LightPipe.color[1]=0;		//G
		Cur_Led_Effect.LightPipe.color[2]=0;		//B
	}
	else if(CUR_PROFILE==2)
	{	
		Cur_Led_Effect.LightPipe.color[0]=255;	//R
		Cur_Led_Effect.LightPipe.color[1]=0;		//G
		Cur_Led_Effect.LightPipe.color[2]=255;	//B
	}
	else if(CUR_PROFILE==3)
	{
		Cur_Led_Effect.LightPipe.color[0]=0;		//R
		Cur_Led_Effect.LightPipe.color[1]=0;		//G
		Cur_Led_Effect.LightPipe.color[2]=255;	//B
	}
	
	ptr=(U8*)&Cur_Mouse_Status;
	memcpy(BUFFER, ptr, Mouse_Status_length);
	loc=Profile_1_Loc;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Status_length );
	loc=Profile_2_Loc;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Status_length );
	loc=Profile_3_Loc;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Status_length );

	ptr=(U8*)&Cur_Button_Func;
	memcpy(BUFFER, ptr, Mouse_Button_Func_length);
	loc=Profile_1_Loc+Mouse_Button_Func_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Button_Func_length );
	loc=Profile_2_Loc+Mouse_Button_Func_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Button_Func_length );
	loc=Profile_3_Loc+Mouse_Button_Func_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Button_Func_length );

	Cur_Led_Effect.LightPipe.color[0]=255;	//R
	Cur_Led_Effect.LightPipe.color[1]=0;		//G
	Cur_Led_Effect.LightPipe.color[2]=0;		//B
	ptr=(U8*)&Cur_Led_Effect;
	memcpy(BUFFER, ptr, Mouse_Led_Effect_Param_length);
	loc=Profile_1_Loc+Mouse_Button_Func_length+Mouse_Led_Effect_Param_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Led_Effect_Param_length );
	Cur_Led_Effect.LightPipe.color[0]=255;	//R
	Cur_Led_Effect.LightPipe.color[1]=0;		//G
	Cur_Led_Effect.LightPipe.color[2]=255;	//B
	loc=Profile_2_Loc+Mouse_Button_Func_length+Mouse_Led_Effect_Param_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Led_Effect_Param_length );
	Cur_Led_Effect.LightPipe.color[0]=0;		//R
	Cur_Led_Effect.LightPipe.color[1]=0;		//G
	Cur_Led_Effect.LightPipe.color[2]=255;	//B
	loc=Profile_3_Loc+Mouse_Button_Func_length+Mouse_Led_Effect_Param_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Led_Effect_Param_length );

	ptr=(U8*)&Cur_Sensor_Calibration_Param;
	memcpy(BUFFER, ptr, Mouse_Sensor_Calibration_Param_length);
	loc=Profile_1_Loc+Mouse_Button_Func_length+Mouse_Led_Effect_Param_length+Mouse_Sensor_Calibration_Param_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Sensor_Calibration_Param_length );
	loc=Profile_2_Loc+Mouse_Button_Func_length+Mouse_Led_Effect_Param_length+Mouse_Sensor_Calibration_Param_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Sensor_Calibration_Param_length );
	loc=Profile_3_Loc+Mouse_Button_Func_length+Mouse_Led_Effect_Param_length+Mouse_Sensor_Calibration_Param_length;
	writeEEPROM( (uint8_t*) loc, (uint8_t*) BUFFER,  (U32)Mouse_Sensor_Calibration_Param_length );
	
}
//--------------------------------------------------------------------------------
//將目前設定好的資料, 寫入對應的profile中
//--------------------------------------------------------------------------------
void Set_CurParam_from_CurProfile(void)
{
	U32 loc;
	U8 *ptr;
	U8 BUFFER[36];
	if(CUR_PROFILE==1)
		loc=Profile_1_Loc;
	else if(CUR_PROFILE==2)
		loc=Profile_2_Loc;
	else if(CUR_PROFILE==3)
		loc=Profile_3_Loc;
	
	ptr=(U8*)&DEFAULT_MOUSE_STATUS;
	memcpy(BUFFER, ptr, Mouse_Status_length);
	writeEEPROM( (uint8_t*) loc, (uint8_t*) &Cur_Mouse_Status,  (U32)Mouse_Status_length );

	ptr=(U8*)&Cur_Button_Func;
	memcpy(BUFFER, ptr, Mouse_Button_Func_length);
	loc+=Mouse_Status_length;
	writeEEPROM( (uint8_t*)  loc, (uint8_t*) &Cur_Button_Func,  (U32)Mouse_Button_Func_length );

	ptr=(U8*)&Cur_Led_Effect;
	memcpy(BUFFER, ptr, Mouse_Sensor_Calibration_Param_length);
	loc+=Mouse_Button_Func_length;
	writeEEPROM( (uint8_t*)  loc, (uint8_t*) &Cur_Led_Effect,  (U32)Mouse_Led_Effect_Param_length );

	ptr=(U8*)&Cur_Sensor_Calibration_Param;
	memcpy(BUFFER, ptr, Mouse_Led_Effect_Param_length);
	loc+=Mouse_Led_Effect_Param_length;
	writeEEPROM( (uint8_t*)  loc, (uint8_t*) &Cur_Led_Effect,  (U32)Mouse_Sensor_Calibration_Param_length );
}
//--------------------------------------------------------------------------------
//選取Profile
//--------------------------------------------------------------------------------
void Choose_Profile(U8 profile_no)
{
	CUR_PROFILE=profile_no;
	Load_Profile_to_CurParam();
}
//--------------------------------------------------------------------------------
/*
void KeyChange_Func(U8 *ptr)
{
	U16 Source_key=*((U16*)(ptr+4));
	U16 Target_key=*((U16*)(ptr+6));
	U8 key_vaule=0;
	U8 in_type_flag=0;
	if(Target_key<0x01E0)
	{
		Target_key-=0x04;
		key_vaule=KEY_TABLE[(U8)Target_key];
		if(key_vaule>0xE7)
			in_type_flag=4;
		else
			in_type_flag=2;
	}
	else
	{
		Target_key-=0x1E4;
		key_vaule=Special_KEY_TABLE[(U8)Target_key];
		in_type_flag=1;
	}
	
	switch(Source_key)
	{
		case 0x01F0:
			Cur_Button_Func.left.InType=in_type_flag;
			Cur_Button_Func.left.Value=Target_key;		
		break;
		case 0x01F1:
			Cur_Button_Func.right.InType=in_type_flag;
			Cur_Button_Func.right.Value=Target_key;		
		break;
		case 0x01F2:
			Cur_Button_Func.middle.InType=in_type_flag;
			Cur_Button_Func.middle.Value=Target_key;		
		break;
		case 0x01E4:
			Cur_Button_Func.key4.InType=in_type_flag;
			Cur_Button_Func.key4.Value=Target_key;		
		break;
		case 0x01E5:
			Cur_Button_Func.key5.InType=in_type_flag;
			Cur_Button_Func.key5.Value=Target_key;		
		break;
		case 0x01E6:
			Cur_Button_Func.dpi1.InType=in_type_flag;
			Cur_Button_Func.dpi1.Value=Target_key;		
		break;
		case 0x01E7:
			Cur_Button_Func.dpi2.InType=in_type_flag;
			Cur_Button_Func.dpi2.Value=Target_key;		
		break;
	}
	//Cur_Mouse_Status
}
*/
//--------------------------------------------------------------------------------
void Test_Func(void)
{
  	U8 BUFFER[36];
	U8 *ptr=(U8*)&DEFAULT_MOUSE_STATUS;
	memcpy(BUFFER, ptr, Mouse_Status_length);
		
	U32 loc=Profile_1_Loc;
	writeEEPROM( (uint8_t*) loc, (U8*) BUFFER, (U32)Mouse_Status_length );
	
	loc+=(U32)Mouse_Status_length;
	ptr=(U8*)&DEFAULT_MOUSE_BUTTON_FUNC;
	memcpy(BUFFER, ptr, Mouse_Button_Func_length);
	writeEEPROM( (uint8_t*)  loc, (uint8_t*)BUFFER,  (U32)Mouse_Button_Func_length );
	
	loc+=Mouse_Button_Func_length;
	ptr=(U8*)&DEFAULT_MOUSE_LED_EFFECT;
	memcpy(BUFFER, ptr, Mouse_Led_Effect_Param_length);
	writeEEPROM( (uint8_t*)  loc, (uint8_t*) BUFFER,  (U32)Mouse_Led_Effect_Param_length );
		
	loc=Profile_1_Loc;
	readEEPROM( (uint8_t*) loc, (uint8_t*) &Cur_Mouse_Status,  (U32)Mouse_Status_length );
	loc+=Mouse_Status_length;
	readEEPROM( (uint8_t*)  loc, (uint8_t*) &Cur_Button_Func,  (U32)Mouse_Button_Func_length );
	loc+=Mouse_Button_Func_length;
	readEEPROM( (uint8_t*)  loc, (uint8_t*) &Cur_Led_Effect,  (U32)Mouse_Led_Effect_Param_length );
}
//--------------------------------------------------------------------------------
void PROFILE_CHECK_AND_LOAD(void)
{
	U8 buffer[2];
	readEEPROM( (uint8_t*) EERPOM_CHECK_ADDR, (uint8_t*) buffer, 2);
	if(buffer[0] != 0x78 || buffer[1]!=0x87)
	{
		buffer[0]=0x78;
		buffer[1]=0x87;
		writeEEPROM( (uint8_t*) EERPOM_CHECK_ADDR, (uint8_t*) buffer, 2 );
		Set_ALL_Profile_Param_to_Default();
		Choose_Profile(1);
	}
	else
	{
		readEEPROM( (uint8_t*)0, (uint8_t*) buffer, 1);
		Choose_Profile(buffer[0]);
	}
}
#endif
//--------------------------------------------------------------------------------
#if 0
typedef struct _Hex_DATA_
{
	U8	Enable;
	U8	PageLen;

	U8	Len;
	U16	Addr;
	U8	Data[16];

	U16 WriteAddr;
	U16 FW_End_Addr;
	U16 FW_CNT_Addr;	
	
}Hex_DATA;
#endif

#if 0
typedef struct _PWM_Fade_
{
	U8	F_InOut;
	U8	R_ShowTimer;

	//U8	R_LevelIndex;
	//U8	R_LevelIndex_CNT;
	
}PWM_Fade;


typedef struct _Fade_
{
	U8	F_InOut;
	U8	R_ShowTimer;

	U32	R_Max_Light;		// for Light Index
	
	U16	R_Timer_CNT;		// for PWM Index
	U16 R_Timer_CNT_Limit;
	
	U32	R_Light_Delta;	

	U16 R_PWM_Level;

	U8	R_Breathing_Enable;
	
}Fade;

#endif

//--------------------------------------------------------------------------------
U16	R_SensorINIT_Delay=0;
U8 	F_SensorINIT=0;

//U8	R_SensorBurst[6];

U8	R_Scroll_Value =0;
U8	R_Scroll_Data =0;
U8	R_Scroll_Index =0;

U8	F_KeyScan_Change =0;
U16 R_KeyStatus =0;
U16 R_KeyStatus_Old =0;
U16 R_KeyScan_Bounce =0;

U8	R_DPI_Index =0;
U8	R_DPI_ChangeDelay=0;
U8	R_DPI_4Level[4];

U16	R_FW_Ver =0;
U16	R_BL_Ver =0;

U8	R_DPI_Fix =0xff;
U16	R_DPI_LED_Timer =0;


U8	R_LED_Light = 0x00;		// 0 is MIX : 0xFF is MAX for Flash Index
U8	R_PWM_Level =0;		// for Flash Index 




U8	F_Send_GetReport_Flag =0;


U8	R_Sensor_RepoetRate=0;
U8	R_Sensor_RepoetRate_CNT=0;

//U16	R_YLogo_Sleep_Sec_CNT =0;
//U16	R_YLogo_Sleep_Min_CNT=0;
//U16 R_YLogo_Sleep_CMP=0;
//U8	R_YLogo_Sleep_Index =0;

U8	F_PWM_Off =0;

U8	DR_Flag =0;

#ifdef V_KeyScan_Pin_Debug
	U8	F_Togle =0;
#endif
#ifdef V_SleepMode_Pin_Debug
	U8	F_Togle =0;
#endif


bool BUTTON_USER_DEFINE_FLAG = false;
bool DRIVER_ENABLE_FLAG = false;



///--------------------------------------------------------------


///--------------------------------------------------------------------------------

///--------------------------------------------------------------------------------





///--------------------------------------------------------------------------------
#if 0

#endif
///--------------------------------------------------------------------------------
#if 0

#endif

///--------------------------------------------------------------------------------
#ifdef V_Uart_Print
void S_Addr_Show(U32 V_Addr){

	xprintf("%x",((V_Addr>>20)&0xf));	
	xprintf("%x",((V_Addr>>16)&0xf));	
	xprintf("%x",((V_Addr>>12)&0xf));		
	xprintf("%x",((V_Addr>>8)&0xf));	
	xprintf("%x",((V_Addr>>4)&0xf));		
	xprintf("%x",((V_Addr)&0xf));	
}
#endif
#if 0

#endif
///--------------------------------------------------------------------------------

///--------------------------------------------------------------------------------
#if 1
void S_Sensor_MotionCheck(void)		// Motion Loop for 15uSec /USB MAX 2mSec
{

	//U8	R_MotionValue=0;	
	//U8	R_X_L_Value=0,R_X_H_Value=0,R_Y_L_Value=0,R_Y_H_Value=0;	
	
	//U8	R_TempValue=0;
	U16	X_Value=0,Y_Value=0;	

	#if 1
	if(rSuspendFlag!=0x00)
	{
		return;
	}

	// when Change DPI than Idel Delay 10mSec 
	if(R_DPI_ChangeDelay!=0x00)
	{
		return;
	}
	#endif
	if(R_Sensor_RepoetRate_CNT!=0x00)
	{
		return;
	}

	//R_Sensor_RepoetRate_CNT = R_Sensor_RepoetRate;	// Release CNT

#if 1

	if(CHECK_MOTION_PIN() == LOW)		// Check Motion Pin to Low
	{
		if(nsAPI_SSP0_Sensor_Monitor(&X_Value, &Y_Value))
		{
			g_Sensor_Data.X1 =(U8)(X_Value & 0x00ff);		// X_L
			g_Sensor_Data.X2 =(U8)((X_Value>>8) & 0x00ff);	// X_H
			g_Sensor_Data.Y1 = (U8)(Y_Value & 0x00ff);		// Y_L
			g_Sensor_Data.Y2 = (U8)((Y_Value>>8) & 0x00ff);	// Y_H
			g_Sensor_Data.Changed = 1;
			//g_Repot_Process.Mouse_Report_In = 1;
			//R_TempValue= (R_DPI_Index<<4)&0xf0;						
			//g_Main_Setting.MouseInReport[1] |= R_TempValue;		// HansH Fix Report for DPI Index	
			//g_Main_Setting.MouseInReport[7] = R_DPI_Index;		// HansH Fix Report for DPI Index			
			//g_Main_Setting.MouseInReport[7] = R_DPI_4Level[R_DPI_Index];		// Bono change to x100 
			//g_Main_Setting.MouseInReport[7] = g_Main_Setting.OutReport[R_DPI_Index+2];//Bono,the dpi value is always determined by host
			
			//org size is 8 byte, 2016_0608 become 6 byte and remove id
			//g_Main_Setting.MouseInReport[7] =dpi_set_from_host[R_DPI_Index];
		
		}
		
	}

#endif
	

}
///--------------------------------------------------------------------------------
#else

#endif

///--------------------------------------------------------------------------------
#if 0

#endif

#if 0

#endif
///--------------------------------------------------------------------------------
#if 0

#endif
///--------------------------------------------------------------------------------
#if 0
void S_KeyScan_BounceCheck(void){	// Base on 1mSec Interrupt



	if(R_KeyScan_Bounce!=0x00){		// KeySCan Bounce
		R_KeyScan_Bounce--;
	}
	
///----------------------------------------------------
	if(R_DPI_ChangeDelay!=0x00){	// DPI Change Delay
		R_DPI_ChangeDelay--;
	}


///----------------------------------------------------
	if(R_SensorINIT_Delay!=0x00){	// Sensor INIT Delay
		R_SensorINIT_Delay--;
	}
	
///----------------------------------------------------
	// Sensor Report Rate CNT
#if 1

	if(R_Sensor_RepoetRate_CNT!=0x00){
		R_Sensor_RepoetRate_CNT--;
	}

#else
	if(R_Sensor_RepoetRate_CNT !=R_Sensor_RepoetRate){
		R_Sensor_RepoetRate_CNT++;
	}
	else{
		R_Sensor_RepoetRate_CNT =0;
	}
#endif
///----------------------------------------------------
	#if 0
	if(R_YLogo_Sleep_Min_CNT!=0x00){

		if(R_YLogo_Sleep_Sec_CNT!=0x00){
			R_YLogo_Sleep_Sec_CNT--;
		}
		else{
			R_YLogo_Sleep_Min_CNT--;
			R_YLogo_Sleep_Sec_CNT = V_YLogo_Sleep_Sec;			
		}		
	}	
	#endif
}
///--------------------------------------------------------------------------------
#endif





#if 0

#endif
//--------------------------------------------------------------------------------
void Button_Status_and_Conuts_Process(U16 scan_value)
{
	U8 *key_status=(U8*)&Cur_Mouse_Status.key_status;
	U16 *key_counts=(U16*)&Cur_Mouse_Status.counts;
	if(scan_value & V_SW_L)
	{
		*(key_status)=1;
		*(key_counts)+=1;
	}
	else
	{
		*(key_status)=0;
	}
	if(scan_value & V_SW_R)
	{
		*(key_status+1)=1;
		*(key_counts+1)+=1;
	}
	else
	{
		*(key_status+1)=0;
	}
	if(scan_value & V_SW_M)
	{
		*(key_status+2)=1;
		*(key_counts+2)+=1;
	}
	else
	{
		*(key_status+2)=0;
	}
	if(scan_value & V_SW_DPI_1)
	{
		*(key_status+3)=1;
		*(key_counts+3)+=1;
	}
	else
	{
		*(key_status+3)=0;
	}
	/*
	if(scan_value & V_SW_DPI_2)
	{
		*(key_status+4)=1;
		*(key_counts+4)+=1;
	}
	else
	{
		*(key_status+4)=0;
	}
	
	if(scan_value & V_Side_1)
	{
		*(key_status+5)=1;
		*(key_counts+5)+=1;
	}
	else
	{
		*(key_status+5)=0;
	}
	if(scan_value & V_Side_2)
	{
		*(key_status+6)=1;
		*(key_counts+6)+=1;
	}
	else
	{
		*(key_status+6)=0;
	}
	*/
		
		
}
//--------------------------------------------------------------------------------
#if 0
U16 S_GPIO_Scan(void)
{
	//U8 *key_status=(U8*)&Cur_Mouse_Status.key_status;
	//U16 *key_counts=(U16*)&Cur_Mouse_Status.counts;
	U16 R_GPIOScan_Value =0;
	U32 u32ButtonGPIO1 = 0;
	U32 u32ButtonGPIO0 = 0;
	
	u32ButtonGPIO0 = u32ButtonGPIO1 = R_GPIOScan_Value =0;	// for INIT Value 
	 
	u32ButtonGPIO1 = NDrv_GPIO_GetPinValue(PORT1,0xFF);	// Get All Port Data 
	u32ButtonGPIO0 = NDrv_GPIO_GetPinValue(PORT0,0xFF);	// Get All Port Data 

	///-------------------------------------------------------- Scan 
	//if(!(u32ButtonGPIO0 & PORT0_ALL_BUTTON))
	if(BUTTON_USER_DEFINE_FLAG==0)
	{
	//#if 0
//	#if P502==1
#ifdef P502
		if(!(u32ButtonGPIO1 & V_SW_L_Bit))
		{
			R_GPIOScan_Value |= V_SW_L;
			//*(key_status)=1;
			//*(key_counts)+=1;
			//Cur_Mouse_Status.counts.left++;
		}
		if(!(u32ButtonGPIO0 & V_SW_R_Bit))
		{
			R_GPIOScan_Value |= V_SW_R;
			//*(key_status+1)=1;
			//*(key_counts+1)+=1;
			
			//Cur_Mouse_Status.counts.right++;
			
		}
		if(!(u32ButtonGPIO0&V_SW_M_Bit))
		{
			R_GPIOScan_Value |= V_SW_M;
			//*(key_status+2)=1;
			//*(key_counts+2)+=1;
			//Cur_Mouse_Status.counts.middle++;
		}
		if(!(u32ButtonGPIO1&V_SW_DPI_1_Bit))
		{
			R_GPIOScan_Value |= V_SW_DPI_1;
			//*(key_status+3)=1;
			//*(key_counts+3)+=1;
			//Cur_Mouse_Status.counts.dpi1++;
		}

		if(!(u32ButtonGPIO1&V_SW_DPI_2_Bit))
		{
			R_GPIOScan_Value |= V_SW_DPI_2;
			//*(key_status+4)=1;
			//*(key_counts+4)+=1;
			//Cur_Mouse_Status.counts.dpi2++;
		}
		
		if(!(u32ButtonGPIO1 & V_Side_1_Bit))
		{
			R_GPIOScan_Value |= V_Side_1;
			//*(key_status+5)=1;
			//*(key_counts+5)+=1;
			//Cur_Mouse_Status.counts.key4++;
		}
		if(!(u32ButtonGPIO1 & V_Side_2_Bit))
		{
			R_GPIOScan_Value |= V_Side_2;
			//*(key_status+6)=1;
			//*(key_counts+6)+=1;
			//Cur_Mouse_Status.counts.key5++;
		}
	#else
		if(!(u32ButtonGPIO0 & V_SW_L_Bit))
		{
			R_GPIOScan_Value |= V_SW_L;
		}
		if(!(u32ButtonGPIO1 & V_SW_R_Bit))
		{
			R_GPIOScan_Value |= V_SW_R;			
		}
		if(!(u32ButtonGPIO1 & V_SW_M_Bit))
		{
			R_GPIOScan_Value |= V_SW_M;
		}
		if(!(u32ButtonGPIO0 & V_SW_DPI_1_Bit))
		{
			R_GPIOScan_Value |= V_SW_DPI_1;
		}		
		if(!(u32ButtonGPIO1 & V_L_Side_1_Bit))
		{
			R_GPIOScan_Value |= V_L_Side_1;
		}
		if(!(u32ButtonGPIO1 & V_L_Side_2_Bit))
		{
			R_GPIOScan_Value |= V_L_Side_2;
		}
		if(!(u32ButtonGPIO1 & V_R_Side_1_Bit))
		{
			R_GPIOScan_Value |= V_R_Side_1;
		}
		if(!(u32ButtonGPIO1 & V_R_Side_2_Bit))
		{
			R_GPIOScan_Value |= V_R_Side_2;
		}
	#endif
	}else{
		/*
		if(!(u32ButtonGPIO0 & V_SW_R_Bit))
		{
			R_GPIOScan_Value |= V_SW_R;
		}
		if(!(u32ButtonGPIO0&V_SW_M_Bit))
		{
			R_GPIOScan_Value |= V_SW_M;
		}
		if(!(u32ButtonGPIO1&V_SW_DPI_1_Bit))
		{
			R_GPIOScan_Value |= V_SW_DPI_1;
		}
		
		if(!(u32ButtonGPIO1&V_SW_DPI_2_Bit))
		{
			R_GPIOScan_Value |= V_SW_DPI_2;
		}
		
		if(!(u32ButtonGPIO1 & V_SW_L_Bit))
		{
			R_GPIOScan_Value |= V_SW_L;
		}
		if(!(u32ButtonGPIO1 & V_Side_1_Bit))
		{
			R_GPIOScan_Value |= V_Side_1;
		}
		if(!(u32ButtonGPIO1 & V_Side_2_Bit))
		{
			R_GPIOScan_Value |= V_Side_2;
		}
		*/
	}
		
	return R_GPIOScan_Value;
}



///--------------------------------
#if 0
void S_GPIO_Check_Key(void)
{

	U16 R_GPIO_Value =0;
	R_GPIO_Value = S_GPIO_Scan();


///----------------------------------------------------
	if(R_GPIO_Value != R_KeyStatus)
	{
		if(R_KeyScan_Bounce==0)
		{
			F_KeyScan_Change =1;	
			R_KeyStatus_Old = R_KeyStatus;
			R_KeyStatus = R_GPIO_Value;		// Reload Value
		}
	}
	else
	{
		if(F_KeyScan_Change==0)
		{
			if (rSuspendFlag ==1)
			{
				//R_KeyScan_Bounce = V_KeyScan_Bounce;		// ReCoaver Bounce Value					
				R_KeyScan_Bounce = 0;		// ReCoaver Bounce Value					
		 	}
		 	else
			{
				R_KeyScan_Bounce = V_KeyScan_Bounce;		// ReCoaver Bounce Value				
		 	}
		 							
		}
		else
		{
			///------------------------------------------------  Get KeyScan Change
			F_KeyScan_Change =0;			// Clear KeyScan Flag

			#ifdef V_KeyScan_Pin_Debug
			if(F_Togle)
			{
				F_Togle= 0x0;
				nsAPI_GPIO_LEDSetting(V_DPI_2_LED_Port,V_DPI_2_LED_Pin, V_LED_Active);		// LED Off				
			}
			else
			{
				F_Togle =0xff;
				nsAPI_GPIO_LEDSetting(V_DPI_2_LED_Port,V_DPI_2_LED_Pin, !V_LED_Active);		// LED On					
			}				
			#endif	
		

			#ifdef V_KeyScan_Print
				xprintf("\r\n P.Old.%x",R_KeyStatus_Old);
				xprintf("--P.New.%x",R_GPIO_Value);		
			#endif					


			if(R_KeyStatus ==0)
			{				// Get KeyRelaase

				#if 1
				g_Main_Setting.MouseInReport[1] = 0x00;	// Release All Key
				g_Main_Setting.MouseInReport[0] = 0x01;	//Mouse report ID			
				g_Repot_Process.Mouse_Report_In = 1;		// Set UpReport Flag	
			
				#else
				if(R_KeyStatus_Old &(V_Side_L1|V_Side_L2|V_Side_R1|V_Side_R2))
				{	
					g_Main_Setting.MediaInReport[1] = 0x00;	// Low Byte
					g_Main_Setting.MediaInReport[2] = 0x00;	// High Byte
					
					g_Main_Setting.MediaInReport[0] = 0x03;	// Send ID
					g_Repot_Process.Media_Report_In = 1;					
				}
				else
				{
					g_Main_Setting.MouseInReport[1] = 0x00;	// Release All Key
					g_Main_Setting.MouseInReport[0] = 0x01;	//Mouse report ID			
					g_Repot_Process.Mouse_Report_In = 1;		// Set UpReport Flag						
				}
				#endif
				
								
			}
			///-------------------------------------------	// Get Key Mark
			else
			{
						
				///==================================
				if(R_KeyStatus&(V_SW_L|V_SW_M|V_SW_R|V_Side_R1|V_Side_R2|V_Side_L1|V_Side_L2))	// When Get Nomo Key
				{
					//g_Main_Setting.MouseInReport[1] = R_KeyStatus & 0x07;	// UpData				
					g_Main_Setting.MouseInReport[1] = R_KeyStatus & 0x7f;	// UpData						
					g_Main_Setting.MouseInReport[0] = 0x01;					//Mouse report ID			
					g_Repot_Process.Mouse_Report_In = 1;					// Set UpReport Flag	
					
					///------------ Debug Item Use ------------
					#ifdef V_Page0_Print
					S_DATA_INIT();	
					#endif
				
					#ifdef V_Sensor_Debug
					if(g_Main_Setting.MouseInReport[1]==0x04)
					{
						S_9800_Shutdown();
					}
					else
					{
						S_ADNS9800_INIT();
					}										
					#endif	
					///------------ Debug Item Use ------------			
										
				}

				///==================================
				#if 0
				else if(R_KeyStatus &(V_Side_R1|V_Side_R2)){	// Get Vol Up/Down

					if(R_KeyStatus & V_Side_R1){
												
					// Vol + 
						g_Main_Setting.MediaInReport[1] = 0xE9;	// Low Byte						
						g_Main_Setting.MediaInReport[2] = 0x00;	// High Byte						
					}
					else{

					// Vol -	
						g_Main_Setting.MediaInReport[1] = 0xEA;	// Low Byte						
						g_Main_Setting.MediaInReport[2] = 0x00;	// High Byte							
					}
					
					g_Main_Setting.MediaInReport[0] = 0x03;	// Send ID	
					g_Repot_Process.Media_Report_In = 1;						
			
				}
			///==================================				
				else if(R_KeyStatus &(V_Side_L1|V_Side_L2)){	// Get Page ForWard/Back
				
					if(R_KeyStatus & V_Side_L1){
						
					// Forward
						g_Main_Setting.MediaInReport[1] = 0x25;	// Low Byte						
						g_Main_Setting.MediaInReport[2] = 0x02;	// High Byte
																			
					}
					else{
						
					// Back
						g_Main_Setting.MediaInReport[1] = 0x24;	// Low Byte						
						g_Main_Setting.MediaInReport[2] = 0x02;	// High Byte																			
					}
					
					g_Main_Setting.MediaInReport[0] = 0x03;	// Send ID
					g_Repot_Process.Media_Report_In = 1;	
				
				}

				#endif	
				///==================================

				else if(R_KeyStatus & V_SW_DPI)		// Get DPI Change
				{				
					///------ DPI Change MAX for 4 Level
					R_DPI_Index++;
					if(R_DPI_Index >3)
					{
						R_DPI_Index =0;
					}
					///-------
					//S_DPI_Change();	//2016_0506 Hsaing

					#ifdef V_DPIChange_Print
					xprintf("\r\nDPI-%x",R_DPI_Index);						
					#endif

					///------Save DPI Index to EX-Flash ------
					R_FlashPage_Write[CV_DPI_Index] = R_DPI_Index;

					#ifdef V_DPIChange_Print
					xprintf("\r\n EX-ADDR->",0);
					S_Addr_Show(V_Page_0);

					DS_TempFlashBuffer_Show();	
					#endif

					//Save parameter to flash
					S_EX_Flash_Erase_Sector(V_Sector_0);						
					S_EX_Flash_Write_Page(V_Page_0);	

					///------------------------------------ Check when get Wark Up Event

					g_Main_Setting.MouseInReport[1] = V_SW_DPI_USB;	// UpData
					g_Main_Setting.MouseInReport[0] = 0x01;				//Mouse report ID			
					g_Repot_Process.Mouse_Report_In = 1;				// Set UpReport Flag		
					g_GpioKeyboard.ButtonStatus = GPIO_KEY_CHANGED;		//2016_0408_Hsiang

					///------------------------------------ Check when get Wark Up Event
				}

							
			}

							
	///------------------------------------------------  Get KeyScan Change			
		}
		
	}


}
#else
void S_GPIO_Check_Key(void)
{
	U16 R_GPIO_Value =0;
	R_GPIO_Value = S_GPIO_Scan();

	if(R_GPIO_Value != R_KeyStatus)
	{
		if(R_KeyScan_Bounce==0)
		{
			F_KeyScan_Change =1;	
			R_KeyStatus_Old = R_KeyStatus;
			R_KeyStatus = R_GPIO_Value;		// Reload Value
			Button_Status_and_Conuts_Process(R_GPIO_Value);
		}
	}
	else
	{
		if(F_KeyScan_Change==0)
		{
			if (rSuspendFlag ==1)
			{		
				R_KeyScan_Bounce = 0;		// ReCoaver Bounce Value					
		 	}
		 	else
			{
				R_KeyScan_Bounce = V_KeyScan_Bounce;		// ReCoaver Bounce Value				
		 	}							
		}
		else
		{
			///------------------------------------------------  Get KeyScan Change
			F_KeyScan_Change =0;			// Clear KeyScan Flag

			#ifdef V_KeyScan_Pin_Debug
			if(F_Togle)
			{
				F_Togle= 0x0;
				nsAPI_GPIO_LEDSetting(V_DPI_2_LED_Port,V_DPI_2_LED_Pin, V_LED_Active);		// LED Off				
			}
			else
			{
				F_Togle =0xff;
				nsAPI_GPIO_LEDSetting(V_DPI_2_LED_Port,V_DPI_2_LED_Pin, !V_LED_Active);		// LED On					
			}				
			#endif	
		

			#ifdef V_KeyScan_Print
				xprintf("\r\n P.Old.%x",R_KeyStatus_Old);
				xprintf("--P.New.%x",R_GPIO_Value);		
			#endif					


			if(R_KeyStatus ==0)
			{	
				
				// Get KeyRelaase
				//org size is 8 byte, 2016_0608 become 6 byte and remove id
				//g_Main_Setting.MouseInReport[0] = 0x01;	//Mouse report ID
				//g_Main_Setting.MouseInReport[1] = 0x00;	// Release All Key
				g_Main_Setting.MouseInReport[0] = 0x00;	// Release All Key
				g_Repot_Process.Mouse_Report_In = 1;		// Set UpReport Flag	
				
								
			}
			///-------------------------------------------	// Get Key Mark
			else
			{			
				///==================================
				//if(R_KeyStatus&(V_SW_L|V_SW_M|V_SW_R|V_Side_1|V_Side_2 | V_Side_3))	// When Get Nomo Key
			//#if 0
			#ifdef P502
				if(R_KeyStatus&(V_SW_L|V_SW_M|V_SW_R|V_Side_1|V_Side_2 ))	// When Get Nomo Key		//20160809 Hsiang
			#else
				if(R_KeyStatus&(V_SW_L|V_SW_M|V_SW_R|V_L_Side_1|V_L_Side_2|V_R_Side_1|V_R_Side_2))	// When Get Nomo Key		//20160809 Hsiang
			#endif
				{
					//org size is 8 byte, 2016_0608 become 6 byte and remove id
					//g_Main_Setting.MouseInReport[1] = R_KeyStatus & 0x07;	// UpData				
					//g_Main_Setting.MouseInReport[0] = 0x01;					//Mouse report ID	
					g_Main_Setting.MouseInReport[0] = R_KeyStatus & 0x7f;	// UpData						
					
					g_Repot_Process.Mouse_Report_In = 1;					// Set UpReport Flag	

					#if 0
					///------------ Debug Item Use ------------
					#ifdef V_Page0_Print
					S_DATA_INIT();	
					#endif
				
					#ifdef V_Sensor_Debug
					if(g_Main_Setting.MouseInReport[0]==0x04)
					{
						S_9800_Shutdown();
					}
					else
					{
						S_ADNS9800_INIT();
					}										
					#endif	
					///------------ Debug Item Use ------------
					#endif
										
				}

				#endif	
				///==================================

				else if(R_KeyStatus & V_SW_DPI_1)		// Get DPI Change
				{				
					///------ DPI Change MAX for 4 Level
					R_DPI_Index++;
					if(R_DPI_Index >3)
					{
						R_DPI_Index =0;
					}
					///-------
					//S_DPI_Change();		//2016_0506 Hsaing
					SENSOR_DPI_SETTING(R_DPI_Index);	//2016_0519_Hsiang



					#if 0
					#ifdef V_DPIChange_Print
					xprintf("\r\nDPI-%x",R_DPI_Index);						
					#endif
					#endif

					///------Save DPI Index to EX-Flash ------
					//R_FlashPage_Write[CV_DPI_Index] = R_DPI_Index;	//2016_0519_Hsiang

					#if 0
					#ifdef V_DPIChange_Print
					xprintf("\r\n EX-ADDR->",0);
					S_Addr_Show(V_Page_0);

					DS_TempFlashBuffer_Show();	
					#endif
					#endif
					
					//Save parameter to flash
					//S_EX_Flash_Erase_Sector(V_Sector_0);				//2016_0519_Hsiang					
					//S_EX_Flash_Write_Page(V_Page_0);				//2016_0519_Hsiang

					///------------------------------------ Check when get Wark Up Event
					//org size is 8 byte, 2016_0608 become 6 byte and remove id
					//g_Main_Setting.MouseInReport[0] = 0x01;				//Mouse report ID			
					//g_Main_Setting.MouseInReport[1] = V_SW_DPI_USB;	// UpData
					g_Main_Setting.MouseInReport[0] = V_SW_DPI_USB;	// UpData
					g_Repot_Process.Mouse_Report_In = 1;				// Set UpReport Flag		
					g_GpioKeyboard.ButtonStatus = GPIO_KEY_CHANGED;		//2016_0408_Hsiang

					///------------------------------------ Check when get Wark Up Event
				}

							
			}

							
	///------------------------------------------------  Get KeyScan Change			
		}
		
	}


}
#endif
#if 0

#endif

///--------------------------------------------------------------------------------
void S_IO_Suspend_Setting(void){


///------------------------------------------ PWM
	LPC_IOCON->TDO_PIO0_13 &= ~0x07;		// PIO_13
	LPC_IOCON->TDO_PIO0_13 |= BIT0;			// Set to GPIO
	//LPC_IOCON->TDO_PIO0_13 |= BIT3;			// Pull Down
	//LPC_IOCON->TDO_PIO0_13 |= BIT10;			// Open-Drain Enable	

	LPC_IOCON->TRST_PIO0_14 &= ~0x07;		// PIO0_14
	LPC_IOCON->TRST_PIO0_14 |= BIT0;		// Set to GPIO	
	//LPC_IOCON->TRST_PIO0_14 |= BIT3;		// Pull Down
	//LPC_IOCON->TDO_PIO0_13 |= BIT10;			// Open-Drain Enable		


///------------------------------------------ LED 
	LPC_IOCON->PIO1_13 &= ~0x07;			// PIO1_13
	//LPC_IOCON->PIO1_13 |= BIT3;				// Pull Down
	//LPC_IOCON->TDO_PIO0_13 |= BIT10;			// Open-Drain Enable		

	LPC_IOCON->PIO1_14 &= ~0x07;			// PIO1_14
	//LPC_IOCON->PIO1_14 |= BIT3;				// Pull Down
	//LPC_IOCON->TDO_PIO0_13 |= BIT10;			// Open-Drain Enable		

	LPC_IOCON->PIO1_15 &= ~0x07;			// PIO1_15
	//LPC_IOCON->PIO1_15 |= BIT3;				// Pull Down
	//LPC_IOCON->TDO_PIO0_13 |= BIT10;			// Open-Drain Enable		



}

#if 0

#endif

///-------------------------------------------------------------------------------
///-------------------------------------------------------------------------------
#ifdef V_DPI_4Level_Print
#endif
#if 0

#endif
///-------------------------------------------------------------------------------
#if 0

#endif
///==============================================================

#ifdef V_FWDownLoad_Print
void DS_FWDownLoad_FlashShow(void)
{
}
#endif


///--------------------------------------------------------------------------------


///--------------------------------------------------------------------------------

void S_Clear_OutReport_Buffer(void){

	U8	R_TempLoop=0;

	for(R_TempLoop=0;R_TempLoop<USB_PACKET_SIZE;R_TempLoop++){
		g_Main_Setting.OutReport[R_TempLoop]=0x00;
	}	
}


void S_Clear_Feature_PartBuffer(void){

	U8	R_TempLoop=0;

	//for(R_TempLoop=0;R_TempLoop<USB_PACKET_SIZE;R_TempLoop++){
	for(R_TempLoop=0;R_TempLoop<V_EP0Buffer_Size;R_TempLoop++){		
		g_Main_Setting.FeatureInReport[R_TempLoop]=0x00;
	}	
}


#if 1
void S_Clear_EP0_Buffer(void){
	U8	R_TempLoop=0;

	for(R_TempLoop=0;R_TempLoop<V_EP0Buffer_Size;R_TempLoop++){
		g_USBCore_Setting.EP0Buf[R_TempLoop]=0x00;
	}
	 
}
#endif

#if 0

#endif
///--------------------------------------------------------------------------------
#if 0

#endif

#if 0

#endif
#endif
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------


