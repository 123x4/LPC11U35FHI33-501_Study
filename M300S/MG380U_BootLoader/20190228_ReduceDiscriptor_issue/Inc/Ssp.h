#ifndef __SSP_H__
#define __SSP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "Type.h"
#include "Gpio.h"


#ifdef  __SSP_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if 0
#define _DEBUG_SSP_(x) x
#else
#define _DEBUG_SSP_(x) 
#endif

#if 0
#define _DEBUG_SSP2_(x) x
#else
#define _DEBUG_SSP2_(x) 
#endif

#if 0
#define _LASER_POWER_CALIBRATION_ 0
/*Laser_Power_Calibration*/
#define LASER_POWER_CALIBRATION_MIN 0x10
#define LASER_POWER_CALIBRATION_DEFAULT 0x80
#define LASER_POWER_CALIBRATION_MAX 0xFF
#endif

#define NCS_HIGH()				NDrv_GPIO_SetBitValue(PORT0, 2, HIGH) 
#define NCS_LOW()				NDrv_GPIO_SetBitValue(PORT0, 2, LOW)
#define NRESET_HIGH()			NDrv_GPIO_SetBitValue(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, HIGH);
#define NRESET_LOW()			NDrv_GPIO_SetBitValue(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, LOW);
//#define SENSOR_MOSI_PIN_CONFIG 	LPC_IOCON->PIO0_9
//#define SENSOR_MISO_PIN_CONFIG 	LPC_IOCON->PIO0_8
//#define SENSOR_SCK_PIN_CONFIG 		LPC_IOCON->PIO1_29
//#define SENSOR_NCS_PIN_CONFIG 		LPC_IOCON->PIO0_2
//#define SENSOR_MOTION_PIN_CONFIG 	LPC_IOCON->PIO1_15
//#define SENSOR_NRESET_PIN_CONFIG 	LPC_IOCON->PIO1_15

#define SENSOR_MOSI_PIN 		9
#define SENSOR_MOSI_PORT 		PORT0
#define SENSOR_MISO_PIN 		8
#define SENSOR_MISO_PORT 		PORT0
#define SENSOR_SCK_PIN 			29
#define SENSOR_SCK_PORT 		PORT1
#define SENSOR_MOTION_PIN 		15
#define SENSOR_MOTION_PORT 	PORT1
#define SENSOR_NCS_PIN 			2
#define SENSOR_NCS_PORT 		PORT0
#define SENSOR_NRESET_PIN 		22
#define SENSOR_NRESET_PORT 	PORT0

/*Report Rate (Hz)*/
#if 0
#define _125Hz 0x01
#define _250Hz 0x02
#define _500Hz 0x03
#define _1000Hz 0x04
#else
//#define _125Hz 0x01
#define _250Hz 0x04
#define _500Hz 0x02
#define _1000Hz 0x01
#endif

//#define ANS9800_DEFAULT_REPORT_RATE _500Hz //(Add) 20120503 add report rate

#define _125Hz_FrameCapture  4888
#define _250Hz_FrameCapture  2628
#define _500Hz_FrameCapture  912
#define _1000Hz_FrameCapture  0

#define ColorMaxValue 255
#if 0
/*Sensor DPI Setting*/
#define _90_DPI 0x01
#define _1620_DPI 0x02
#define _3240_DPI 0x03
#define _5040_DPI 0x04

#define _90_DPI_REGISTER_VALUE 0x01
#define _1620_DPI_REGISTER_VALUE 0x12
#define _3240_DPI_REGISTER_VALUE 0x24
#define _5040_DPI_REGISTER_VALUE 0x38
#endif


/*Sensor Resolution*/
#define Rpt_Mod BIT2
#define MIN_XY_RESOULTION 0x01
#define MAX_XY_RESOULTION 0x29//0x38
#define MIN_X_RESOULTION 0x01
#define MAX_X_RESOULTION 0x29//0x38
#define MIN_Y_RESOULTION MIN_X_RESOULTION
#define MAX_Y_RESOULTION MAX_X_RESOULTION//(MAX_X_RESOULTION -1)

