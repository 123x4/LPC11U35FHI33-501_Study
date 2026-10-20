/****************************************************************************
 *   $Id:: i2c.c 4058 2010-07-30 01:03:21Z usb00423                         $
 *   Project: NXP LPC11Uxx I2C example
 *
 *   Description:
 *     This file contains I2C code example which include I2C initialization,
 *     I2C interrupt handler, and APIs for I2C access.
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

* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.

****************************************************************************/
#define __I2C_C__

#include "LPC11Uxx.h"			/* LPC11xx Peripheral Registers */
#include "type.h"
#include "i2c.h"
#include "ssp.h"
#include "uart.h"
#if 0
volatile uint8_t I2CMasterState = I2C_IDLE;
volatile uint8_t I2CSlaveState = I2C_IDLE;
volatile uint32_t timeout = 0;

//volatile uint32_t I2CMode;

volatile uint16_t I2CMasterBuffer[BUFSIZE];
volatile uint8_t I2CSlaveBuffer[BUFSIZE];
volatile uint32_t I2CCount = 0;
volatile uint16_t I2CReadLength;
volatile uint16_t I2CWriteLength;

volatile uint16_t RdIndex = 0;
volatile uint16_t WrIndex = 0;
#endif
/*
From device to device, the I2C communication protocol may vary,
in the example below, the protocol uses repeated start to read data from or
write to the device:
For master read: the sequence is: STA,Addr(W),offset,RE-STA,Addr(r),data...STO
for master write: the sequence is: STA,Addr(W),offset,RE-STA,Addr(w),data...STO
Thus, in state 8, the address is always WRITE. in state 10, the address could
be READ or WRITE depending on the I2C command.
*/

/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void I2C_IRQHandler(void)
{

	U8 StatValue = LPC_I2C->STAT;

	g_I2C_Setting.u32Timeout = 0;
	/* this handler deals with master read and master write only */
 
	switch ( StatValue )
	{
		case 0x08:			/* A Start condition is issued. */
		g_I2C_Setting.u8WrIndex = 0;
		g_I2C_Setting.u8RdIndex = 0;
		LPC_I2C->DAT = g_I2C_Setting.u8I2CMasterBuffer[g_I2C_Setting.u8WrIndex++];
		LPC_I2C->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);	
		break;

		case 0x10:			/* A repeated started is issued */
		g_I2C_Setting.u8RdIndex = 0;
		/* Send SLA with R bit set, */
		LPC_I2C->DAT = g_I2C_Setting.u8I2CMasterBuffer[g_I2C_Setting.u8WrIndex++];
		LPC_I2C->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
		break;

		case 0x18:			/* Regardless, it's a ACK */
		if ( g_I2C_Setting.u8I2CWriteLength == 1 )
		{
			LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
			g_I2C_Setting.u8I2CMasterState = I2C_NO_DATA;
		}
		else
		{
			LPC_I2C->DAT = g_I2C_Setting.u8I2CMasterBuffer[g_I2C_Setting.u8WrIndex++];
		}
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

		case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
		if ( g_I2C_Setting.u8WrIndex < g_I2C_Setting.u8I2CWriteLength )
		{
			LPC_I2C->DAT = g_I2C_Setting.u8I2CMasterBuffer[g_I2C_Setting.u8WrIndex++]; /* this should be the last one */
		}
		else
		{
			if ( g_I2C_Setting.u8I2CReadLength != 0 )
			{
				LPC_I2C->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
			}
			else
			{
				LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
				g_I2C_Setting.u8I2CMasterState = I2C_OK;
			}
		}
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

		case 0x30:
		LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
		g_I2C_Setting.u8I2CMasterState = I2C_NACK_ON_DATA;
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

		case 0x40:	/* Master Receive, SLA_R has been sent */
		if ( (g_I2C_Setting.u8RdIndex + 1) < g_I2C_Setting.u8I2CReadLength )
		{
			/* Will go to State 0x50 */
			LPC_I2C->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
		}
		else
		{
			/* Will go to State 0x58 */
			LPC_I2C->CONCLR = I2CONCLR_AAC;	/* assert NACK after data is received */
		}
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

		case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
		g_I2C_Setting.u8I2CSlaveBuffer[g_I2C_Setting.u8RdIndex++] = LPC_I2C->DAT;
		if ( (g_I2C_Setting.u8RdIndex + 1) < g_I2C_Setting.u8I2CReadLength )
		{
			LPC_I2C->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
		}
		else
		{
			LPC_I2C->CONCLR = I2CONCLR_AAC;	/* assert NACK on last byte */
		}
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

		case 0x58:
		g_I2C_Setting.u8I2CSlaveBuffer[g_I2C_Setting.u8RdIndex++] = LPC_I2C->DAT;
		g_I2C_Setting.u8I2CMasterState = I2C_OK;
		LPC_I2C->CONSET = I2CONSET_STO;	/* Set Stop flag */
		LPC_I2C->CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
		break;

		case 0x20:		/* regardless, it's a NACK */
		case 0x48:
		LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
		g_I2C_Setting.u8I2CMasterState = I2C_NACK_ON_ADDRESS;
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

		case 0x38:		/* Arbitration lost, in this example, we don't
		deal with multiple master situation */
		default:
		g_I2C_Setting.u8I2CMasterState = I2C_ARBITRATION_LOST;
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;
	}

}
#if 0
/*****************************************************************************
** Function name:		I2CStart
**
** Descriptions:		Create I2C start condition, a timeout
**				value is set if the I2C never gets started,
**				and timed out. It's a fatal error.
**
** parameters:			None
** Returned value:		true or false, return false if timed out
**
*****************************************************************************/
/*Unused*/
uint32_t I2CStart( void )
{
  uint32_t timeout = 0;
  uint32_t retVal = FALSE;

	/*--- Issue a start condition ---*/
  LPC_I2C->CONSET = I2CONSET_STA;	/* Set Start flag */

	/*--- Wait until START transmitted ---*/
	while(1)
	{
	if ( I2CMasterState == I2C_STARTED )
		{
			retVal = TRUE;
			break;
		}
	if ( timeout >= MAX_TIMEOUT )
		{
			retVal = FALSE;
			break;
		}
	timeout++;
	}
	return(retVal);
}

