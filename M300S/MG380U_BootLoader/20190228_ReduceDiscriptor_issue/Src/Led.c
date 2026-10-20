//#include <SN32F240.h>
//#include <SN32F200_Def.h>
//#include <rtl.h>
#include <stdbool.h>
#include "LPC11Uxx.h"
#include "Mouse.h" 
#include "Led.h"
#include "Main.h" 
#include "I2c.h"
#include "gpio.h"
#include "Wheel.h"

extern volatile uint32_t I2CCount;
extern volatile uint8_t I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t I2CSlaveBuffer[BUFSIZE];
extern volatile uint8_t I2CMasterState;
extern volatile uint16_t I2CReadLength, I2CWriteLength;
extern void S_Idle_Delay_uSec(uint16_t V_Value);
extern void nsAPI_Timer16_B0M3_PWM_Init(U32 period);
extern void nsAPI_Timer32_B0M3_PWM_Init(U32 period);
extern void PWM_TIMER_ENABLE(bool enable);
extern bool LED_COLOR_CHANGE_FLAG;
extern U8 LED_Color_Wave_counts;
extern struct Led_Effect_t Cur_Led_Effect;

U8 LED_COLOR_TIMES;
//bool	LED_COLOR_FLAG;
//bool	LED_EFFECT_FLAG;
//U8 LED_Breath_Effect=0;	       //bit 0 : ALL, bit 1: logo, bit 2: wheel, bit 3: light pipe
//U8 LED_Rainbow_Effect=0;	//bit 0 : ALL, bit 1: logo, bit 2: wheel, bit 3: light pipe
U8 LED_Logo_Color_index=0;	//0=red, 1=green, 2=blue
U8 LED_Color_Wave_Color_index=0;	//0=red, 1=green, 2=blue
U8 LED_Color_Cycle_Color_index=0;	//0=red, 1=green, 2=blue
U8 LED_Lightpipe_Breath_Flag=0; //0=none, 1=first step, 2=sencond step;
U8 LED_Lightpipe_Color_Cycle_Flag=0; //0=none, 1=first step, 2=sencond step;
bool LED_Lightpipe_Color_Wave_Flag=0;
//bool LED_Lightpipe_Color_Cycle_Flag=0;
HsvColor HSV;
U8 HSV_H;





struct Led_Effect_Flag_t Led_Effect_Flag;


static U8 ALL_R_Table_Index[]=
{
	D2_R_PWM_REGISTER,
	D1_R_PWM_REGISTER,
	FL1_R_PWM_REGISTER, 
	FL2_R_PWM_REGISTER, 
	FL3_R_PWM_REGISTER, 
	FL4_R_PWM_REGISTER, 
	FL5_R_PWM_REGISTER, 
	FL6_R_PWM_REGISTER, 
	FL7_R_PWM_REGISTER, 
	FL8_R_PWM_REGISTER, 
	D107_R_PWM_REGISTER,
	FL9_R_PWM_REGISTER, 
	FL10_R_PWM_REGISTER, 
	FL11_R_PWM_REGISTER, 
	FL12_R_PWM_REGISTER, 
	FL13_R_PWM_REGISTER, 
	FL14_R_PWM_REGISTER, 
	FL15_R_PWM_REGISTER, 
	FL16_R_PWM_REGISTER,	
};

static U8 ALL_G_Table_Index[]=
{
	D2_G_PWM_REGISTER,
	D1_G_PWM_REGISTER,
	FL1_G_PWM_REGISTER, 
	FL2_G_PWM_REGISTER, 
	FL3_G_PWM_REGISTER, 
	FL4_G_PWM_REGISTER, 
	FL5_G_PWM_REGISTER, 
	FL6_G_PWM_REGISTER, 
	FL7_G_PWM_REGISTER, 
	FL8_G_PWM_REGISTER, 
	D107_G_PWM_REGISTER,
	FL9_G_PWM_REGISTER, 
	FL10_G_PWM_REGISTER, 
	FL11_G_PWM_REGISTER, 
	FL12_G_PWM_REGISTER, 
	FL13_G_PWM_REGISTER, 
	FL14_G_PWM_REGISTER, 
	FL15_G_PWM_REGISTER, 
	FL16_G_PWM_REGISTER,	
};

static U8 ALL_B_Table_Index[]=
{
	D2_B_PWM_REGISTER,
	D1_B_PWM_REGISTER,
	FL1_B_PWM_REGISTER, 
	FL2_B_PWM_REGISTER, 
	FL3_B_PWM_REGISTER, 
	FL4_B_PWM_REGISTER, 
	FL5_B_PWM_REGISTER, 
	FL6_B_PWM_REGISTER, 
	FL7_B_PWM_REGISTER, 
	FL8_B_PWM_REGISTER, 
	D107_B_PWM_REGISTER,
	FL9_B_PWM_REGISTER, 
	FL10_B_PWM_REGISTER, 
	FL11_B_PWM_REGISTER, 
	FL12_B_PWM_REGISTER, 
	FL13_B_PWM_REGISTER, 
	FL14_B_PWM_REGISTER, 
	FL15_B_PWM_REGISTER, 
	FL16_B_PWM_REGISTER,	
};

//--------------------------------------------

static U8 LightPipe_R_Table_Index[]=
{
	FL1_R_PWM_REGISTER, 
	FL2_R_PWM_REGISTER, 
	FL3_R_PWM_REGISTER, 
	FL4_R_PWM_REGISTER, 
	FL5_R_PWM_REGISTER, 
	FL6_R_PWM_REGISTER, 
	FL7_R_PWM_REGISTER, 
	FL8_R_PWM_REGISTER, 
	FL9_R_PWM_REGISTER, 
	FL10_R_PWM_REGISTER, 
	FL11_R_PWM_REGISTER, 
	FL12_R_PWM_REGISTER, 
	FL13_R_PWM_REGISTER, 
	FL14_R_PWM_REGISTER, 
	FL15_R_PWM_REGISTER, 
	FL16_R_PWM_REGISTER,	
};

static U8 LightPipe_G_Table_Index[]=
{
	FL1_G_PWM_REGISTER, 
	FL2_G_PWM_REGISTER, 
	FL3_G_PWM_REGISTER, 
	FL4_G_PWM_REGISTER, 
	FL5_G_PWM_REGISTER, 
	FL6_G_PWM_REGISTER, 
	FL7_G_PWM_REGISTER, 
	FL8_G_PWM_REGISTER, 
	FL9_G_PWM_REGISTER, 
	FL10_G_PWM_REGISTER, 
	FL11_G_PWM_REGISTER, 
	FL12_G_PWM_REGISTER, 
	FL13_G_PWM_REGISTER, 
	FL14_G_PWM_REGISTER, 
	FL15_G_PWM_REGISTER, 
	FL16_G_PWM_REGISTER,	
};

static U8 LightPipe_B_Table_Index[]=
{
	FL1_B_PWM_REGISTER, 
	FL2_B_PWM_REGISTER, 
	FL3_B_PWM_REGISTER, 
	FL4_B_PWM_REGISTER, 
	FL5_B_PWM_REGISTER, 
	FL6_B_PWM_REGISTER, 
	FL7_B_PWM_REGISTER, 
	FL8_B_PWM_REGISTER, 
	FL9_B_PWM_REGISTER, 
	FL10_B_PWM_REGISTER, 
	FL11_B_PWM_REGISTER, 
	FL12_B_PWM_REGISTER, 
	FL13_B_PWM_REGISTER, 
	FL14_B_PWM_REGISTER, 
	FL15_B_PWM_REGISTER, 
	FL16_B_PWM_REGISTER,	
};


/*
static U8 Wheel_R_Table_Index[]=
{
	D1_R_PWM_REGISTER, 
	D2_R_PWM_REGISTER
};

static U8 Wheel_G_Table_Index[]=
{
	D1_G_PWM_REGISTER, 
	D2_G_PWM_REGISTER
};

static U8 Wheel_B_Table_Index[]=
{
	D1_B_PWM_REGISTER, 
	D2_B_PWM_REGISTER
};
*/
/*
#define M_KEY_LED_ON  		SN_GPIO2->DATA &= ~(1<<14)
#define M_KEY_LED_OFF 		SN_GPIO2->DATA |= (1<<14)
#define G_KEY_LED_ON 		SN_GPIO2->DATA &= ~(1<<15)
#define G_KEY_LED_OFF 		SN_GPIO2->DATA |= (1<<15)
#define SCROLL_LOCK_LED_ON 	SN_GPIO3->DATA &= ~(1<<12)
#define SCROLL_LOCK_LED_OFF SN_GPIO3->DATA |= (1<<12)
#define CAPS_LOCK_LED_ON 	SN_GPIO3->DATA &= ~(1<<13)
#define CAPS_LOCK_LED_OFF 	SN_GPIO3->DATA |= (1<<13)
#define NUMS_LOCK_LED_ON 	SN_GPIO3->DATA &= ~(1<<14)
#define NUMS_LOCK_LED_OFF 	SN_GPIO3->DATA |= (1<<14)


#define		I2C_NO_REPEAT			0				//No use repeat
#define		I2C_REPEAT_START		1				//Next transfer with a Repated Start condition
#define		I2C_NEXT_START			2				//Work next transfer
#define		I2C_MAS_ACK_1BYTE		0				//Send (wReTxNum + 1) Data when wRepeatTX is 1.
*/
#define		I2C_LED_DRIVER_ADDR		0xE8
//LED driver 
#define	LED_SDB_PIN_CONFIG LPC_IOCON->PIO1_26

const U8 m_rainbow[12][3]=
{	//	r	 b	  g
	{ 255,   0,   0},
	{ 178,   0,  67},
	{ 128,   0, 128},
	{  67,   0, 178},
	{   0,   0, 255},
	{   0,  67, 178},
	{   0, 128, 128},
	{   0, 178,  67},
	{   0, 255,   0},
	{  67, 178,   0},
	{ 128, 128,   0},
	{ 178,  67,   0}
};