#define FRAME_CAPTURE_SIZE 900 
#define SSP_FIFOSIZE 8/* SPI read and write buffer size */
#define MOTIONBURSTSIZE 14

/* SSP Clock div (0..255) */
#define SCB_CLKDIV_DISABLE (0x00)
#define SCB_CLKDIV_DIV1 (0x01)
#define SCB_CLKDIV_DIV2 (0x02)
#define SCB_CLKDIV_DIV3 (0x03)
#define SCB_CLKDIV_DIV4 (0x04)
#define SCB_CLKDIV_DIV5 (0x05)
#define SCB_CLKDIV_DIV6 (0x06)
#define SCB_CLKDIV_DIV7 (0x07)
#define SCB_CLKDIV_DIV8 (0x08)
#define SCB_CLKDIV_DIV9 (0x09)
#define SCB_CLKDIV_DIV10 (0x0A)
#if 0
#define SCB_CLKDIV_DIV11 (0x0B)
#define SCB_CLKDIV_DIV12 (0x0C)
#define SCB_CLKDIV_DIV13 (0x0D)
#define SCB_CLKDIV_DIV14 (0x0E)
#define SCB_CLKDIV_DIV15 (0x0F)
#define SCB_CLKDIV_DIV16 (0x10)
#define SCB_CLKDIV_DIV17 (0x11)
#define SCB_CLKDIV_DIV18 (0x12)
#define SCB_CLKDIV_DIV19 (0x13)
#define SCB_CLKDIV_DIV20 (0x14)
#define SCB_CLKDIV_DIV21 (0x15)
#define SCB_CLKDIV_DIV22 (0x16)
#define SCB_CLKDIV_DIV23 (0x17)
#define SCB_CLKDIV_DIV24 (0x18)
#define SCB_CLKDIV_DIV25 (0x19)
#define SCB_CLKDIV_DIV26 (0x1A)
#define SCB_CLKDIV_DIV27 (0x1B)
#define SCB_CLKDIV_DIV28 (0x1C)
#define SCB_CLKDIV_DIV29 (0x1D)
#define SCB_CLKDIV_DIV30 (0x1E)
#define SCB_CLKDIV_DIV31 (0x1F)
#define SCB_CLKDIV_DIV32 (0x20)
#define SCB_CLKDIV_DIV33 (0x21)
#define SCB_CLKDIV_DIV34 (0x22)
#define SCB_CLKDIV_DIV35 (0x23)
#define SCB_CLKDIV_DIV36 (0x24)
#define SCB_CLKDIV_DIV37 (0x25)
#define SCB_CLKDIV_DIV38 (0x26)
#define SCB_CLKDIV_DIV39 (0x27)
#define SCB_CLKDIV_DIV40 (0x28)
#define SCB_CLKDIV_DIV41 (0x29)
#define SCB_CLKDIV_DIV42 (0x2A)
#define SCB_CLKDIV_DIV43 (0x2B)
#define SCB_CLKDIV_DIV44 (0x2C)
#define SCB_CLKDIV_DIV45 (0x2D)
#define SCB_CLKDIV_DIV46 (0x2E)
#define SCB_CLKDIV_DIV47 (0x2F)
#define SCB_CLKDIV_DIV48 (0x30)
#define SCB_CLKDIV_DIV49 (0x31)
#define SCB_CLKDIV_DIV50 (0x32)
#endif
#define SCB_CLKDIV_MASK (0x000000FF)

