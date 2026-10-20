/*****************************************************************************
 *   i2c.h:  Header file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"

#ifdef  __I2C_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if 0
#define _DEBUG_I2C_(x) x
#else
#define _DEBUG_I2C_(x)
#endif

#define I2C_SCL_PIN_CONFIG 		LPC_IOCON->PIO0_4
#define I2C_SDA_PIN_CONFIG 	LPC_IOCON->PIO0_4
//#define BUTTON_LEFT_PIN 		24
//#define BUTTON_LEFT_PORT 		PORT1

#define DISABLE_I2C FALSE

#define FAST_MODE_PLUS      0

#define BUFSIZE            (64+3)
#define MAX_TIMEOUT    0x00FFFFFF  //0x00FFFFFF

#define I2CMASTER           0x01
#define I2CSLAVE            0x02

#define AT24C512_ADDR       0xA0
#define READ_WRITE          0x01

#define I2C_PAGE_WRITE      0x40

/* For more info, read NXP's SE95 datasheet */
//#define SE95_ADDR			0x90
//#define SE95_ID				0x05
//#define SE95_CONFIG			0x01
//#define SE95_TEMP			0x00

#define RD_BIT              0x01

#define I2C_IDLE              0
#define I2C_STARTED           1
#define I2C_RESTARTED         2
#define I2C_REPEATED_START    3
#define DATA_ACK              4
#define DATA_NACK             5
#define I2C_BUSY              6
#define I2C_NO_DATA           7
#define I2C_NACK_ON_ADDRESS   8
#define I2C_NACK_ON_DATA      9
#define I2C_ARBITRATION_LOST  10
#define I2C_TIME_OUT          11
#define I2C_OK                12

#define I2CONSET_I2EN       (0x1<<6)  /* I2C Control Set Register */
#define I2CONSET_AA         (0x1<<2)
#define I2CONSET_SI         (0x1<<3)
#define I2CONSET_STO        (0x1<<4)
#define I2CONSET_STA        (0x1<<5)

#define I2CONCLR_AAC        (0x1<<2)  /* I2C Control clear Register */
#define I2CONCLR_SIC        (0x1<<3)
#define I2CONCLR_STAC       (0x1<<5)
#define I2CONCLR_I2ENC      (0x1<<6)

#define I2DAT_I2C           0x00000000  /* I2C Data Reg */
#define I2ADR_I2C           0x00000000  /* I2C Slave Address Reg */

#define I2SCLH_HS_SCLH		0x00000015  /* Fast Plus I2C SCL Duty Cycle High Reg */
#define I2SCLL_HS_SCLL		0x00000015  /* Fast Plus I2C SCL Duty Cycle Low Reg */
#define I2C_SPEED           400000UL//(ATM24C256)3.3v-1Mhz //400000UL//400K
#define I2SCLH_SCLH         (SystemCoreClock / I2C_SPEED) / 2/* I2C SCL Duty Cycle High Reg */
#define I2SCLL_SCLL         I2SCLH_SCLH/* I2C SCL Duty Cycle Low Reg */

#define EEPROM_WP_ENABLE 		 FALSE
#define EEPROM_WRITE_PROTECT_ENABLE()    NDrv_GPIO_SetBitValue(PORT0,11,LOW)
#define EEPROM_WRITE_PROTECT_DISABLE()   NDrv_GPIO_SetBitValue(PORT0,11,HIGH)


typedef  struct _LPC11Uxx_I2C_SETTING_
{
	volatile U32 u32Timeout; 
	volatile U8 u8I2CMasterState;
	volatile U8 u8I2CSlaveState;	
	volatile U8 u8I2CMasterBuffer[BUFSIZE];
	volatile U8 u8I2CSlaveBuffer[BUFSIZE];
	volatile U8 u8I2CCount;
	volatile U8 u8I2CReadLength;
	volatile U8 u8I2CWriteLength;
	volatile U8 u8RdIndex;
	volatile U8 u8WrIndex;
}LPC11Uxx_I2C_SETTING;

INTERFACE LPC11Uxx_I2C_SETTING g_I2C_Setting; 


INTERFACE void I2C_IRQHandler(void);
INTERFACE void nsAPI_I2C_Init(void);
INTERFACE void NDrv_API_I2C_Stop(void);
//INTERFACE void nsAPI_I2C_Write(void);
//INTERFACE void nsAPI_I2C_Read(void);
INTERFACE volatile void nsAPI_I2C_Write_Database(U16 u16Adr,U8 *pBuf,U16 u16Len);
INTERFACE volatile void nsAPI_I2C_Read_Database(U16 u16Adr,U8 *pBuf,U16 u16Len);
INTERFACE void nsAPI_I2C_Engine( void );

#undef INTERFACE
#ifdef __cplusplus
}
#endif
#endif /* end __I2C_H */