#define FRAM1 0x00
#define FRAM2 0x01
#define FRAM3 0x02
#define FRAM4 0x03
#define FRAM5 0x04
#define FRAM6 0x05
#define FRAM7 0x06
#define FRAM8 0x07
#define FUNCTION_FRAME 0x0B

#define MATRIXA_CA1_CTR 0x00
#define MATRIXA_CA2_CTR 0x02
#define MATRIXA_CA3_CTR 0x04
#define MATRIXA_CA4_CTR 0x06
#define MATRIXA_CA5_CTR 0x08
#define MATRIXA_CA6_CTR 0x0A
#define MATRIXA_CA7_CTR 0x0C
#define MATRIXA_CA8_CTR 0x0E
#define MATRIXA_CA9_CTR 0x10
	


/*
#define MatrixB_CB1_CTR 0x01
#define MatrixB_CB2_CTR 0x03
#define MatrixB_CB3_CTR 0x05
#define MatrixB_CB4_CTR 0x07
#define MatrixB_CB5_CTR 0x09
#define MatrixB_CB6_CTR 0x0B
#define MatrixB_CB7_CTR 0x0D
#define MatrixB_CB8_CTR 0x0F
#define MatrixB_CB9_CTR 0x11
*/
#define MatrixA_CA1_BLINK 0x12
#define MatrixA_CA2_BLINK 0x14
#define MatrixA_CA3_BLINK 0x16
#define MatrixA_CA4_BLINK 0x18
#define MatrixA_CA5_BLINK 0x1A
#define MatrixA_CA6_BLINK 0x1C
#define MatrixA_CA7_BLINK 0x1E
#define MatrixA_CA8_BLINK 0x20
#define MatrixA_CA9_BLINK 0x22


 /*
#define MatrixB_CB1_BLINK 0x13
#define MatrixB_CB2_BLINK 0x15
#define MatrixB_CB3_BLINK 0x17
#define MatrixB_CB4_BLINK 0x19
#define MatrixB_CB5_BLINK 0x1B
#define MatrixB_CB6_BLINK 0x1D
#define MatrixB_CB7_BLINK 0x1F
#define MatrixB_CB8_BLINK 0x21
#define MatrixB_CB9_BLINK 0x23
*/

/*
#define PWM_Register_0	0x24

uint8_t SHOW_WIN_FRAM=0x04;
uint8_t LED_FUNCTION_FRAME = 0x0B;
uint8_t I2C_TX_LENGTH=0;
uint8_t I2C_RX_LENGTH=0;
uint8_t bI2C_MasEndFlag = 0;
uint8_t bI2C_SlaEndFlag = 0;
uint8_t bI2C_MasTxPointer=0;
uint8_t bI2C_MasRxPointer=0;

uint32_t LED_DELAY=0;

U8 CUR_CA1_VALUE=0;
U8 CUR_CA2_VALUE=0;
U8 CUR_CA3_VALUE=0;
U8 CUR_CA4_VALUE=0;
U8 CUR_CA5_VALUE=0;
U8 CUR_CA6_VALUE=0;
U8 CUR_CA7_VALUE=0;
U8 CUR_CA8_VALUE=0;
U8 CUR_CA9_VALUE=0;

bool RW=1;	//0:read, 1:write

static U8 LED_PWM_REGISTER[]=
{
	0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
	0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B,
	0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
	0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B,
	0x84, 0x85, 0x86, 0x87, 0x00, 0x00, 0x00, 0x00,
	0x94, 0x95, 0x96, 0x97, 0x00, 0x00, 0x00, 0x00,
	0xA4, 0xA5, 0xA6, 0xA7, 0x00, 0x00, 0x00, 0x00
};

extern uint8_t VENDER_CMD_FLAG;
extern uint8_t MAIN_BL_CURRENT;
extern uint8_t ARROW_BL_CURRENT;
extern uint8_t CAPS_LED;
extern uint8_t CAPS_LOCK_LED_FLAG;
extern uint8_t	Consuner_Data[32];
extern void NOP(void);
*/
#if 0
//--------------------------------------------------------------------------
void I2C_TX(void)
{
	uint32_t i=0;
	RW=1;
	SN_I2C1->CTRL |= (0x01 << 5);		//Start		   
	while (bI2C_MasEndFlag == 0);
	bI2C_MasEndFlag = 0;

	if(wI2C_TotalError != 0x00){
		while(1);					//Happen Error
	}
	for( i = 0; i< 1000; i++);
}
//--------------------------------------------------------------------------
void I2C_RX(void)
{
	uint32_t i=0;
	RW=0;
	SN_I2C1->CTRL |= (0x01 << 5);		//Start		   
	while (bI2C_MasEndFlag == 0);
	bI2C_MasEndFlag = 0;
	if(wI2C_TotalError != 0x00){
		while(1);					//Happen Error
	}
	for( i = 0; i< 1000; i++);
}
//--------------------------------------------------------------------------
void LED_Frame_Setting(uint8_t reg )
{
	bI2C_MasTxData[0] = 0xFD;
	bI2C_MasTxData[1] = reg;
	nsAPI_I2C_Write_Database(I2C_LED_DRIVER_ADDR, bI2C_MasTxData, 2);
}

//--------------------------------------------------------------------------
void LED_Register_Setting(uint8_t reg, uint8_t length)
{
	bI2C_MasTxData[0] = reg;
	nsAPI_I2C_Write_Database(I2C_LED_DRIVER_ADDR, bI2C_MasTxData, length);
}
//--------------------------------------------------------------------------
void LED_CONTROL_REGISTER_SEETTING(uint8_t reg, uint8_t data)
{
	bI2C_MasTxData[0] = reg;
	bI2C_MasTxData[1] = data;
	nsAPI_I2C_Write_Database(I2C_LED_DRIVER_ADDR, bI2C_MasTxData, length);
}
#endif
//--------------------------------------------------------------------------
void LED_Frame_Setting(uint8_t reg )
{
	
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = 0xFD;
	g_I2C_Setting.u8I2CMasterBuffer[2] = reg;
	nsAPI_I2C_Engine( );
}
//--------------------------------------------------------------------------
void ISSI3731_Software_showdown(bool enable)
{	
	LED_Frame_Setting(FUNCTION_FRAME);
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = 0x0A;
	if(enable)
		g_I2C_Setting.u8I2CMasterBuffer[2] = 0x00;
	else
		g_I2C_Setting.u8I2CMasterBuffer[2] = 0x01;
	nsAPI_I2C_Engine( );
}
//--------------------------------------------------------------------------
#if 0
void SET_ONLY_RED_ON(void)
{

	U8 i, j, step=16;//, length;
	
	#if 0
	LED_Frame_Setting(FRAM1);
	g_I2C_Setting.u8I2CWriteLength = 146;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER;
	for(i=0; i<144; i++)
	{
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_RED_TABLE[i];
	}
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA4_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA7_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xF0;
	nsAPI_I2C_Engine( );
	
	#else
	
	LED_Frame_Setting(FRAM1);	
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA4_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA7_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xF0;
	nsAPI_I2C_Engine( );

	
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<144; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
		g_I2C_Setting.u8I2CWriteLength = step+2;
		for(j=0; j<step; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = 0xFF;

		}
		nsAPI_I2C_Engine( );
	}

	#endif
	
}
//--------------------------------------------------------------------------
void SET_ONLY_GREEN_ON(void)
{
	U8 i;
	LED_Frame_Setting(FRAM1);
	g_I2C_Setting.u8I2CWriteLength = 146;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER;
	for(i=0; i<144; i++)
	{
		//g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_GREEN_TABLE[i];
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = 0;
	}
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA2_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA5_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA8_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xF0;
	nsAPI_I2C_Engine( );
	
}
//--------------------------------------------------------------------------
void SET_ONLY_BLUE_ON(void)
{
	U8 i;
	LED_Frame_Setting(FRAM1);
	g_I2C_Setting.u8I2CWriteLength = 146;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER;
	for(i=0; i<144; i++)
	{
		//g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_BLUE_TABLE[i];
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = 0;
	}
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA3_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA6_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA9_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xF0;
	nsAPI_I2C_Engine( );
}

//--------------------------------------------------------------------------
void SET_ALL_LED_OFF(void)
{

	U8 i, j, step=48;//, length;
	//SET FRAME
	
#if 0
	LED_Frame_Setting(FRAM1);		
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CWriteLength = 38;
	for(i=0; i<36; i++)
	{		
		g_I2C_Setting.u8I2CReadLength = 0;
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = 0x00;
	}
	nsAPI_I2C_Engine( );
	
	//set pwm to zero
	g_I2C_Setting.u8I2CWriteLength = 146;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER;
	for(i=0; i<144; i++)
	{
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = 0x00;
	}
	nsAPI_I2C_Engine( );
#else
	
	LED_Frame_Setting(FRAM1);	
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CWriteLength = 38;
	for(i=0; i<36; i++)
	{		
		g_I2C_Setting.u8I2CReadLength = 0;
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = 0x00;
	}
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<144; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
		g_I2C_Setting.u8I2CWriteLength = step+2;
		for(j=0; j<step; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = 0;

		}
		nsAPI_I2C_Engine( );
	}
#endif


}

#endif
//--------------------------------------------------------------------------
void SET_ALL_LED_ON(U8 color)
{
	U8 i, j, step=8;//, length;
	U8 LED_Ctrl_Idx=0;
	//SET FRAME
	LED_Frame_Setting(FRAM1);		
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CWriteLength = 20;
	g_I2C_Setting.u8I2CReadLength = 0;
	for(i=0; i<18; i++)
	{			
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_CTRL_REGISTER_ON_SETTING[i];
	}
	nsAPI_I2C_Engine( );

	
	//clear all pwm register
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = step+2;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	// set led color
	for(i=0; i<144; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = FIRST_COLOR_PWM_REGISTER+i;
		
		for(j=0; j<step; j++)
		{
			if(LED_CTRL_REGISTER_ON_SETTING[LED_Ctrl_Idx]&(1<<j))
				g_I2C_Setting.u8I2CMasterBuffer[2+j] = color;
			else
				g_I2C_Setting.u8I2CMasterBuffer[2+j] = 0;

		}
		LED_Ctrl_Idx++;
		nsAPI_I2C_Engine( );
	}

}