/* SSP Serial Clock Rate Values */
#define SSP_SCR_1 (0x100)
#define SSP_SCR_2 (0x200)
#define SSP_SCR_3 (0x300)
#define SSP_SCR_4 (0x400)
#define SSP_SCR_5 (0x500)
#define SSP_SCR_6 (0x600)
#define SSP_SCR_7 (0x700)
#define SSP_SCR_8 (0x800)
#define SSP_SCR_9 (0x900)
#define SSP_SCR_10 (0xA00)
#define SSP_SCR_11 (0xB00)
#define SSP_SCR_12 (0xC00)
#define SSP_SCR_13 (0xD00)
#define SSP_SCR_14 (0xE00)
#define SSP_SCR_15 (0xF00)
#define SSP_SCR_16 (0x1000)
#define SSP_SCR_17 (0x1100)
#define SSP_SCR_18 (0x1200)
#define SSP_SCR_19 (0x1300)
#define SSP_SCR_20 (0x1400)
#define SSP_SCR_21 (0x1500)
#define SSP_SCR_22 (0x1600)
#define SSP_SCR_23 (0x1700)
#if 0
#define SSP_SCR_24 (0x1800)
#define SSP_SCR_25 (0x1900)
#define SSP_SCR_26 (0x1A00)
#define SSP_SCR_27 (0x1B00)
#define SSP_SCR_28 (0x1C00)
#define SSP_SCR_29 (0x1D00)
#define SSP_SCR_30 (0x1E00)
#define SSP_SCR_31 (0x1F00)
#define SSP_SCR_32 (0x2000)
#define SSP_SCR_33 (0x2100)
#define SSP_SCR_34 (0x2200)
#define SSP_SCR_35 (0x2300)
#define SSP_SCR_36 (0x2400)
#define SSP_SCR_37 (0x2500)
#define SSP_SCR_38 (0x2600)
#define SSP_SCR_39 (0x2700)
#define SSP_SCR_40 (0x2800)
#define SSP_SCR_41 (0x2900)
#define SSP_SCR_42 (0x2A00)
#define SSP_SCR_43 (0x2B00)
#define SSP_SCR_44 (0x2C00)
#define SSP_SCR_45 (0x2D00)
#define SSP_SCR_46 (0x2E00)
#define SSP_SCR_47 (0x2F00)
#define SSP_SCR_48 (0x3000)
#define SSP_SCR_49 (0x3100)
#define SSP_SCR_50 (0x3200)
#define SSP_SCR_51 (0x3300)
#define SSP_SCR_52 (0x3400)
#define SSP_SCR_53 (0x3500)
#define SSP_SCR_54 (0x3600)
#define SSP_SCR_55 (0x3700)
#define SSP_SCR_56 (0x3800)
#define SSP_SCR_57 (0x3900)
#define SSP_SCR_58 (0x3A00)
#define SSP_SCR_59 (0x3B00)
#define SSP_SCR_60 (0x3C00)
#define SSP_SCR_61 (0x3D00)
#define SSP_SCR_62 (0x3E00)
#define SSP_SCR_63 (0x3F00)
#define SSP_SCR_64 (0x4000)
#define SSP_SCR_65 (0x4100)
#define SSP_SCR_66 (0x4200)
#define SSP_SCR_67 (0x4300)
#define SSP_SCR_68 (0x4400)
#define SSP_SCR_69 (0x4500)
#define SSP_SCR_70 (0x4600)
#define SSP_SCR_71 (0x4700)
#define SSP_SCR_72 (0x4800)
#define SSP_SCR_73 (0x4900)
#define SSP_SCR_74 (0x4A00)
#define SSP_SCR_75 (0x4B00)
#define SSP_SCR_76 (0x4C00)
#define SSP_SCR_77 (0x4D00)
#define SSP_SCR_78 (0x4E00)
#define SSP_SCR_79 (0x4F00)
#define SSP_SCR_80 (0x5000)
#define SSP_SCR_81 (0x5100)
#define SSP_SCR_82 (0x5200)
#define SSP_SCR_83 (0x5300)
#define SSP_SCR_84 (0x5400)
#define SSP_SCR_85 (0x5500)
#define SSP_SCR_86 (0x5600)
#define SSP_SCR_87 (0x5700)
#define SSP_SCR_88 (0x5800)
#define SSP_SCR_89 (0x5900)
#define SSP_SCR_90 (0x5A00)
#define SSP_SCR_91 (0x5B00)
#define SSP_SCR_92 (0x5C00)
#define SSP_SCR_93 (0x5D00)
#define SSP_SCR_94 (0x5E00)
#define SSP_SCR_95 (0x5F00)
#define SSP_SCR_96 (0x6000)
#define SSP_SCR_97 (0x6100)
#define SSP_SCR_98 (0x6200)
#define SSP_SCR_99 (0x6300)
#define SSP_SCR_100 (0x6400)
#define SSP_SCR_101 (0x6500)
#define SSP_SCR_102 (0x6600)
#define SSP_SCR_103 (0x6700)
#define SSP_SCR_104 (0x6800)
#define SSP_SCR_105 (0x6900)
#define SSP_SCR_106 (0x6A00)
#define SSP_SCR_107 (0x6B00)
#define SSP_SCR_108 (0x6C00)
#define SSP_SCR_109 (0x6D00)
#define SSP_SCR_110 (0x6E00)
#define SSP_SCR_111 (0x6F00)
#define SSP_SCR_112 (0x7000)
#define SSP_SCR_113 (0x7100)
#define SSP_SCR_114 (0x7200)
#define SSP_SCR_115 (0x7300)
#define SSP_SCR_116 (0x7400)
#define SSP_SCR_117 (0x7500)
#define SSP_SCR_118 (0x7600)
#define SSP_SCR_119 (0x7700)
#define SSP_SCR_120 (0x7800)
#define SSP_SCR_121 (0x7900)
#define SSP_SCR_122 (0x7A00)
#define SSP_SCR_123 (0x7B00)
#define SSP_SCR_124 (0x7C00)
#define SSP_SCR_125 (0x7D00)
#define SSP_SCR_126 (0x7E00)
#define SSP_SCR_127 (0x7F00)
#define SSP_SCR_128 (0x8000)
#define SSP_SCR_129 (0x8100)
#define SSP_SCR_130 (0x8200)
#define SSP_SCR_131 (0x8300)
#define SSP_SCR_132 (0x8400)
#define SSP_SCR_133 (0x8500)
#define SSP_SCR_134 (0x8600)
#define SSP_SCR_136 (0x8700)
#define SSP_SCR_137 (0x8800)
#define SSP_SCR_138 (0x8800)
#define SSP_SCR_139 (0x8900)
#define SSP_SCR_140 (0x8A00)
#define SSP_SCR_141 (0x8B00)
#define SSP_SCR_142 (0x8C00)
#define SSP_SCR_143 (0x8D00)
#define SSP_SCR_144 (0x8E00)
#define SSP_SCR_145 (0x8F00)
#define SSP_SCR_146 (0x9000)
#define SSP_SCR_147 (0x9100)
#define SSP_SCR_148 (0x9200)
#define SSP_SCR_149 (0x9300)
#define SSP_SCR_150 (0x9400)
#define SSP_SCR_151 (0x9500)
#define SSP_SCR_152 (0x9600)
#define SSP_SCR_153 (0x9700)
#define SSP_SCR_154 (0x9800)
#define SSP_SCR_155 (0x9900)
#define SSP_SCR_156 (0x9A00)
#define SSP_SCR_157 (0x9B00)
#define SSP_SCR_158 (0x9C00)
#define SSP_SCR_159 (0x9D00)
#define SSP_SCR_160 (0x9E00)
#define SSP_SCR_161 (0x9F00)
#define SSP_SCR_162 (0xA000)
#define SSP_SCR_163 (0xA100)
#define SSP_SCR_164 (0xA200)
#define SSP_SCR_165 (0xA300)
#define SSP_SCR_166 (0xA400)
#define SSP_SCR_167 (0xA500)
#define SSP_SCR_168 (0xA600)
#define SSP_SCR_169 (0xA700)
#define SSP_SCR_170 (0xA800)
#define SSP_SCR_171 (0xA900)
#define SSP_SCR_172 (0xAA00)
#define SSP_SCR_173 (0xAB00)
#define SSP_SCR_174 (0xAC00)
#define SSP_SCR_175 (0xAD00)
#define SSP_SCR_176 (0xAE00)
#define SSP_SCR_177 (0xAF00)
#define SSP_SCR_178 (0xB000)
#define SSP_SCR_179 (0xB100)
#define SSP_SCR_180 (0xB200)
#define SSP_SCR_181 (0xB300)
#define SSP_SCR_182 (0xB400)
#define SSP_SCR_183 (0xB500)
#define SSP_SCR_184 (0xB600)
#define SSP_SCR_185 (0xB700)
#define SSP_SCR_186 (0xB800)
#define SSP_SCR_187 (0xB900)
#define SSP_SCR_188 (0xBA00)
#define SSP_SCR_189 (0xBB00)
#define SSP_SCR_190 (0xBC00)
#define SSP_SCR_191 (0xBD00)
#define SSP_SCR_192 (0xBE00)
#define SSP_SCR_193 (0xBF00)
#define SSP_SCR_194 (0xC000)
#define SSP_SCR_195 (0xC100)
#define SSP_SCR_196 (0xC200)
#define SSP_SCR_197 (0xC300)
#define SSP_SCR_198 (0xC400)
#define SSP_SCR_199 (0xC500)
#define SSP_SCR_200 (0xC600)
#define SSP_SCR_201 (0xC700)
#define SSP_SCR_202 (0xC800)
#define SSP_SCR_203 (0xC900)
#define SSP_SCR_204 (0xCA00)
#define SSP_SCR_205 (0xCB00)
#define SSP_SCR_206 (0xCC00)
#define SSP_SCR_207 (0xCD00)
#define SSP_SCR_208 (0xCE00)
#define SSP_SCR_209 (0xCF00)
#define SSP_SCR_210 (0xD000)
#define SSP_SCR_211 (0xD100)
#define SSP_SCR_212 (0xD200)
#define SSP_SCR_213 (0xD300)
#define SSP_SCR_214 (0xD400)
#define SSP_SCR_215 (0xD500)
#define SSP_SCR_216 (0xD600)
#define SSP_SCR_217 (0xD700)
#define SSP_SCR_218 (0xD800)
#define SSP_SCR_219 (0xD900)
#define SSP_SCR_220 (0xDA00)
#define SSP_SCR_221 (0xDB00)
#define SSP_SCR_222 (0xDC00)
#define SSP_SCR_223 (0xDD00)
#define SSP_SCR_224 (0xDE00)
#define SSP_SCR_225 (0xDF00)
#define SSP_SCR_226 (0xE000)
#define SSP_SCR_227 (0xE100)
#define SSP_SCR_228 (0xE200)
#define SSP_SCR_229 (0xE300)
#define SSP_SCR_230 (0xE400)
#endif

