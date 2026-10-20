/****************************************************************************
 *   $Id:: uart.h 6172 2011-01-13 18:22:51Z usb00423                        $
 *   Project: NXP LPC11xx software example
 *
 *   Description:
 *     This file contains definition and prototype for UART configuration.
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
#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"

//#include "..\Src\F_M800.h"

#ifdef __UART_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif


#define RS485_ENABLED 0
#define TX_INTERRUPT 0/* 0 if TX uses polling, 1 interrupt driven. */

//#ifdef V_Uart_Print
#define ENABLE_UART 0
//#endif

#if (ENABLE_UART)
#define UART_USE_PIO1_13_14    0
#define UART_USE_PIO0_18_19    1 //XOR UART_USE_PIO1_13_14
#else
#define UART_USE_PIO1_13_14    0
#define UART_USE_PIO0_18_19    0
#endif

#if (ENABLE_UART)
#define IER_RBR BIT0
#define IER_THRE BIT1
#define IER_RLS BIT2

#define IIR_PEND (BIT0)
#define IIR_RLS (BIT1|BIT0)
#define IIR_RDA (BIT1)
#define IIR_CTI (BIT2|BIT1)
#define IIR_THRE (BIT0)

#define LSR_RDR BIT0
#define LSR_OE BIT1
#define LSR_PE BIT2
#define LSR_FE BIT3
#define LSR_BI BIT4
#define LSR_THRE BIT5
#define LSR_TEMT BIT6
#define LSR_RXFE BIT7

#define UARTBUFSIZE 20
#define BR 115200

/* RS485 mode definition. */
#define RS485_NMMEN BIT0
#define RS485_RXDIS BIT1
#define RS485_AADEN	BIT2
#define RS485_SEL BIT3
#define RS485_DCTRL BIT4
#define RS485_OINV BIT5


typedef __packed struct _LPC11Uxx_Uart_Setting_
{
	volatile U32 UartStatus;
	volatile U8 UartBuffer[UARTBUFSIZE];
	volatile U8 UartCount;
}LPC11Uxx_Uart_Setting;

INTERFACE LPC11Uxx_Uart_Setting g_Uart_Setting;

INTERFACE void nsAPI_UART_Init(uint32_t Baudrate);
INTERFACE void UART_IRQHandler(void);
INTERFACE void nsAPI_UART_Send(U8 *BufferPtr, U32 Length);
INTERFACE void xprintf(U8 *pFmt,U32 wVal,...);

//void S_Board_UARTPutSTR(char *str);

#endif

#undef INTERFACE
#ifdef __cplusplus
}
#endif
#endif /* end __UART_H */