//--------------------------------------------------------------------------
#if 0
void SET_ALL_LED_ON_WITH_PWM0(void)
{
	U8 i, j, step=48;
	//SET FRAME
	LED_Frame_Setting(FRAM1);		
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CWriteLength = 20;
	for(i=0; i<18; i++)
	{		
		g_I2C_Setting.u8I2CReadLength = 0;
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_CTRL_REGISTER_ON_SETTING[i];
	}
	nsAPI_I2C_Engine( );
	
	//set pwm to 255
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<144; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
		g_I2C_Setting.u8I2CWriteLength = step+2;
		for(j=0; j<step; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = 0;

		}
		nsAPI_I2C_Engine( );
	}
}

//--------------------------------------------------------------------------
void SET_ALL_LED_ON_R230(void)
{
	U8 i, j, step=48;//, length;
	//SET FRAME
	LED_Frame_Setting(FRAM1);		
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CWriteLength = 20;
	for(i=0; i<18; i++)
	{		
		g_I2C_Setting.u8I2CReadLength = 0;
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_CTRL_REGISTER_ON_SETTING[i];
	}
	nsAPI_I2C_Engine( );
	
	//set pwm to 255
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<144; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
		g_I2C_Setting.u8I2CWriteLength = step+2;
		for(j=0; j<step; j++)
		{
			//g_I2C_Setting.u8I2CMasterBuffer[2+j] = LED_ALL_RED230_TABLE[i];
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = 0;

		}
		nsAPI_I2C_Engine( );
	}
}
//--------------------------------------------------------------------------
void SET_SCROOL_LED_ON(void)
{
	//U8 i, j, step=48;//, length;
	//SET FRAME
	LED_Frame_Setting(FRAM1);		
	//set blink and control off
	g_I2C_Setting.u8I2CWriteLength = 3;
	

	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = D35_R_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );
	
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = D35_G_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );

	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = D35_B_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
	nsAPI_I2C_Engine( );


	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA7_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0x04;
	nsAPI_I2C_Engine( );
	
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA8_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0x04;
	nsAPI_I2C_Engine( );
	
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA9_CTR;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0x04;
	nsAPI_I2C_Engine( );
	
	#if 0
	//set pwm to 255
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<144; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
		g_I2C_Setting.u8I2CWriteLength = step+2;
		for(j=0; j<step; j++)
		{
				g_I2C_Setting.u8I2CMasterBuffer[2+j] = 255;

		}
		nsAPI_I2C_Engine( );
	}
	#endif
}
#endif
//--------------------------------------------------------------------------
void Idle_Delay_Sec(U16 sec)
{			// Reset Idle 60mSec
	U16 i;
	for(i=0; i<(100*sec); i++)
		S_Idle_Delay_uSec(100);			// Reset Idle 10mSec
}

//--------------------------------------------------------------------------
#if 0
void ISSI3731_Driver_Default_Setting(void)
{
	LED_Frame_Setting(LED_FUNCTION_FRAME);
	bI2C_MasTxData[0] = 0x00;		//from first fuction register(0x00)
	bI2C_MasTxData[1] = 0x00;		//first function register data
	bI2C_MasTxData[2] = 0x00;		//second function register data
	bI2C_MasTxData[3] = 0x00;		
	bI2C_MasTxData[4] = 0x00;
	nsAPI_I2C_Write_Database(I2C_LED_DRIVER_ADDR, bI2C_MasTxData, 5);
	bI2C_MasTxData[0] = 0x05;
	bI2C_MasTxData[1] = 0x00;
	bI2C_MasTxData[2] = 0x00;
	bI2C_MasTxData[3] = 0x00;
	bI2C_MasTxData[4] = 0x77;
	bI2C_MasTxData[5] = 0x04;
	bI2C_MasTxData[6] = 0x00;
	bI2C_MasTxData[7] = 0x00;
	bI2C_MasTxData[8] = 0x00;
	nsAPI_I2C_Write_Database(I2C_LED_DRIVER_ADDR, bI2C_MasTxData, 9);
	for(i=0; i<4;i++){	
		//SET FRAME
		LED_Frame_Setting(i);
		bI2C_MasTxData[0] = LED_Control_Register_0;
		for(j=0;j<18;j++)
			bI2C_MasTxData[1+j] = LED_CTRL_TABLE[j];
		nsAPI_I2C_Write_Database(I2C_LED_DRIVER_ADDR, bI2C_MasTxData, 19);

		//LED BLINK
		bI2C_MasTxData[0] = Blink_Control_Register_0;
		for(j=0;j<18;j++)
			bI2C_MasTxData[1+j] = BLINK_P1_CTRL_TABLE[j];
		nsAPI_I2C_Write_Database(I2C_LED_DRIVER_ADDR, bI2C_MasTxData, 19);

		if(i==0)	ptr=(uint8_t*)LED_P1_PWM_TABLE;
		else if(i==1)	ptr=(uint8_t*)LED_P2_PWM_TABLE;
		else if(i==2)	ptr=(uint8_t*)LED_P3_PWM_TABLE;
		else if(i==3)	ptr=(uint8_t*)LED_P4_PWM_TABLE;

		bI2C_MasTxData[0] = PWM_Register_0;
		for(j=0;j<144;j++)
			bI2C_MasTxData[j+1] = ptr[j];
		nsAPI_I2C_Write_Database(I2C_LED_DRIVER_ADDR, bI2C_MasTxData, 145);
	}

	
}

#endif