/*  Current status of the SSP controller.  */
#define SSP_SR_TFE_MASK (0x00000001) // Transmit FIFO empty
#define SSP_SR_TFE_EMPTY (0x00000001)
#define SSP_SR_TFE_NOTEMPTY (0x00000000)
#define SSP_SR_TNF_MASK (0x00000002) // Transmit FIFO not full
#define SSP_SR_TNF_NOTFULL (0x00000002)
#define SSP_SR_TNF_FULL (0x00000000)
#define SSP_SR_RNE_MASK (0x00000004) // Receive FIFO not empty
#define SSP_SR_RNE_NOTEMPTY (0x00000004)
#define SSP_SR_RNE_EMPTY (0x00000000)
#define SSP_SR_RFF_MASK (0x00000008) // Receive FIFO full
#define SSP_SR_RFF_FULL (0x00000008)
#define SSP_SR_RFF_NOTFULL (0x00000000)
#define SSP_SR_BSY_MASK (0x00000010) // Busy Flag
#define SSP_SR_BSY_IDLE (0x00000000)
#define SSP_SR_BSY_BUSY (0x00000010)

/* Control Register 1 */
#define SSP_CR1_LBM_MASK BIT0 // Loop back mode
#define SSP_CR1_LBM_NORMAL (0)
#define SSP_CR1_LBM_INVERTED BIT0 // MISO/MOSI are reversed
#define SSP_CR1_SSE_MASK BIT1 // SSP enable
#define SSP_CR1_SSE_DISABLED (0)
#define SSP_CR1_SSE_ENABLED BIT1
#define SSP_CR1_MS_MASK BIT2 // Master/Slave Mode
#define SSP_CR1_MS_MASTER (0)
#define SSP_CR1_MS_SLAVE BIT2
#define SSP_CR1_SOD_MASK BIT2 // Slave output disable

