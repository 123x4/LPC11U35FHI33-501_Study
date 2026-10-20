
#include <stdbool.h>

typedef struct RgbColor
{
    U8 r;
    U8 g;
    U8 b;
} RgbColor;

typedef struct
{
    U8 h;
    U8 s;
    U8 v;
} HsvColor;

struct Led_Effect_Flag_t
{
	bool	enable;
	U8	Breath;
	U8	Rainbow;
	U8	Colorcycle;
};

//------------------------------------------------------------------------------------------

extern U8 LED_COLOR_TIMES;
//extern bool LED_COLOR_FLAG;
//extern bool LED_COLOR_FLAG;
//extern bool LED_EFFECT_FLAG;
extern HsvColor HSV;
extern U8 HSV_H;
extern const U8 m_rainbow[12][3];
//extern bool LED_EFFECT_FLAG;
//extern U8 LED_Breath_Effect;	       //bit 0 : ALL, bit 1: logo, bit 2: wheel, bit 3: light pipe
//extern U8 LED_Rainbow_Effect;	//bit 0 : ALL, bit 1: logo, bit 2: wheel, bit 3: light pipe
extern U8 LED_Lightpipe_Breath_Flag;
extern U8 LED_Lightpipe_Color_Cycle_Flag;
extern U8 LED_Logo_Color_index;		//0=red, 1=green, 2=blue;
extern U8 LED_Color_Wave_Color_index;
extern U8 LED_Color_Cycle_Color_index;
extern bool LED_Lightpipe_Color_Wave_Flag;
//extern struct Led_Effect_Flag_t;
//extern struct Led_Effect_t Led_Effect;

extern void LED_INIT(void);
extern void LED_COLOR_CHANGE(HsvColor hsv);
extern void LED_BREATHING_FRAM_SETTING(U8 fram);
extern void LED_BREATHING_CONTROLE_ENABLE(bool enable);
extern void LED_COLOR_CHANGE_ENABLE(bool enable);
//extern void RGB_Convent2_Brightness(U8 R, U8 G, U8 B, U16 ratio);
extern void RGB_Convent2_Brightness(U8 *R, U8 *G, U8 *B , U8 brightness);
extern void RGB_Convent2_Brightness_Sigle_Color(U8 *color, U8 brightness)	;
extern void LED_Color_Wave(U8 ranbow_index)	;
extern void LED_LightPipe_Color_Wave(U8 ranbow_index, U8 update_idx);		
extern void Led_Logo_Color_Setting(U8 R, U8 G, U8 B);
extern void RGB_Convent2_Brightness_By_Region(U8 color, U8 color_index, U8 region );
extern void LED_Color_Wave_By_Region(U8 ranbow_index, U8 color_index, U8 region );
extern void Dfu_Led_Effect(void);
extern void Led_Effect_Process(U16 region, U8* data);
extern void Led_Region_Logo_Light_On(void);
extern void Led_Region_Wheel_Light_On(void);
extern void Led_Region_LightPipe_Light_On(void);

#define LED_AUDIOIN_ON() 		NDrv_GPIO_SetBitValue(PORT1, 25, LOW);
#define LED_AUDIOIN_OFF() 	NDrv_GPIO_SetBitValue(PORT1, 25, HIGH);

static U8 LED_CTRL_REGISTER_OFF_SETTING[]=
{
	0x00, 0x00,	//CA1 CB1 
	0x00, 0x00, 	//CA2 CB2
	0x00, 0x00, 	//CA3 CB3
	0x00, 0x00,	//CA4 CB4
	0x00, 0x00, 	//CA5 CB5
	0x00, 0x00,	//CA6 CB6
	0x00, 0x00, 	//CA7 CB7
	0x00, 0x00,	//CA8 CB8
	0x00, 0x00 	//CA9 CB9
};