//--------------------------------------------------------------------------
#if 0
void SET_ALL_LED_FADE_IN(void)
{
	U8 i, j, step;
	//U8 value=0;
	S8 index;
	step=48;
	
#if 0
	//SET FRAME
	LED_Frame_Setting(FRAM1);	
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	

		for(i=0; i<144; i++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
			g_I2C_Setting.u8I2CMasterBuffer[2] = 0;
			nsAPI_I2C_Engine( );
		}
		

	
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CWriteLength = 20;
	for(i=0; i<18; i++)
	{		

		g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_CTRL_REGISTER_ON_SETTING[i];
	}
	nsAPI_I2C_Engine( );
	
	//set pwm to zero
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	
	while(value!=0xff)
	{
		for(i=0; i<144; i++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
			g_I2C_Setting.u8I2CMasterBuffer[2] = value;
			nsAPI_I2C_Engine( );
		}
		
		S_Idle_Delay_uSec(100);			// Reset Idle 500mSec

		value++;
	}
#else	
	
	LED_Frame_Setting(FRAM1);
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CWriteLength = 20;
	for(i=0; i<18; i++)
	{		
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_CTRL_REGISTER_ON_SETTING[i];
	}
	nsAPI_I2C_Engine( );
	
	//set pwm to zero
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	
	for(index=0; index<32; index++)
	{
		for(i=0; i<144; i+=step)
		{
			g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
			g_I2C_Setting.u8I2CWriteLength = step+2;
			for(j=0; j<step; j++)
			{
				g_I2C_Setting.u8I2CMasterBuffer[2+j] = LED_PWM_TABLE[index];
			}
			nsAPI_I2C_Engine( );
		}
		S_Idle_Delay_uSec(150);			// Reset Idle 500mSec
	}
	#endif
}
//--------------------------------------------------------------------------
void SET_ALL_LED_FADE_OUT(void)
{
	U8 i, j, step;
	//U8 value=0xff;
	S8 index=0;
	step=48;
	LED_Frame_Setting(FRAM1);
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	g_I2C_Setting.u8I2CWriteLength = 20;
	for(i=0; i<18; i++)
	{		
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_CTRL_REGISTER_ON_SETTING[i];
	}
	nsAPI_I2C_Engine( );
	
	//set pwm to zero
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	
	for(index=31; index>=0; index--)
	{
		for(i=0; i<144; i+=step)
		{
			g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
			g_I2C_Setting.u8I2CWriteLength = step+2;
			for(j=0; j<step; j++)
			{
				g_I2C_Setting.u8I2CMasterBuffer[2+j] = LED_PWM_TABLE[index];
			}
			nsAPI_I2C_Engine( );
		}
		
		S_Idle_Delay_uSec(150);			// Reset Idle 500mSec
	}
}
//--------------------------------------------------------------------------
void LED_ALL_PWM_SETTING(U8 PWM_INDEX)
{
	U8 i, j, step=16;
	//set all red led pwm level
	LED_Frame_Setting(FRAM1);
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	for(i=0; i<144; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
		g_I2C_Setting.u8I2CWriteLength = step+2;
		for(j=0; j<step; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = LED_PWM_TABLE[PWM_INDEX];
		}
		nsAPI_I2C_Engine( );
	}
}
//--------------------------------------------------------------------------
void LED_PWM_VALUE_SETTING(U8 no, U8 pwm)
{
	LED_Frame_Setting(FRAM1);
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = no;
	g_I2C_Setting.u8I2CMasterBuffer[2] = pwm;
	nsAPI_I2C_Engine( );
}
//--------------------------------------------------------------------------
void LED_CONTROL_SETTING(U8 no, U8 ctr)
{
	LED_Frame_Setting(FRAM1);
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = no;
	g_I2C_Setting.u8I2CMasterBuffer[2] = ctr;
	nsAPI_I2C_Engine( );
}
//--------------------------------------------------------------------------
void LED_TEST_FUNCTION(void)
{
	U8 i=0,j=0, k;
	/*
	for(i=0; i<16; i++)
	{
		LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[i], 255 );
		//S_Idle_Delay_uSec(10);	//Idle 1ms
	}
	LED_CONTROL_SETTING(0x00, 0xFF );
	LED_CONTROL_SETTING(0x06, 0xFF );
	LED_CONTROL_SETTING(0x00, 0x00 );
	LED_CONTROL_SETTING(0x06, 0x00 );
	*/
	LED_CONTROL_SETTING(0x00, 0xFF );
	LED_CONTROL_SETTING(0x06, 0xFF );
	LED_CONTROL_SETTING(0x02, 0xFF );
	LED_CONTROL_SETTING(0x08, 0xFF );
	LED_CONTROL_SETTING(0x04, 0xFF );
	LED_CONTROL_SETTING(0x0A, 0xFF );
	while(1)
	{
		for(i=0; i<16; i++)
		{
			for(j=0, k=16-i; j<i; j++, k++)
			{
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
			for(k=0; j<16; j++, k++)
			{
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
		}
	
		for(i=0; i<16; i++)
		{
			for(j=0, k=16-i; j<i; j++, k++)
			{
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
			for(k=0; j<16; j++, k++)
			{
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
		}
		for(i=0; i<16; i++)
		{
			for(j=0, k=16-i; j<i; j++, k++)
			{
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
			for(k=0; j<16; j++, k++)
			{
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
		}
	}
}
//--------------------------------------------------------------------------
void LED_TEST_FUNCTION_1(void)
{
	U8 i=0,j=0, k;
	/*
	for(i=0; i<16; i++)
	{
		LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[i], 255 );
		//S_Idle_Delay_uSec(10);	//Idle 1ms
	}
	LED_CONTROL_SETTING(0x00, 0xFF );
	LED_CONTROL_SETTING(0x06, 0xFF );
	LED_CONTROL_SETTING(0x00, 0x00 );
	LED_CONTROL_SETTING(0x06, 0x00 );
	*/
	LED_CONTROL_SETTING(0x00, 0xFF );
	LED_CONTROL_SETTING(0x06, 0xFF );
	LED_CONTROL_SETTING(0x02, 0xFF );
	LED_CONTROL_SETTING(0x08, 0xFF );
	LED_CONTROL_SETTING(0x04, 0xFF );
	LED_CONTROL_SETTING(0x0A, 0xFF );
	while(1)
	{
		for(i=0; i<16; i++)
		{
			for(j=0, k=16-i; j<i; j++, k--)
			{
				//LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], 0 );
				//LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
			for(k=0; j<16; j++, k++)
			{
				//LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], 0 );
				//LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
		}
	#if 0
		for(i=0; i<16; i++)
		{
			for(j=0, k=16-i; j<i; j++, k++)
			{
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
			for(k=0; j<16; j++, k++)
			{
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
		}
		for(i=0; i<16; i++)
		{
			for(j=0, k=16-i; j<i; j++, k++)
			{
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
			for(k=0; j<16; j++, k++)
			{
				LED_CONTROL_SETTING(LED_RED_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_GREEN_PWM_REGISTER_TABLE[j], 0 );
				LED_CONTROL_SETTING(LED_BLUE_PWM_REGISTER_TABLE[j], LED_PWM_TABLE_1[k] );
				S_Idle_Delay_uSec(30);	//Idle 1ms
			}
		}
		#endif
	}
}
//-------------------------------------------------------------------
void LED_TEST(void)
{
	//U8 i;
	//LED_ALL_PWM_SETTING(0);
	LED_TEST_FUNCTION();
	while(1)
	{
		
		/*
		SET_ALL_LED_OFF();
		SET_ONLY_RED_ON();
		Idle_Delay_Sec(1);			// Reset Idle 1Sec
		SET_ALL_LED_OFF();
		SET_ONLY_GREEN_ON();
		Idle_Delay_Sec(1);			// Reset Idle 1Sec
		SET_ALL_LED_OFF();
		SET_ONLY_BLUE_ON();
		Idle_Delay_Sec(1);			// Reset Idle 1Sec
		*/
		/*
		LED_Frame_Setting(FRAM1);
		g_I2C_Setting.u8I2CWriteLength = 3;
		g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
		g_I2C_Setting.u8I2CMasterBuffer[1] = D74_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
		nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CWriteLength = 3;
		g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
		g_I2C_Setting.u8I2CMasterBuffer[1] = D75_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = 0xFF;
		nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CWriteLength = 3;
		g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
		g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA6_CTR;
		g_I2C_Setting.u8I2CMasterBuffer[2] = 0x28;
		nsAPI_I2C_Engine( );
		*/
		//2015_0520_HSIANG
		/*
		SET_ALL_LED_FADE_IN();
		SET_ALL_LED_FADE_OUT();
		*/
		//2015_0523_HSIANG
		/*
		for(i=0; i<16; i++)
		{
			if(i==0)
			{
				LED_CONTROL_SETTING(LED_RED_REGISTER_TABLE[15] , 0);
				LED_CONTROL_SETTING(LED_RED_REGISTER_TABLE[0], LED_REGISTER_BIT_TABLE[0] );
			}
			else
			{
				LED_CONTROL_SETTING(LED_RED_REGISTER_TABLE[i-1] , 0);
				LED_CONTROL_SETTING(LED_RED_REGISTER_TABLE[i], LED_REGISTER_BIT_TABLE[i] );
			}
			S_Idle_Delay_uSec(500);			// Reset Idle 			
		}
		*/
			
		
		};
	
}
#endif
//--------------------------------------------------------------------------
void ISSI3731_Driver_Default_Setting(void)
{
	/*
	U8 i;//, length;
	//SET FRAME
		
	//set blink and control off
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = MATRIXA_CA1_CTR;
	for(i=0; i<36; i++)
	{
		g_I2C_Setting.u8I2CWriteLength = 38;
		g_I2C_Setting.u8I2CReadLength = 0;
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = LED_CTRL_REGISTER_DEFAULT_SETTING[i];
	}
	nsAPI_I2C_Engine( );
	
	//set pwm to zero
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER;
	for(i=0; i<144; i++)
	{
		g_I2C_Setting.u8I2CMasterBuffer[2+i] = 0x00;
	}
	nsAPI_I2C_Engine( );
	*/
	#if 1
	LED_Frame_Setting(FUNCTION_FRAME);
	
	g_I2C_Setting.u8I2CWriteLength = 15;
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;
	g_I2C_Setting.u8I2CMasterBuffer[1] = Configuration_Reg;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0x00;	//Configuration_Reg value
	g_I2C_Setting.u8I2CMasterBuffer[3] = 0x00;	//Picture_Display_Reg
	g_I2C_Setting.u8I2CMasterBuffer[4] = 0x00;	//Auto_Play_Control_1_Reg
	g_I2C_Setting.u8I2CMasterBuffer[5] = 0x00;	//Auto_Play_Control_2_Reg
	g_I2C_Setting.u8I2CMasterBuffer[6] = 0x00;	//Reserved
	g_I2C_Setting.u8I2CMasterBuffer[7] = 0x00;	//Display_Option_Reg
	g_I2C_Setting.u8I2CMasterBuffer[8] = 0x00;	//Audio_Synchronization_Reg
	g_I2C_Setting.u8I2CMasterBuffer[9] = 0x00;	//Fram_State_Reg
	g_I2C_Setting.u8I2CMasterBuffer[10] = 0x00;	//Breath_Control_1_Reg
	g_I2C_Setting.u8I2CMasterBuffer[11] = 0x00;	//Breath_Control_2_Reg
	g_I2C_Setting.u8I2CMasterBuffer[12] = 0x00;	//Shutdown_Reg
	g_I2C_Setting.u8I2CMasterBuffer[13] = 0x00;	//AGC_Control_Reg
	g_I2C_Setting.u8I2CMasterBuffer[14] = 0x00;	//Audio_ADC_Rate_Reg

	#endif
	//SET_ALL_LED_OFF();
	//SET_ALL_LED_ON_WITH_PWM0();
	SET_ALL_LED_ON(0);
	//SET_ALL_LED_ON_R230();
	//SET_ONLY_RED_ON();
	//SET_ONLY_GREEN_ON();
	//SET_ONLY_BLUE_ON();
	//SET_ALL_LED_ON();
	//SET_SCROOL_LED_ON();	
}
//--------------------------------------------------------------------------
void AAAA(void)
{
	if(Cur_Led_Effect.Is_All_Effect==FALSE)
	{
		//Logo
		if(Cur_Led_Effect.Logo.Effect==0x0)
		{
			Led_Effect_Flag.Breath &= (~0x01);
			Led_Effect_Flag.Rainbow &= (~0x01);
			Led_Effect_Flag.Colorcycle &= (~0x01);
			Led_Region_Logo_Light_On();
		}
		else if(Cur_Led_Effect.Logo.Effect==0x01)
		{
			Led_Effect_Flag.enable=1;
			Led_Effect_Flag.Rainbow &= (~0x01);
			Led_Effect_Flag.Colorcycle&= (~0x01);
			Led_Effect_Flag.Breath |= 0x01;
		}
		else if(Cur_Led_Effect.Logo.Effect==0x02)
		{
			Led_Effect_Flag.enable=1;
			Led_Effect_Flag.Breath &= (~0x01);
			Led_Effect_Flag.Rainbow &= (~0x01);
			Led_Effect_Flag.Colorcycle |= 0x01;
		}

		//Wheel
		if(Cur_Led_Effect.Wheel.Effect==0x0)
		{
			Led_Effect_Flag.Breath &= (~0x02);
			Led_Effect_Flag.Rainbow &= (~0x02);
			Led_Effect_Flag.Colorcycle &= (~0x02);
			Led_Region_Wheel_Light_On();
		}
		else if(Cur_Led_Effect.Wheel.Effect==0x01)
		{
			Led_Effect_Flag.enable=1;
			Led_Effect_Flag.Rainbow &= (~0x02);
			Led_Effect_Flag.Colorcycle&= (~0x02);
			Led_Effect_Flag.Breath |= 0x02;
		}
		else if(Cur_Led_Effect.Wheel.Effect==0x02)
		{
			Led_Effect_Flag.enable=1;
			Led_Effect_Flag.Breath &= (~0x02);
			Led_Effect_Flag.Rainbow &= (~0x02);
			Led_Effect_Flag.Colorcycle |= 0x02;
		}
		
		//LightPipe
		if(Cur_Led_Effect.LightPipe.Effect==0x0)
		{
			Led_Effect_Flag.Breath &= (~0x04);
			Led_Effect_Flag.Rainbow &= (~0x04);
			Led_Effect_Flag.Colorcycle &= (~0x04);
			Led_Region_LightPipe_Light_On();
		}
		else if(Cur_Led_Effect.LightPipe.Effect==0x01)
		{
			Led_Effect_Flag.enable=1;
			Led_Effect_Flag.Rainbow &= (~0x04);
			Led_Effect_Flag.Colorcycle&= (~0x04);
			Led_Effect_Flag.Breath |= 0x04;
		}
		else if(Cur_Led_Effect.LightPipe.Effect==0x02)
		{
			Led_Effect_Flag.enable=1;
			Led_Effect_Flag.Breath &= (~0x04);
			Led_Effect_Flag.Rainbow &= (~0x04);
			Led_Effect_Flag.Colorcycle |= 0x04;
		}
		else if(Cur_Led_Effect.LightPipe.Effect==0x03)
		{
			Led_Effect_Flag.enable=1;
			LED_Color_Wave_counts=16;
			Led_Effect_Flag.Breath &= (~0x04);
			Led_Effect_Flag.Colorcycle&= (~0x04);
			Led_Effect_Flag.Rainbow |= 0x04;
		}
		
		
	}
	else
	{
		if(Cur_Led_Effect.All.Effect==0x0)
		{
			Led_Effect_Flag.Breath &= (~0x08);
			Led_Effect_Flag.Rainbow &= (~0x08);
			Led_Effect_Flag.Colorcycle &= (~0x08);
			Led_Region_Logo_Light_On();
			Led_Region_Wheel_Light_On();
			Led_Region_LightPipe_Light_On();
		}
		else if(Cur_Led_Effect.All.Effect==0x01)
		{
			Led_Effect_Flag.enable=1;
			Led_Effect_Flag.Rainbow &= (~0x08);
			Led_Effect_Flag.Colorcycle&= (~0x08);
			Led_Effect_Flag.Breath |= 0x08;
		}
		else if(Cur_Led_Effect.All.Effect==0x02)
		{
			Led_Effect_Flag.enable=1;
			Led_Effect_Flag.Breath &= (~0x08);
			Led_Effect_Flag.Rainbow &= (~0x08);
			Led_Effect_Flag.Colorcycle |= 0x08;
		}
		else if(Cur_Led_Effect.All.Effect==0x03)
		{
			Led_Effect_Flag.enable=1;
			LED_Color_Wave_counts=19;
			Led_Effect_Flag.Breath &= (~0x08);
			Led_Effect_Flag.Colorcycle&= (~0x08);
			Led_Effect_Flag.Rainbow |= 0x08;
		}
	}
}







//--------------------------------------------------------------------------
void LED_INIT(void)
{
	//software showdown
	//SDB_HIGH;
	ISSI3731_Software_showdown(TRUE);
	//hardware showdown
	SDB_LOW;
	
	//hardware showdown
	SDB_HIGH;

	//3731 LED Driver IC init Setting
	ISSI3731_Driver_Default_Setting();

	// normal operation
	ISSI3731_Software_showdown(FALSE);
	
	NDrv_GPIO_SetDir(PORT0, 21, GPIOOUT);	//SET SDB as outout
	NDrv_GPIO_SetBitValue(PORT0, 21, HIGH);
	
	LPC_IOCON->PIO0_22 &= ~0x07;			/*INTB*/
	LPC_IOCON->PIO0_22 |= BIT4;
	NDrv_GPIO_SetDir(PORT0, 22, GPIOIN);
//	Led_Logo_Color_Setting(255, 0, 0);
	/*
	HSV.s=255;
	HSV.v=175;
	HSV.h=HSV_H=0;
	*/
	//SET_ALL_LED_ON();
	LED_Frame_Setting(FRAM1);					//SET FRAME
	Led_Effect_Flag.Breath=0;
	Led_Effect_Flag.Colorcycle=0;
	Led_Effect_Flag.Rainbow=0;
	//LED_TEST();

}
//-----------------------------------------------------------------------------------
RgbColor m_HsvToRgb(HsvColor hsv)
{
	RgbColor rgb;
    	unsigned char region, remainder, p, q, t;
	//hsv.s=255;
	//hsv.v=175;
	//hsv.h=0,2,4,6,....254, 0 ;
 	if (hsv.s == 0)
    	{
        	rgb.r = hsv.v;
        	rgb.g = hsv.v;
        	rgb.b = hsv.v;
        	return rgb;
    	}
	/* make region 0-5 */
    	region = hsv.h / 43;
	/* find remainder part, make it from 0-255 */
    	remainder = (hsv.h - (region * 43)) * 6; 

    	p = (U16)(hsv.v * (255 - hsv.s)) >> 8;
    	q = (U16)(hsv.v * (255 - ((U16)(hsv.s * remainder) >> 8))) >> 8;
    	t = (U16)(hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    	switch (region)
    	{
        	case 0:
            		rgb.r = hsv.v;
			rgb.g = t;
			rgb.b = p;
            	break;
        	case 1:
            		rgb.r = q;
			rgb.g = hsv.v;
			rgb.b = p;
            	break;
       	case 2:
            		rgb.r = p;
			rgb.g = hsv.v;
			rgb.b = t;
            	break;
        	case 3:
            		rgb.r = p;
			rgb.g = q;
			rgb.b = hsv.v;
            	break;
        	case 4:
            		rgb.r = t;
			rgb.g = p;
			rgb.b = hsv.v;
            	break;
        	default:
            		rgb.r = hsv.v;
			rgb.g = p;
			rgb.b = q;
            	break;
    	}

    	return rgb;
}
//-----------------------------------------------------------------------------------
#if 0
void LED_COLOR_CHANGE(HsvColor hsv)
{
	RgbColor rgb;
	U8 i, j, step=48, R, G, B;
	
	rgb=m_HsvToRgb(hsv);	//get rgb value
	R=rgb.r;
	G=rgb.g;
	B=rgb.b;
	LED_Frame_Setting(FRAM1);		//SET FRAME	
	//set pwm to 255
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = step+2;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<96; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
		
		for(j=0; j<8; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = R;

		}
		for(j=0; j<8; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[10+j] = 0;

		}
		for(j=0; j<8; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[18+j] = G;

		}
		for(j=0; j<8; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[26+j] = 0;

		}
		for(j=0; j<8; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[34+j] = B;

		}
		for(j=0; j<8; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[42+j] = 0;

		}
		nsAPI_I2C_Engine( );
	}
	
	for(i=96; i<144; i+=step)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D80_R_PWM_REGISTER+i;
		
		for(j=0; j<4; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = R;

		}
		for(j=0; j<12; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[6+j] = 0;

		}
		for(j=0; j<4; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[18+j] = G;

		}
		for(j=0; j<12; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[22+j] = 0;

		}
		for(j=0; j<4; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[34+j] = B;

		}
		for(j=0; j<12; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[38+j] = 0;

		}
		nsAPI_I2C_Engine( );
	}
}
#endif
//-----------------------------------------------------------------------------------
void LED_BREATHING_FRAM_SETTING(U8 fram)
{
	//nsAPI_Timer16_B0M3_PWM_Init(fram);
	nsAPI_Timer32_B0M3_PWM_Init(fram);
}
//-----------------------------------------------------------------------------------
void LED_BREATHING_CONTROLE_ENABLE(bool enable)
{
	PWM_TIMER_ENABLE(enable);
}
//-----------------------------------------------------------------------------------
void LED_COLOR_CHANGE_ENABLE(bool enable)
{
	if(enable)
	{
		LED_COLOR_CHANGE_FLAG=TRUE;
	}
	else
	{
		LED_COLOR_CHANGE_FLAG=FALSE;
	}
}
//-----------------------------------------------------------------------------------
//static U8 Logo_Index[]=
//{
//	0x85, 0x95, 0xA5
//};
//static U8 LightPipe_Table_Index[]=
//{
//	0x24, 0x34, 0x44, 0x54, 0x64, 0x74
//};


//-----------------------------------------------------------------------------------
#if 0
void LightPipe_PWM_Setting(U8 R, U8 G, U8 B)
{
	U8 i, j;
	U8 length=8;
	U8 index=0;
	LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = length+2;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<2; i++)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_Table_Index[index++];
		for(j=0; j<length; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = R;

		}
		nsAPI_I2C_Engine( );

		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_Table_Index[index++];
		for(j=0; j<length; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = G;

		}
		nsAPI_I2C_Engine( );

		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_Table_Index[index++];
		for(j=0; j<length; j++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[2+j] = B;

		}
		nsAPI_I2C_Engine( );
		
	}
	
}
#endif
//-----------------------------------------------------------------------------------
#if 0
void Logo_PWM_Setting(U8 R, U8 G, U8 B)
{
	U8 j;
	//U8 length=2;
	U8 index=0;
	//LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	//for(i=0; i<2; i++)
	if(R)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_R_PWM_REGISTER;
		//for(j=0; j<length; j++)
		//{
		//	g_I2C_Setting.u8I2CMasterBuffer[2+j] = R;
		//}
		//nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CMasterBuffer[2] = R;
	}
	else if(G)
	{
	
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_G_PWM_REGISTER;
		//for(j=0; j<length; j++)
		//{
		//	g_I2C_Setting.u8I2CMasterBuffer[2+j] = G;
		//}
		g_I2C_Setting.u8I2CMasterBuffer[2] = G;
		//nsAPI_I2C_Engine( );
	}
	else if(B)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_B_PWM_REGISTER;
		//for(j=0; j<length; j++)
		//{
		//	g_I2C_Setting.u8I2CMasterBuffer[2+j] = B;
		//}
		g_I2C_Setting.u8I2CMasterBuffer[2] = B;
	}
	nsAPI_I2C_Engine( );
	
}
#else
//-----------------------------------------------------------------------------------
void Logo_PWM_Setting(U8 color, U8 color_index)
{
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	if(color_index==0)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_R_PWM_REGISTER; 
	}
	else if(color_index==1)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_G_PWM_REGISTER;
	}
	else if(color_index==2)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_B_PWM_REGISTER;
	}
	g_I2C_Setting.u8I2CMasterBuffer[2] = color;
	nsAPI_I2C_Engine( );
	
}
#endif
//-----------------------------------------------------------------------------------
void Wheel_PWM_Setting(U8 color, U8 color_index)
{
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 4;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	if(color_index==0)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D2_R_PWM_REGISTER;
	}
	else if(color_index==1)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D2_G_PWM_REGISTER;
	}
	else if(color_index==2)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D2_B_PWM_REGISTER;
	}
	g_I2C_Setting.u8I2CMasterBuffer[2] = color;
	g_I2C_Setting.u8I2CMasterBuffer[3] = color;
	nsAPI_I2C_Engine( );
	
}
//-----------------------------------------------------------------------------------
void LightPipe_PWM_Setting_1(U8 color, U8 color_index)
{
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	if(color_index==0)
	{
		g_I2C_Setting.u8I2CWriteLength = 10;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL1_R_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		g_I2C_Setting.u8I2CMasterBuffer[3] = color;
		g_I2C_Setting.u8I2CMasterBuffer[4] = color;
		g_I2C_Setting.u8I2CMasterBuffer[5] = color;
		g_I2C_Setting.u8I2CMasterBuffer[6] = color;
		g_I2C_Setting.u8I2CMasterBuffer[7] = color;
		g_I2C_Setting.u8I2CMasterBuffer[8] = color;
		g_I2C_Setting.u8I2CMasterBuffer[9] = color;
	}
	else if(color_index==1)
	{
		g_I2C_Setting.u8I2CWriteLength = 9;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL2_G_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		g_I2C_Setting.u8I2CMasterBuffer[3] = color;
		g_I2C_Setting.u8I2CMasterBuffer[4] = color;
		g_I2C_Setting.u8I2CMasterBuffer[5] = color;
		g_I2C_Setting.u8I2CMasterBuffer[6] = color;
		g_I2C_Setting.u8I2CMasterBuffer[7] = color;
		g_I2C_Setting.u8I2CMasterBuffer[8] = color;
		nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CWriteLength = 3;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL1_G_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		
	}
	else if(color_index==2)
	{
		g_I2C_Setting.u8I2CWriteLength = 8;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL3_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		g_I2C_Setting.u8I2CMasterBuffer[3] = color;
		g_I2C_Setting.u8I2CMasterBuffer[4] = color;
		g_I2C_Setting.u8I2CMasterBuffer[5] = color;
		g_I2C_Setting.u8I2CMasterBuffer[6] = color;
		g_I2C_Setting.u8I2CMasterBuffer[7] = color;
		nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CWriteLength = 4;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL1_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		g_I2C_Setting.u8I2CMasterBuffer[3] = color;
	}

	nsAPI_I2C_Engine( );
	
}
//-----------------------------------------------------------------------------------
void LightPipe_PWM_Setting_2(U8 color, U8 color_index)
{
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	if(color_index==0)
	{
		g_I2C_Setting.u8I2CWriteLength = 10;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL9_R_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		g_I2C_Setting.u8I2CMasterBuffer[3] = color;
		g_I2C_Setting.u8I2CMasterBuffer[4] = color;
		g_I2C_Setting.u8I2CMasterBuffer[5] = color;
		g_I2C_Setting.u8I2CMasterBuffer[6] = color;
		g_I2C_Setting.u8I2CMasterBuffer[7] = color;
		g_I2C_Setting.u8I2CMasterBuffer[8] = color;
		g_I2C_Setting.u8I2CMasterBuffer[9] = color;
	}
	else if(color_index==1)
	{
		g_I2C_Setting.u8I2CWriteLength = 9;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL9_G_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		g_I2C_Setting.u8I2CMasterBuffer[3] = color;
		g_I2C_Setting.u8I2CMasterBuffer[4] = color;
		g_I2C_Setting.u8I2CMasterBuffer[5] = color;
		g_I2C_Setting.u8I2CMasterBuffer[6] = color;
		g_I2C_Setting.u8I2CMasterBuffer[7] = color;
		g_I2C_Setting.u8I2CMasterBuffer[8] = color;
		nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CWriteLength = 3;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL16_G_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		
	}
	else if(color_index==2)
	{
		g_I2C_Setting.u8I2CWriteLength = 8;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL9_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		g_I2C_Setting.u8I2CMasterBuffer[3] = color;
		g_I2C_Setting.u8I2CMasterBuffer[4] = color;
		g_I2C_Setting.u8I2CMasterBuffer[5] = color;
		g_I2C_Setting.u8I2CMasterBuffer[6] = color;
		g_I2C_Setting.u8I2CMasterBuffer[7] = color;
		nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CWriteLength = 4;
		g_I2C_Setting.u8I2CMasterBuffer[1] = FL15_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = color;
		g_I2C_Setting.u8I2CMasterBuffer[3] = color;
	}

	nsAPI_I2C_Engine( );
	
}
//-----------------------------------------------------------------------------------
// EX:	R: 0xE5 * 136 = 0x79A8, Hi byte 0x79 is red duty
//		B: 0x00 * 136 = 0, Blue duty=0
//		G: 0x1A * 136 = 0x0DD0, Hi byte 0x0D is green duty
//-----------------------------------------------------------------------------------