/*Avago-ADNS9800 Status*/
#define SSP_FLAG_ADNS9800_SAVE_REPORT_RATE BIT0
#define SSP_FLAG_ADNS9800_SAVE_FRAME_CAPTURE BIT1

#define NDrv_SSP_ADNS9800_SSEL(x) NDrv_GPIO_SetBitValue(PORT0,2,x)	
//#define NDrv_SSP_PMW3360_SSEL(x) NDrv_GPIO_SetBitValue(PORT0,2,x)	

typedef struct _LPC11Uxx_SSP_ADNS9800_SETTING_
{  		
	U8 DPIXLevel[6];
	U8 MaxDPILevelIndex;
	U8 MinDPILevelIndex;
	volatile U8 sSensorInit;
	U8 sSensorStatus;
}LPC11Uxx_SSP_ADNS9800_SETTING;

INTERFACE LPC11Uxx_SSP_ADNS9800_SETTING g_SSP_ADNS9800_Setting;



#define NDrv_SSP_ADNS9800_Get_Motion() NDrv_GPIO_GetPinValue(PORT0,BIT19) 

INTERFACE void SSP0_IRQHandler(void); 
INTERFACE void SSP1_IRQHandler(void); 
INTERFACE void nsAPI_SSP0_Init(void);
INTERFACE void nsAPI_SSP1_Init(void);
INTERFACE void nsAPI_SSP0_Send(U8 *buf, U16 length);
INTERFACE void nsAPI_SSP1_Send(U8 *buf, U16 length);
INTERFACE void nsAPI_SSP0_Receive(U8 *buf, U16 length);
INTERFACE void nsAPI_SSP1_Receive(U8 *buf, U16 length);
INTERFACE void nsAPI_SSP_Sensor_Delsyus(U16 u16DelayTime);
INTERFACE void nsAPI_SSP_Sensor_Delsyms(U16 u16DelayTime);
//INTERFACE void nsAPI_SSP_Sensor_Monitor(void);
INTERFACE bool nsAPI_SSP_Sensor_Monitor(U16 * X_Value, U16* Y_Value);
INTERFACE void nsAPI_SSP_Switch_DPILevel(void);

/*
#if _LASER_POWER_CALIBRATION_
INTERFACE RetU8 nsAPI_SSP_ADNS9800_Frame_Capture(void);
INTERFACE void nsAPI_SSP_ADNS9800_Laser_Power_Calibration(U8 u8Ave);
#endif
*/
//INTERFACE RetU16 nsAPI_SSP_ADNS9800_GetReportRate(U8 *u8ReportRate);
INTERFACE void nsAPI_SSP_Set_Sensor_X_Resolution(U8 u8Index ,BOOLEAN bEnable);
INTERFACE void nsAPI_SSP_Set_Sensor_Y_Resolution(U8 u8Index ,BOOLEAN bEnable);
INTERFACE void nsAPI_SSP_Set_Sensor_XY_Resolution(void);
//INTERFACE RetU8 nsAPI_SSP_Get_Sensor_XY_Resolution(U8 u8ResolutionIndex);

//void S_ADNS9800_Value_INIT(void);
 
#undef INTERFACE
#ifdef __cplusplus
}
#endif
#endif