#if 0
static U8 LED_CTRL_REGISTER_ON_SETTING[]=
{
	0xEF, 0x00,	//CA1 CB1 
	0xEF, 0x00, 	//CA2 CB2
	0xEF, 0x00, 	//CA3 CB3
	0xFF, 0x00,	//CA4 CB4
	0xFF, 0x00, 	//CA5 CB5
	0xFF, 0x00,	//CA6 CB6
	0x0E, 0x00, 	//CA7 CB7
	0x0E, 0x00,	//CA8 CB8
	0x0E, 0x00 	//CA9 CB9
};
#else
//0624 Schematic
static U8 LED_CTRL_REGISTER_ON_SETTING[]=
{
	0xFF, 0xE0,	//CA1 CB1 
	0xFE, 0xE0, 	//CA2 CB2
	0xFE, 0xE0, 	//CA3 CB3
	0x06, 0x00,	//CA4 CB4
	0x00, 0x00, 	//CA5 CB5
	0x60, 0x00,	//CA6 CB6
	0x7F, 0x00, 	//CA7 CB7
	0x7F, 0x00,	//CA8 CB8
	0xFF, 0x00 	//CA9 CB9
};
#endif
static U8 LED_PWM_TABLE[]=
{
	0, 1, 2, 4, 6, 10, 13, 18,
	22, 28, 33, 39, 46, 53, 61, 69,
	78, 86, 96, 106, 116, 126, 138, 149, 
	161, 173, 186, 199, 212, 226, 240, 255
};

static U8 LED_PWM_TABLE_1[]=
{
	0, 1, 2, 4, 6, 10, 13, 18, 69, 86, 106, 126, 149,173,199, 226
};

#define FIRST_COLOR_PWM_REGISTER 0x24

//Light Pipe
#define FL1_R_PWM_REGISTER 0x24
#define FL1_G_PWM_REGISTER 0x45
#define FL1_B_PWM_REGISTER 0x55

#define FL2_R_PWM_REGISTER 0x25
#define FL2_G_PWM_REGISTER 0x35
#define FL2_B_PWM_REGISTER 0x56

#define FL3_R_PWM_REGISTER 0x26
#define FL3_G_PWM_REGISTER 0x36
#define FL3_B_PWM_REGISTER 0x46

#define FL4_R_PWM_REGISTER 0x27
#define FL4_G_PWM_REGISTER 0x37
#define FL4_B_PWM_REGISTER 0x47

#define FL5_R_PWM_REGISTER 0x28
#define FL5_G_PWM_REGISTER 0x38
#define FL5_B_PWM_REGISTER 0x48

#define FL6_R_PWM_REGISTER 0x29
#define FL6_G_PWM_REGISTER 0x39
#define FL6_B_PWM_REGISTER 0x49

#define FL7_R_PWM_REGISTER 0x2A
#define FL7_G_PWM_REGISTER 0x3A
#define FL7_B_PWM_REGISTER 0x4A

#define FL8_R_PWM_REGISTER 0x2B
#define FL8_G_PWM_REGISTER 0x3B
#define FL8_B_PWM_REGISTER 0x4B

#define FL9_R_PWM_REGISTER 0xA4
#define FL9_G_PWM_REGISTER 0x94
#define FL9_B_PWM_REGISTER 0x84

#define FL10_R_PWM_REGISTER 0xA5
#define FL10_G_PWM_REGISTER 0x95
#define FL10_B_PWM_REGISTER 0x85

#define FL11_R_PWM_REGISTER 0xA6
#define FL11_G_PWM_REGISTER 0x96
#define FL11_B_PWM_REGISTER 0x86

#define FL12_R_PWM_REGISTER 0xA7
#define FL12_G_PWM_REGISTER 0x97
#define FL12_B_PWM_REGISTER 0x87

#define FL13_R_PWM_REGISTER 0xA8
#define FL13_G_PWM_REGISTER 0x98
#define FL13_B_PWM_REGISTER 0x88

#define FL14_R_PWM_REGISTER 0xA9
#define FL14_G_PWM_REGISTER 0x99
#define FL14_B_PWM_REGISTER 0x89

#define FL15_R_PWM_REGISTER 0xAA
#define FL15_G_PWM_REGISTER 0x9A
#define FL15_B_PWM_REGISTER 0x79

#define FL16_R_PWM_REGISTER 0xAB
#define FL16_G_PWM_REGISTER 0x8A
#define FL16_B_PWM_REGISTER 0x7A