//void RGB_Convent2_Brightness(U8 R, U8 G, U8 B, U16 ratio )			
void RGB_Convent2_Brightness(U8 *R, U8 *G, U8 *B , U8 brightness)			
{
	U16 R_temp=(U16)(*R);
	U16 G_temp=(U16)(*G);
	U16 B_temp=(U16)(*B);
	R_temp=((U16)brightness*R_temp)>>8;
	G_temp=((U16)brightness*G_temp)>>8;
	B_temp=((U16)brightness*B_temp)>>8;
	*R=(U8)R_temp;
	*G=(U8)G_temp;
	*B=(U8)B_temp;
	//LightPipe_PWM_Setting(R_temp, G_temp, B_temp);
	//if(LED_Breath_Effect&0x01)	//all led region breath
	//{
	//}else{
	//}
	//Logo_PWM_Setting(R_temp, B_temp, G_temp);
}
//-----------------------------------------------------------------------------------

//void RGB_Convent2_Brightness(U8 R, U8 G, U8 B, U16 ratio )			
void RGB_Convent2_Brightness_Sigle_Color(U8 *color, U8 brightness)			
{
	U16 color_temp=(U16)(*color);
	color_temp=((U16)brightness*color_temp)>>8;
	*color=(U8)color_temp;
	//LightPipe_PWM_Setting(R_temp, G_temp, B_temp);
	//if(LED_Breath_Effect&0x01)	//all led region breath
	//{
	//}else{
	//}
	//Logo_PWM_Setting(R_temp, B_temp, G_temp);
}
//-----------------------------------------------------------------------------------
// EX:	R: 0xE5 * 136 = 0x79A8, Hi byte 0x79 is red duty
//		B: 0x00 * 136 = 0, Blue duty=0
//		G: 0x1A * 136 = 0x0DD0, Hi byte 0x0D is green duty
//-----------------------------------------------------------------------------------