/*****************************************************************************
** Function name:		I2CStop
**
** Descriptions:		Set the I2C stop condition, if the routine
**				never exit, it's a fatal bus error.
**
** parameters:			None
** Returned value:		true or never return
**
*****************************************************************************/
uint32_t I2CStop( void )
{
  LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
  LPC_I2C->CONCLR = I2CONCLR_SIC;  /* Clear SI flag */

	/*--- Wait for STOP detected ---*/
  while( LPC_I2C->CONSET & I2CONSET_STO );
  return TRUE;
}

/*****************************************************************************
** Function name:		I2CInit
**
** Descriptions:		Initialize I2C controller
**
** parameters:			I2c mode is either MASTER or SLAVE
** Returned value:		true or false, return false if the I2C
**				interrupt handler was not installed correctly
**
*****************************************************************************/
void nsAPI_I2C_Init(void)
{
	NVIC_DisableIRQ(I2C_IRQn);

	g_I2C_Setting.u8I2CMasterState = I2C_IDLE;
	g_I2C_Setting.u8I2CSlaveState = I2C_IDLE;
	g_I2C_Setting.u32Timeout = 0;
	g_I2C_Setting.u8RdIndex = 0;
	g_I2C_Setting.u8WrIndex = 0;

	LPC_SYSCON->PRESETCTRL |= BIT1;

	LPC_SYSCON->SYSAHBCLKCTRL |= BIT5;
	
#if (EEPROM_WP_ENABLE)
	LPC_IOCON->TDI_PIO0_11 &= ~0x07;
	LPC_IOCON->TDI_PIO0_11 |= BIT0; // PIO0_11
	NDrv_GPIO_SetDir(PORT0,11,GPIOOUT);/*EEPROM: WP*/
	EEPROM_WRITE_PROTECT_DISABLE(); 
#endif

	
	LPC_IOCON->PIO0_4 &= ~0x3F;		/*  I2C I/O config */
	LPC_IOCON->PIO0_4 |= 0x01;		/* I2C SCL */
	LPC_IOCON->PIO0_5 &= ~0x3F;
	LPC_IOCON->PIO0_5 |= 0x01;		/* I2C SDA */
	/* IOCON may change in the next release, save change for future references. */
	//  LPC_IOCON->PIO0_4 |= (0x1<<10);	/* open drain pins */
	//  LPC_IOCON->PIO0_5 |= (0x1<<10);	/* open drain pins */

	/*--- Clear flags ---*/
	LPC_I2C->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;

	/*--- Reset registers ---*/
#if FAST_MODE_PLUS
	LPC_IOCON->PIO0_4 |= (0x1<<9);
	LPC_IOCON->PIO0_5 |= (0x1<<9);
	LPC_I2C->SCLL   = I2SCLL_HS_SCLL;
	LPC_I2C->SCLH   = I2SCLH_HS_SCLH;
#else
	LPC_I2C->SCLL   = I2SCLL_SCLL;
	LPC_I2C->SCLH   = I2SCLH_SCLH;
#endif

//   #if (DISABLE_I2C == 0)
	/* Enable the I2C Interrupt */
	NVIC_EnableIRQ(I2C_IRQn);

	LPC_I2C->CONSET = I2CONSET_I2EN;
  // #endif
	
}
#endif
#if 0
/*****************************************************************************
** Function name:		I2CEngine
**
** Descriptions:		The routine to complete a I2C transaction
**				from start to stop. All the intermitten
**				steps are handled in the interrupt handler.
**				Before this routine is called, the read
**				length, write length, I2C master buffer,
**				and I2C command fields need to be filled.
**				see i2cmst.c for more details.
**
** parameters:			None
** Returned value:		true or false, return false only if the
**				start condition can never be generated and
**				timed out.
**
*****************************************************************************/
void nsAPI_I2C_Engine( void )
{
	g_I2C_Setting.u8RdIndex = 0;
	g_I2C_Setting.u8WrIndex = 0;

	/*--- Issue a start condition ---*/
	LPC_I2C->CONSET = I2CONSET_STA;	/* Set Start flag */

	g_I2C_Setting.u8I2CMasterState = I2C_BUSY;

	while(g_I2C_Setting.u8I2CMasterState == I2C_BUSY )
	{
		if(g_I2C_Setting.u32Timeout >= MAX_TIMEOUT )
		{
			g_I2C_Setting.u8I2CMasterState = I2C_TIME_OUT;
			break;
		}
		g_I2C_Setting.u32Timeout++;
	}
	LPC_I2C->CONCLR = I2CONCLR_STAC;  
}