//Logo
#define D107_R_PWM_REGISTER 0x33
#define D107_G_PWM_REGISTER 0x43
#define D107_B_PWM_REGISTER 0x53

//Wheel
#define D1_R_PWM_REGISTER 0x32
#define D1_G_PWM_REGISTER 0x42
#define D1_B_PWM_REGISTER 0x52

#define D2_R_PWM_REGISTER 0x31
#define D2_G_PWM_REGISTER 0x41
#define D2_B_PWM_REGISTER 0x51

static U8 LED_RED_PWM_REGISTER_TABLE[]=
{
	0x25, 0x28, 0x57, 0x59, 0x56, 0x26, 0x55, 0x2A,
	0x24, 0x58, 0x29, 0x54, 0x27, 0x2B, 0x5A, 0x5B
};

static U8 LED_GREEN_PWM_REGISTER_TABLE[]=
{
	0x35, 0x38, 0x67, 0x69, 0x66, 0x36, 0x65, 0x3A,
	0x34, 0x68, 0x39, 0x64, 0x37, 0x3B, 0x6A, 0x6B
};

static U8 LED_BLUE_PWM_REGISTER_TABLE[]=
{
	0x45, 0x48, 0x77, 0x79, 0x76, 0x46, 0x75, 0x4A,
	0x44, 0x78, 0x49, 0x74, 0x47, 0x4B, 0x7A, 0x7B
};

#define D72_REGISTER 1<<1
#define D73_REGISTER 1<<4
#define D74_REGISTER 1<<3
#define D75_REGISTER 1<<5
#define D76_REGISTER 1<<2
#define D77_REGISTER 1<<2
#define D78_REGISTER 1<<1
#define D79_REGISTER 1<<6

#define D80_REGISTER 1<<0
#define D81_REGISTER 1<<4
#define D82_REGISTER 1<<5
#define D83_REGISTER 1<<0
#define D84_REGISTER 1<<3
#define D85_REGISTER 1<<7
#define D86_REGISTER 1<<6
#define D87_REGISTER 1<<7

static U8 LED_REGISTER_BIT_TABLE[]=
{
	0x02, 0x10, 0x08, 0x20, 0x04, 0x04, 0x02, 0x40,
	0x01, 0x10, 0x20, 0x01, 0x08, 0x80, 0x40, 0x80
};

static U8 LED_RED_REGISTER_TABLE[]=
{
	0x00, 0x00, 0x06, 0x06, 0x06, 0x00, 0x06, 0x00,
	0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x06, 0x06
};

static U8 LED_GREEN_REGISTER_TABLE[]=
{
	0x02, 0x02, 0x08, 0x08, 0x08, 0x02, 0x08, 0x02,
	0x02, 0x08, 0x02, 0x08, 0x02, 0x02, 0x08, 0x08
};

static U8 LED_BLUE_REGISTER_TABLE[]=
{
	0x04, 0x04, 0x0A, 0x0A, 0x0A, 0x04, 0x0A, 0x04,
	0x04, 0x0A, 0x04, 0x0A, 0x04, 0x04, 0x0A, 0x0A
};

#define	SDB_HIGH	\
	do{														\
		NDrv_GPIO_SetDir(PORT1, 26, GPIOOUT);					\
		NDrv_GPIO_SetBitValue(PORT1, 26, HIGH); \
	}while(0);

#define	SDB_LOW	\
	do{														\
		NDrv_GPIO_SetDir(PORT1, 26, GPIOOUT);					\
		NDrv_GPIO_SetBitValue(PORT1, 26, LOW);	\
	}while(0);


#define Configuration_Reg 			0x00
#define Picture_Display_Reg 			0x01
#define Auto_Play_Control_1_Reg 		0x02
#define Auto_Play_Control_2_Reg 		0x03
#define Display_Option_Reg			0x05
#define Audio_Synchronization_Reg 	0x06
#define Fram_State_Reg			   	0x07
#define Breath_Control_1_Reg			0x08
#define Breath_Control_2_Reg			0x09
#define Shutdown_Reg				0x0A
#define AGC_Control_Reg				0x0B
#define Audio_ADC_Rate_Reg			0x0C