//void RGB_Convent2_Brightness_By_Region(U8 R, U8 G, U8 B, U8 region )			
void RGB_Convent2_Brightness_By_Region(U8 color, U8 color_index, U8 region )	
{
	if(region<2)
		Logo_PWM_Setting(color,  color_index);
	else if(region<3)
		Wheel_PWM_Setting(color,  color_index);
	else if(region<4)
		LightPipe_PWM_Setting_1(color,  color_index);
	else if(region<5)
		LightPipe_PWM_Setting_2(color,  color_index);
}
//-----------------------------------------------------------------------------------
#if 0
void LED_Color_Wave(U8 ranbow_index)			
{
	U8 i;
	//U8 length=8;
	U8 index;
	LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	if((LED_Rainbow_Effect&0x1) || (LED_Rainbow_Effect&0x8))						//light pipe
	{
		index=ranbow_index;
		for(i=0; i<16; i++)
		{
			//R
			g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_Table_Index[i];
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index][0];
			nsAPI_I2C_Engine( );
			//G
			g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_Table_Index[i]+0x10;
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index][2];
			nsAPI_I2C_Engine( );
			//B
			g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_Table_Index[i]+0x20;
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index++][1];
			nsAPI_I2C_Engine( );	
			if(index==12)
				index=0;
		}
	}
	if((LED_Rainbow_Effect&0x1) || (LED_Rainbow_Effect&0x2))						//logo
	{
		//R
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_R_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][0];
		nsAPI_I2C_Engine( );
		//G
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][2];
		nsAPI_I2C_Engine( );
		//B
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_G_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][1];
		nsAPI_I2C_Engine( );	
	}
	if((LED_Rainbow_Effect&0x1) || (LED_Rainbow_Effect&0x4))						//wheel
	{
		index=ranbow_index;
		for(i=0; i<2; i++)
		{
			//R
			g_I2C_Setting.u8I2CMasterBuffer[1] = Wheel_Table_Index[i];
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index][0];
			nsAPI_I2C_Engine( );
			//G
			g_I2C_Setting.u8I2CMasterBuffer[1] = Wheel_Table_Index[i]+0x10;
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index][2];
			nsAPI_I2C_Engine( );
			//B
			g_I2C_Setting.u8I2CMasterBuffer[1] = Wheel_Table_Index[i]+0x20;
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index++][1];
			nsAPI_I2C_Engine( );	
		}
	}
	
}
//-----------------------------------------------------------------------------------
void LED_Logo_Color_Wave(U8 ranbow_index, U8 update_idx)
{
	//U8 i;
	//LED_Frame_Setting(FRAM1);					//SET FRAME	
	//g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	//if((LED_Rainbow_Effect&0x1) || (LED_Rainbow_Effect&0x2))						//logo
	switch(update_idx)
	{
		case 0:
			//R
			g_I2C_Setting.u8I2CMasterBuffer[1] = D107_R_PWM_REGISTER;
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][0];
			nsAPI_I2C_Engine( );
		break;
		case 1:
			//G
			g_I2C_Setting.u8I2CMasterBuffer[1] = D107_B_PWM_REGISTER;
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][2];
			nsAPI_I2C_Engine( );
		break;
		case 2:
			//B
			g_I2C_Setting.u8I2CMasterBuffer[1] = D107_G_PWM_REGISTER;
			g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][1];
			nsAPI_I2C_Engine( );	
		break;
	}
}
#endif
//-----------------------------------------------------------------------------------
void LED_LightPipe_Color_Wave(U8 ranbow_index, U8 update_idx)			
{
	//LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	//R
	//g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_R_Table_Index[update_idx];
	g_I2C_Setting.u8I2CMasterBuffer[1] = ALL_R_Table_Index[update_idx];
	g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][0];
	nsAPI_I2C_Engine( );
	//G
	//g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_G_Table_Index[update_idx];
	g_I2C_Setting.u8I2CMasterBuffer[1] = ALL_G_Table_Index[update_idx];
	g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][2];
	nsAPI_I2C_Engine( );
	//B
	//g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_B_Table_Index[update_idx];
	g_I2C_Setting.u8I2CMasterBuffer[1] = ALL_B_Table_Index[update_idx];
	g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][1];
	nsAPI_I2C_Engine( );	
}
//-----------------------------------------------------------------------------------
void Logo_Color_Wave_Setting(U8 ranbow_index, U8 color_index)
{
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	if(color_index==0)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_R_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][0];
	}
	else if(color_index==1)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_G_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][2];
	}
	else if(color_index==2)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D107_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][1];
	}

	nsAPI_I2C_Engine( );
	
}
//-----------------------------------------------------------------------------------
void Wheel_Color_Wave_Setting(U8 ranbow_index, U8 color_index)
{
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 4;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	if(color_index==0)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D2_R_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][0];
		g_I2C_Setting.u8I2CMasterBuffer[3] = m_rainbow[ranbow_index][0];
	}
	else if(color_index==1)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D2_G_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][2];
		g_I2C_Setting.u8I2CMasterBuffer[3] = m_rainbow[ranbow_index][2];
	}
	else if(color_index==2)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = D2_B_PWM_REGISTER;
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[ranbow_index][1];
		g_I2C_Setting.u8I2CMasterBuffer[3] = m_rainbow[ranbow_index][1];
	}
	nsAPI_I2C_Engine( );
	
}