volatile void nsAPI_I2C_Write_Database(U16 u16Adr,U8 *pBuf,U16 u16Len)
{
	U16 i;
	U8 u8TempAdr;
	U8 *pBufTmp;
       #if (DISABLE_I2C)
	NVIC_EnableIRQ(I2C_IRQn);
	LPC_I2C->CONSET = I2CONSET_I2EN;
       #endif

#if (EEPROM_WP_ENABLE)
	EEPROM_WRITE_PROTECT_ENABLE();
#endif


	pBufTmp=pBuf;
	//for ( i = 0; i < BUFSIZE; i++ )	/* clear buffer */                                                                                                                                                                                                                                                                                                                                                                                                          for ( i = 0; i < BUFSIZE; i++ )	/* clear buffer */
	//{
	//    I2CMasterBuffer[i] = 0;
	//}
	while(u16Len > 0)
	{
		u8TempAdr = u16Adr % I2C_PAGE_WRITE;
		u8TempAdr = I2C_PAGE_WRITE -u8TempAdr;

		if(u16Len >= u8TempAdr )
		{
			g_I2C_Setting.u8I2CWriteLength = 3 + u8TempAdr;
			u16Len -= u8TempAdr;
		}
		else
		{
			g_I2C_Setting.u8I2CWriteLength = 3 + u16Len;
			u16Len = 0;
		}


		for (i=0;i<(g_I2C_Setting.u8I2CWriteLength-3);i++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[i+3]= *pBuf++;
#if (ENABLE_UART)
			//xprintf("\r\nWrite=%x ,",I2CMasterBuffer[i+3]);
			//xprintf("[%x]",i);
#endif
		}

		g_I2C_Setting.u8I2CReadLength = 0;
		g_I2C_Setting.u8I2CMasterBuffer[0] = AT24C512_ADDR;

		g_I2C_Setting.u8I2CMasterBuffer[1] = (U8)(u16Adr >>8);
		g_I2C_Setting.u8I2CMasterBuffer[2] = (U8)(u16Adr & 0x00FF);

		nsAPI_I2C_Engine();

		u16Adr += u8TempAdr;
//		nsAPI_SSP_Sensor_Delsyms(6);//5ms
	}
	pBuf=pBufTmp;
#if (ENABLE_UART)
	//xprintf("\r\n adr=%x",(U8 )&g_I2C_Setting.I2CpMasterData);
	//_DEBUG_I2C_(xprintf("\r\n I2CW End",0));
#endif
#if (EEPROM_WP_ENABLE)
	EEPROM_WRITE_PROTECT_DISABLE();
#endif
  
       #if (DISABLE_I2C)
	LPC_I2C->CONSET &= ~I2CONSET_I2EN;  	   
   	NVIC_DisableIRQ(I2C_IRQn); 
       #endif
}
//#else
volatile void nsAPI_I2C_Write_Database(U16 u16Adr,U8 *pBuf,U16 u16Len)
{
	U16 i;
	U8 u8TempAdr;
	U8 *pBufTmp;
	pBufTmp=pBuf;
	while(u16Len > 0)
	{
		u8TempAdr = u16Adr % I2C_PAGE_WRITE;
		u8TempAdr = I2C_PAGE_WRITE -u8TempAdr;

		if(u16Len >= u8TempAdr )
		{
			g_I2C_Setting.u8I2CWriteLength = 3 + u8TempAdr;
			u16Len -= u8TempAdr;
		}
		else
		{
			g_I2C_Setting.u8I2CWriteLength = 3 + u16Len;
			u16Len = 0;
		}


		for (i=0;i<(g_I2C_Setting.u8I2CWriteLength-3);i++)
		{
			g_I2C_Setting.u8I2CMasterBuffer[i+3]= *pBuf++;
		}

		g_I2C_Setting.u8I2CReadLength = 0;
		g_I2C_Setting.u8I2CMasterBuffer[0] = AT24C512_ADDR;

		g_I2C_Setting.u8I2CMasterBuffer[1] = (U8)(u16Adr >>8);
		g_I2C_Setting.u8I2CMasterBuffer[2] = (U8)(u16Adr & 0x00FF);

		nsAPI_I2C_Engine();

		u16Adr += u8TempAdr;
//		nsAPI_SSP_Sensor_Delsyms(6);//5ms
	}
	pBuf=pBufTmp;
 
}
#endif
#if 0
volatile void nsAPI_I2C_Read_Database(U16 u16Adr ,U8 *pBuf, U16 u16Len)
{
	U16 i;
	//U8 u8TempAdr;
	U8 *pBufTmp;

       #if (DISABLE_I2C)
	NVIC_EnableIRQ(I2C_IRQn);
	LPC_I2C->CONSET = I2CONSET_I2EN;
       #endif	

	pBufTmp = pBuf;
	while(u16Len > 0)
	{
		g_I2C_Setting.u8I2CWriteLength = 3;
		g_I2C_Setting.u8I2CReadLength = 0;
		g_I2C_Setting.u8I2CMasterBuffer[0] = AT24C512_ADDR;

		g_I2C_Setting.u8I2CMasterBuffer[1] = (U8)(u16Adr >>8);
		g_I2C_Setting.u8I2CMasterBuffer[2] = (U8)(u16Adr & 0x00FF);

		nsAPI_I2C_Engine( );
#if 0  // 20120710
		u8TempAdr = u16Adr % I2C_PAGE_WRITE;
		u8TempAdr = I2C_PAGE_WRITE -u8TempAdr;

		if(u16Len >= u8TempAdr )
		{
			g_I2C_Setting.u8I2CReadLength = u8TempAdr;
			u16Len -= u8TempAdr;
		}
		else
#endif
		{
			g_I2C_Setting.u8I2CReadLength = u16Len;
			u16Len = 0;
		}

		g_I2C_Setting.u8I2CWriteLength = 1;
		g_I2C_Setting.u8I2CMasterBuffer[0] = AT24C512_ADDR | RD_BIT;

		nsAPI_I2C_Engine();

		for (i=0;i<(g_I2C_Setting.u8I2CReadLength);i++)
		{
			*pBuf++= g_I2C_Setting.u8I2CSlaveBuffer[i];
#if (ENABLE_UART)
			//xprintf("\r\nRead=%x ,",I2CSlaveBuffer[i]);
			//xprintf("[%x]",i);
#endif
		}
		nsAPI_SSP_Sensor_Delsyus(6);//5us
	}
	pBuf=pBufTmp;

   #if (DISABLE_I2C)
	LPC_I2C->CONSET &= ~I2CONSET_I2EN;  
   	NVIC_DisableIRQ(I2C_IRQn);
   #endif
 	
}
#endif