//-----------------------------------------------------------------------------------
void LED_Color_Wave_By_Region(U8 ranbow_index, U8 color_index, U8 region )
{
	if(region<2)
		Logo_Color_Wave_Setting(ranbow_index,  color_index);
	else if(region<3)
		Wheel_Color_Wave_Setting(ranbow_index,  color_index);
}
//-----------------------------------------------------------------------------------

void LightPipe_Rainbow(U8 ranbow_index)			
{
	U8 i;//, j;
	//U8 length=8;
	U8 index=ranbow_index;
	LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<16; i++)
	{
		//R
		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_R_Table_Index[i];
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index][0];
		nsAPI_I2C_Engine( );
		//G
		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_G_Table_Index[i];
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index][2];
		nsAPI_I2C_Engine( );
		//B
		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_B_Table_Index[i];
		g_I2C_Setting.u8I2CMasterBuffer[2] = m_rainbow[index][1];
		nsAPI_I2C_Engine( );		
		index++;
		if(index==12)
			index=0;
		
	}
}
//-----------------------------------------------------------------------------------
void Led_Logo_Color_Setting(U8 R, U8 G, U8 B)
{
	LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	g_I2C_Setting.u8I2CMasterBuffer[1] = D107_R_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = R;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CMasterBuffer[1] = D107_G_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = G;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CMasterBuffer[1] = D107_B_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = B;
	nsAPI_I2C_Engine( );
}
//-----------------------------------------------------------------------------------
void Dfu_Led_Effect(void)
{
	SET_ALL_LED_ON(0);
	LED_Frame_Setting(FRAM1);					//SET FRAME
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 4;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	

	g_I2C_Setting.u8I2CMasterBuffer[1] = D2_R_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 255;
	g_I2C_Setting.u8I2CMasterBuffer[3] = 255;
	nsAPI_I2C_Engine( );
	
	g_I2C_Setting.u8I2CMasterBuffer[1] = D2_G_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0;
	g_I2C_Setting.u8I2CMasterBuffer[3] = 0;
	nsAPI_I2C_Engine( );

	g_I2C_Setting.u8I2CMasterBuffer[1] = D2_B_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0;
	g_I2C_Setting.u8I2CMasterBuffer[3] = 0;
	nsAPI_I2C_Engine( );

	LED_Frame_Setting(FRAM1);			//SET FRAME
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	

	g_I2C_Setting.u8I2CMasterBuffer[1] = 0x13;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0x60;
	nsAPI_I2C_Engine( );

	g_I2C_Setting.u8I2CMasterBuffer[1] = 0x15;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0x60;
	nsAPI_I2C_Engine( );

	g_I2C_Setting.u8I2CMasterBuffer[1] = 0x17;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0x60;
	nsAPI_I2C_Engine( );

	LED_Frame_Setting(FUNCTION_FRAME);			//SET FRAME
	g_I2C_Setting.u8I2CReadLength = 0;
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	g_I2C_Setting.u8I2CMasterBuffer[1] = 0x05;
	g_I2C_Setting.u8I2CMasterBuffer[2] = 0x0D;
	nsAPI_I2C_Engine( );
}
//extern struct Led_Effect_t;

//-----------------------------------------------------------------------------------
void Led_Region_Logo_Light_On(void)
{
	U8 r=Cur_Led_Effect.Logo.color[0];
	U8 g=Cur_Led_Effect.Logo.color[2];
	U8 b=Cur_Led_Effect.Logo.color[1];
	RGB_Convent2_Brightness(&r, &g, &b, Cur_Led_Effect.Logo.Brigntness); 

	LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	g_I2C_Setting.u8I2CMasterBuffer[1] = D107_R_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = r;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CMasterBuffer[1] = D107_G_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = g;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CMasterBuffer[1] = D107_B_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = b;
	nsAPI_I2C_Engine( );
}
//-----------------------------------------------------------------------------------
void Led_Region_Wheel_Light_On(void)
{
	U8 r=Cur_Led_Effect.Wheel.color[0];
	U8 g=Cur_Led_Effect.Wheel.color[2];
	U8 b=Cur_Led_Effect.Wheel.color[1];
	RGB_Convent2_Brightness(&r, &g, &b, Cur_Led_Effect.Wheel.Brigntness); 
	LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CWriteLength = 4;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	g_I2C_Setting.u8I2CMasterBuffer[1] = D2_R_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = r;
	g_I2C_Setting.u8I2CMasterBuffer[3] = r;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CMasterBuffer[1] = D2_G_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = g;
	g_I2C_Setting.u8I2CMasterBuffer[3] = g;
	nsAPI_I2C_Engine( );
	g_I2C_Setting.u8I2CMasterBuffer[1] = D2_B_PWM_REGISTER;
	g_I2C_Setting.u8I2CMasterBuffer[2] = b;
	g_I2C_Setting.u8I2CMasterBuffer[3] = b;
	nsAPI_I2C_Engine( );
}
//-----------------------------------------------------------------------------------
void Led_Region_LightPipe_Light_On(void)
{
	U8 i;
	U8 r=Cur_Led_Effect.LightPipe.color[0];
	U8 g=Cur_Led_Effect.LightPipe.color[2];
	U8 b=Cur_Led_Effect.LightPipe.color[1];
	RGB_Convent2_Brightness(&r, &g, &b, Cur_Led_Effect.LightPipe.Brigntness); 
	LED_Frame_Setting(FRAM1);					//SET FRAME	
	g_I2C_Setting.u8I2CWriteLength = 3;
	g_I2C_Setting.u8I2CMasterBuffer[0] = I2C_LED_DRIVER_ADDR;	
	for(i=0; i<16; i++)
	{
		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_R_Table_Index[i];
		g_I2C_Setting.u8I2CMasterBuffer[2] = r;
		nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_G_Table_Index[i];
		g_I2C_Setting.u8I2CMasterBuffer[2] = g;
		nsAPI_I2C_Engine( );
		g_I2C_Setting.u8I2CMasterBuffer[1] = LightPipe_B_Table_Index[i];
		g_I2C_Setting.u8I2CMasterBuffer[2] = b;
		nsAPI_I2C_Engine( );
	}
}

//-----------------------------------------------------------------------------------
void Led_Effect_Process(U16 region, U8* data)
{
	#if 1
	U8 effect=*data;
	if(effect==0x00)	//Always
	{
		if(region==0x0000)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.Logo.Effect=0;
			Cur_Led_Effect.Logo.Brigntness=data[1];
			Cur_Led_Effect.Logo.color[0]=data[2];
			Cur_Led_Effect.Logo.color[1]=data[3];
			Cur_Led_Effect.Logo.color[2]=data[4];
			Led_Effect_Flag.Breath &= (~0x01);
			Led_Effect_Flag.Rainbow &= (~0x01);
			Led_Effect_Flag.Colorcycle &= (~0x01);
			Led_Region_Logo_Light_On();
		}
		else if(region==0x0001)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.Wheel.Effect=0;
			Cur_Led_Effect.Wheel.Brigntness=data[1];
			Cur_Led_Effect.Wheel.color[0]=data[2];
			Cur_Led_Effect.Wheel.color[1]=data[3];
			Cur_Led_Effect.Wheel.color[2]=data[4];
			Led_Effect_Flag.Breath &= (~0x02);
			Led_Effect_Flag.Rainbow &= (~0x02);
			Led_Effect_Flag.Colorcycle &= (~0x02);
			Led_Region_Wheel_Light_On();
		}
		else if(region==0x0002)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.LightPipe.Effect=0;
			Cur_Led_Effect.LightPipe.Brigntness=data[1];
			Cur_Led_Effect.LightPipe.color[0]=data[2];
			Cur_Led_Effect.LightPipe.color[1]=data[3];
			Cur_Led_Effect.LightPipe.color[2]=data[4];
			Led_Effect_Flag.Breath &= (~0x04);
			Led_Effect_Flag.Rainbow &= (~0x04);
			Led_Effect_Flag.Colorcycle &= (~0x04);
			Led_Region_LightPipe_Light_On();
			//LED_Lightpipe_Color_Cycle_Flag=LED_Lightpipe_Color_Wave_Flag=FALSE;
			//LED_Lightpipe_Color_Cycle_Flag=FALSE;
		}
		else if(region==0x0003)
		{
			Cur_Led_Effect.Is_All_Effect=1;
			Cur_Led_Effect.All.Effect=0;
			Cur_Led_Effect.All.Brigntness=data[1];
			Cur_Led_Effect.All.color[0]=data[2];
			Cur_Led_Effect.All.color[1]=data[3];
			Cur_Led_Effect.All.color[2]=data[4];
			Led_Effect_Flag.Breath =0;
			Led_Effect_Flag.Rainbow =0;
			Led_Effect_Flag.Colorcycle =0;
			//Led_Effect_Flag.enable &= (~0x08);
			Led_Region_Logo_Light_On();
			Led_Region_Wheel_Light_On();
			Led_Region_LightPipe_Light_On();
			//LED_Lightpipe_Color_Cycle_Flag = LED_Lightpipe_Color_Wave_Flag=FALSE;
			//LED_Lightpipe_Color_Cycle_Flag =FALSE;
		}
		//if(Led_Effect_Flag.enable & (~0x0F) == 0)
		//	Led_Effect_Flag.Breath=0;
		//if(Led_Effect_Flag.Rainbow & (~0xF0) == 0)
		//	Led_Effect_Flag.Rainbow=0;
	}
	else if(effect==0x01)	//breath
	{
		Led_Effect_Flag.enable=1;
		if(region==0x0000)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.Logo.Effect=1;
			Cur_Led_Effect.Logo.Brigntness=data[1];
			Cur_Led_Effect.Logo.color[0]=data[2];
			Cur_Led_Effect.Logo.color[1]=data[3];
			Cur_Led_Effect.Logo.color[2]=data[4];
			if(data[59]>20)
				Cur_Led_Effect.Times.breath=data[59];
			//Led_Effect_Flag.enable &= (~0x10);
			//Led_Effect_Flag.enable |= 0x01;
			Led_Effect_Flag.Rainbow &= (~0x01);
			Led_Effect_Flag.Colorcycle&= (~0x01);
			Led_Effect_Flag.Breath |= 0x01;
		}
		else if(region==0x0001)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.Wheel.Effect=1;
			Cur_Led_Effect.Wheel.Brigntness=data[1];
			Cur_Led_Effect.Wheel.color[0]=data[2];
			Cur_Led_Effect.Wheel.color[1]=data[3];
			Cur_Led_Effect.Wheel.color[2]=data[4];
			if(data[59]>20)
				Cur_Led_Effect.Times.breath=data[59];
			Led_Effect_Flag.Rainbow &= (~0x02);
			Led_Effect_Flag.Colorcycle&= (~0x02);
			Led_Effect_Flag.Breath |= 0x02;
		}
		else if(region==0x0002)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.LightPipe.Effect=1;
			Cur_Led_Effect.LightPipe.Brigntness=data[1];
			Cur_Led_Effect.LightPipe.color[0]=data[2];
			Cur_Led_Effect.LightPipe.color[1]=data[3];
			Cur_Led_Effect.LightPipe.color[2]=data[4];
			if(data[59]>20)
				Cur_Led_Effect.Times.breath=data[59];
			Led_Effect_Flag.Rainbow &= (~0x04);
			Led_Effect_Flag.Colorcycle&= (~0x04);
			Led_Effect_Flag.Breath |= 0x04;
			//LED_Lightpipe_Color_Cycle_Flag =0;
		}
		else if(region==0x0003)
		{
			Cur_Led_Effect.Is_All_Effect=1;
			Cur_Led_Effect.All.Effect=1;
			Cur_Led_Effect.All.Brigntness=data[1];
			Cur_Led_Effect.All.color[0]=data[2];
			Cur_Led_Effect.All.color[1]=data[3];
			Cur_Led_Effect.All.color[2]=data[4];
			if(data[59]>20)

			Cur_Led_Effect.Times.breath=data[59];
			Led_Effect_Flag.Rainbow =0;
			Led_Effect_Flag.Colorcycle =0;
			Led_Effect_Flag.Breath |= 0x08;
			//LED_Lightpipe_Color_Cycle_Flag =0;
		}
	}
	else if(effect==0x02)	//color cycle
	{
		Led_Effect_Flag.enable=1;
		if(region==0x0000)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.Logo.Effect=2;
			if(data[59]>20)
				Cur_Led_Effect.Times.colorcycle=data[59];
			Led_Effect_Flag.Breath &= (~0x01);
			Led_Effect_Flag.Rainbow &= (~0x01);
			Led_Effect_Flag.Colorcycle |= 0x01;
			
		}
		else if(region==0x0001)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.Wheel.Effect=2;
			if(data[59]>20)
				Cur_Led_Effect.Times.colorcycle=data[59];
			Led_Effect_Flag.Breath &= (~0x02);
			Led_Effect_Flag.Rainbow &= (~0x02);
			Led_Effect_Flag.Colorcycle |= 0x02;
		}
		else if(region==0x0002)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.LightPipe.Effect=2;
			if(data[59]>20)
				Cur_Led_Effect.Times.colorcycle=data[59];
			Led_Effect_Flag.Breath &= (~0x04);
			Led_Effect_Flag.Rainbow &= (~0x04);
			Led_Effect_Flag.Colorcycle |= 0x04;
			//LED_Lightpipe_Color_Wave_Flag=0;
			//LED_Lightpipe_Color_Cycle_Flag=1;
		}
		else if(region==0x0003)
		{
			Cur_Led_Effect.Is_All_Effect=1;
			Cur_Led_Effect.All.Effect=2;
			if(data[59]>20)
				Cur_Led_Effect.Times.colorcycle=data[59];
			Led_Effect_Flag.Breath = 0;
			Led_Effect_Flag.Rainbow =0;
			Led_Effect_Flag.Colorcycle |= 0x08;
			//LED_Lightpipe_Color_Wave_Flag=0;
			//LED_Lightpipe_Color_Cycle_Flag=1;
		}
		
	}
	else if(effect==0x03)	//color wave
	{
		Led_Effect_Flag.enable=1;
		//Led_Effect_Flag.Rainbow=1;
		if(region==0x0002)
		{
			LED_Color_Wave_counts=16;
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.LightPipe.Effect=3;
			if(data[59]>20)								//color change timming
				Cur_Led_Effect.Times.colorwave=data[59];
			Led_Effect_Flag.Breath &= (~0x04);
			Led_Effect_Flag.Colorcycle&= (~0x04);
			Led_Effect_Flag.Rainbow |= 0x04;
			//LED_Lightpipe_Color_Wave_Flag=1;
			//LED_Lightpipe_Color_Cycle_Flag=0;
		}
		else if(region==0x0003)
		{
			LED_Color_Wave_counts=19;
			Cur_Led_Effect.Is_All_Effect=1;
			Cur_Led_Effect.All.Effect=3;
			if(data[59]>20)
				Cur_Led_Effect.Times.colorwave=data[59];	//color change timming
			Led_Effect_Flag.Breath = 0;
			Led_Effect_Flag.Colorcycle =0;
			Led_Effect_Flag.Rainbow |= 0x08;
			//LED_Lightpipe_Color_Wave_Flag=1;
			//LED_Lightpipe_Color_Cycle_Flag=0;
		}
	}
	else if(effect==0x04)	//
	{
		
	}
	else if(effect==0xff)
	{
		//Led_Effect_Flag.enable=0;
		if(region==0x0000)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.Logo.Effect=0;
			Led_Effect_Flag.Breath &= (~0x11);
			Led_Effect_Flag.Rainbow &= (~0x11);
		}
		else if(region==0x0001)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.Wheel.Effect=0;
			Led_Effect_Flag.Breath &= (~0x22);
			Led_Effect_Flag.Rainbow &= (~0x22);
		}
		else if(region==0x0002)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.LightPipe.Effect=0;
			Led_Effect_Flag.Breath &= (~0x44);
			Led_Effect_Flag.Rainbow &= (~0x44);
		}
		else if(region==0x0003)
		{
			Cur_Led_Effect.Is_All_Effect=0;
			Cur_Led_Effect.All.Effect=0;
			Led_Effect_Flag.Breath=0;
			Led_Effect_Flag.Rainbow=0;
			Led_Effect_Flag.enable=0;
		}
		
	}
		#endif
	
		
}

