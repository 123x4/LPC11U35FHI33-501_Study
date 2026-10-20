
#define __SSP_C__

#include "LPC11Uxx.h"
#include "Mouse.h"
#include "ssp.h"
#include "uart.h"
#include "Main.h"
#include "Timer16.h" //(Add) 20120503
#include "Timer32.h"
//#include "I2c.h"
//#include "ADNS9800_SROM_Table.h"
#include "MouseDatabase.h"
#include "Systick.h"
#include "usb.h"
#include "usbcore.h"


#if 0
extern  LPC11Uxx_DATABASE_MOUSE_GENSETTING g_dMouse;

//extern U8	R_SensorBurst[6];

/*****************************************************************************
** Function name:		SSP0_IRQHandler
**
** Descriptions:		SSP port is used for SPI communication.
**						SSP interrupt handler
**						The algorithm is, if RXFIFO is at least half full,
**						start receive until it's empty; if TXFIFO is at least
**						half empty, start transmit until it's full.
**						This will maximize the use of both FIFOs and performance.
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void SSP0_IRQHandler(void)
{

}

/*****************************************************************************
** Function name:		SSP1_IRQHandler
**
** Descriptions:		SSP port is used for SPI communication.
**						SSP interrupt handler
**						The algorithm is, if RXFIFO is at least half full,
**						start receive until it's empty; if TXFIFO is at least
**						half empty, start transmit until it's full.
**						This will maximize the use of both FIFOs and performance.
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void SSP1_IRQHandler(void)
{

}

/**************************************************************************/
/*!
    @brief Initialise SSP0
*/
/**************************************************************************/
void nsAPI_SSP0_Init(void)
{
	U8 i, Dummy = Dummy;

	NVIC_DisableIRQ(SSP0_IRQn);

	LPC_SYSCON->PRESETCTRL |= BIT0;
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT11;			/* Enable AHB clock to the SSP domain*/
	//LPC_SYSCON->SSP1CLKDIV = SCB_CLKDIV_DIV10;	/* Divide by 2 for SSPCLKDIV */
	LPC_SYSCON->SSP0CLKDIV = SCB_CLKDIV_DIV2;	/* Divide by 2 for SSPCLKDIV */	
	
#if 0
	/* Set SSEL1 to GPIO P1.19  */
	LPC_IOCON->PIO1_19 &= ~0x07;
	LPC_IOCON->PIO1_19 |= BIT4;
#endif

	/* Set 1.29 to SSP SCK */
	// No LPC_IOCON SCKLOC register on LPC11U24?
	//LPC_IOCON->PIO1_29 = BIT0;
	GPIOSetIOConfig(SENSOR_SCK_PORT, SENSOR_SCK_PIN, 1, IOCON_MODE_INACT); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 

	/* Set P0.08 to SSP MISO */
	//LPC_IOCON->PIO0_8 &= ~0x07;
	//LPC_IOCON->PIO0_8 |= BIT0;
	GPIOSetIOConfig(SENSOR_MISO_PORT, SENSOR_MISO_PIN, 1, IOCON_MODE_INACT); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 

	/* Set P0.09 to SSP MOSI */
	//LPC_IOCON->PIO0_9 &= ~0x07;
	//LPC_IOCON->PIO0_9 |= BIT0;
	GPIOSetIOConfig(SENSOR_MOSI_PORT, SENSOR_MOSI_PIN, 1, IOCON_MODE_INACT); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	
	// Set NCS to GPIO P0.2  
	//LPC_IOCON->PIO0_2 &= ~0x07;
	//LPC_IOCON->PIO0_2 |= BIT4;
	GPIOSetIOConfig(SENSOR_NCS_PORT, SENSOR_NCS_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(SENSOR_NCS_PORT, SENSOR_NCS_PIN, GPIOOUT);

	// Set MOTION to GPIO P1.15
	//LPC_IOCON->PIO0_19 &= ~0x07;
	//LPC_IOCON->PIO0_19 |= BIT4;
	GPIOSetIOConfig(SENSOR_MOTION_PORT, SENSOR_MOTION_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(SENSOR_MOTION_PORT, SENSOR_MOTION_PIN, GPIOIN);

	// Set NRESET to GPIO P0.22
	//LPC_IOCON->PIO0_22 &= ~0x07;
	//LPC_IOCON->PIO0_22 |= BIT4;
	GPIOSetIOConfig(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, GPIOOUT);
	NDrv_GPIO_SetBitValue(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, HIGH); // Set NRESET high
	

	/* (PCLK / (CPSDVSR * [SCR+1])) = (48,000,000 / (2 * [11 + 1])) = 2.0 MHz */
	LPC_SSP0->CR0 = ((BIT2|BIT1|BIT0)// Data size = 8-bit  (bits 3:0)
	| BIT6// CPOL = 1(bits 6 + 7)
	| BIT7// CPHA = 1(bit 7)
	//| SSP_SCR_11);			// Clock rate = 5(bits 15:8)		// 1Mhz
	| SSP_SCR_5);			// Clock rate = 5(bits 15:8)		//2Mhz
	//SSP_SCR_23);			// Clock rate = 5(bits 15:8)	
	//LPC_SSP0->CR0 = ( /*SSP_SCR_11*/ SSP_SCR_5 | BIT7 | BIT6 | (BIT2|BIT1|BIT0) ); 
	/* Clock prescale register must be even and at least 2 in master mode */
	//LPC_SSP1->CPSR = 2;
	LPC_SSP0->CPSR = 2;

	/* Clear the Rx FIFO */
	for(i = 0; i < SSP_FIFOSIZE; i++)
	Dummy = LPC_SSP0->DR;

	NVIC_EnableIRQ(SSP0_IRQn);

	/* Enable device and set it to master mode, no loopback */
	//LPC_SSP1->CR1 = SSP_CR1_SSE_ENABLED | SSP_CR1_MS_MASTER | SSP_CR1_LBM_NORMAL;
	LPC_SSP0->CR1 = SSP_CR1_SSE_ENABLED | SSP_CR1_MS_MASTER | SSP_CR1_LBM_NORMAL;
	//NDrv_GPIO_SetBitValue(PORT0,2,HIGH); // Set NCS high
	NCS_HIGH();


	//NDrv_SSP_ADNS9800_SSEL(HIGH);
	//nsAPI_SSP_ADNS9800_Init();
	
}
/**************************************************************************/
/*!
    @brief Initialise SSP1
*/
/**************************************************************************/
void nsAPI_SSP1_Init(void)
{
	U8 i, Dummy = Dummy;

	NVIC_DisableIRQ(SSP1_IRQn);

	LPC_SYSCON->PRESETCTRL |= BIT2;
	LPC_SYSCON->SYSAHBCLKCTRL |= BIT18;			/* Enable AHB clock to the SSP domain*/
	//LPC_SYSCON->SSP1CLKDIV = SCB_CLKDIV_DIV10;	/* Divide by 2 for SSPCLKDIV */
	LPC_SYSCON->SSP1CLKDIV = SCB_CLKDIV_DIV2;	/* Divide by 2 for SSPCLKDIV */	
	
#if 0
	/* Set SSEL1 to GPIO P1.19  */
	LPC_IOCON->PIO1_19 &= ~0x07;
	LPC_IOCON->PIO1_19 |= BIT4;
#endif

	/* Set 1.29 to SSP SCK */
	// No LPC_IOCON SCKLOC register on LPC11U24?
	LPC_IOCON->PIO1_15 = 0x03;
	//SENSOR_SCK_PIN_CONFIG = BIT0;

	/* Set P0.08 to SSP MISO */
	LPC_IOCON->PIO0_22 &= ~0x07;
	LPC_IOCON->PIO0_22 |= 0x03;
	//SENSOR_MISO_PIN_CONFIG &= ~0x07;
	//SENSOR_MISO_PIN_CONFIG |= BIT0;

	/* Set P0.09 to SSP MOSI */
	LPC_IOCON->PIO0_21 &= ~0x07;
	LPC_IOCON->PIO0_21 |= 0x02;
	//SENSOR_MOSI_PIN_CONFIG &= ~0x07;
	//SENSOR_MOSI_PIN_CONFIG |= BIT0;

	// Set NCS to GPIO P0.2  
	//LPC_IOCON->PIO0_2 &= ~0x07;
	//LPC_IOCON->PIO0_2 |= BIT4;
	GPIOSetIOConfig(SENSOR_NCS_PORT, SENSOR_NCS_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(SENSOR_NCS_PORT, SENSOR_NCS_PIN, GPIOOUT);

	// Set MOTION to GPIO P0.19
	//LPC_IOCON->PIO0_19 &= ~0x07;
	//LPC_IOCON->PIO0_19 |= BIT4;
	GPIOSetIOConfig(SENSOR_MOTION_PORT, SENSOR_MOTION_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(SENSOR_MOTION_PORT, SENSOR_MOTION_PIN, GPIOIN);

	// Set NRESET to GPIO P0.22
	LPC_IOCON->PIO0_8 &= ~0x07;
	LPC_IOCON->PIO0_8 |= BIT4;
	NDrv_GPIO_SetDir(PORT0, 8, GPIOOUT);
	NDrv_GPIO_SetBitValue(PORT0, 8, HIGH); // Set NRESET high
	//LPC_IOCON->PIO0_22 &= ~0x07;
	//LPC_IOCON->PIO0_22 |= BIT4;
	//SENSOR_NRESET_PIN_CONFIG &= ~0x07;
	//SENSOR_NRESET_PIN_CONFIG |= BIT4;
	//NDrv_GPIO_SetDir(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, GPIOOUT);
	//NDrv_GPIO_SetBitValue(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, HIGH); // Set NRESET high
	

	/* (PCLK / (CPSDVSR * [SCR+1])) = (48,000,000 / (2 * [11 + 1])) = 2.0 MHz */
	//LPC_SSP1->CR0 = ((BIT2|BIT1|BIT0)// Data size = 8-bit  (bits 3:0)
	//| BIT6// CPOL = 1(bits 6 + 7)
	//| BIT7// CPHA = 1(bit 7)
	//| SSP_SCR_11);			// Clock rate = 5(bits 15:8)		// 1Mhz
	//| SSP_SCR_5);			// Clock rate = 5(bits 15:8)		//2Mhz
	//| SSP_SCR_23);			// Clock rate = 5(bits 15:8)	
	LPC_SSP1->CR0 = ( /*SSP_SCR_11*/ SSP_SCR_5 | BIT7 | BIT6 | (BIT2|BIT1|BIT0) ); 
	/* Clock prescale register must be even and at least 2 in master mode */
	//LPC_SSP1->CPSR = 2;
	LPC_SSP1->CPSR = 2;

	/* Clear the Rx FIFO */
	for(i = 0; i < SSP_FIFOSIZE; i++)
	Dummy = LPC_SSP1->DR;

	NVIC_EnableIRQ(SSP1_IRQn);

	/* Enable device and set it to master mode, no loopback */
	//LPC_SSP1->CR1 = SSP_CR1_SSE_ENABLED | SSP_CR1_MS_MASTER | SSP_CR1_LBM_NORMAL;
	LPC_SSP1->CR1 = SSP_CR1_SSE_ENABLED | SSP_CR1_MS_MASTER | SSP_CR1_LBM_NORMAL;
	//NDrv_GPIO_SetBitValue(PORT0,2,HIGH); // Set NCS high
	NCS_HIGH();	
}
/**************************************************************************/
/*!
    @brief Sends a block of data using SSP0

    @param[in]  buf
                Pointer to the data buffer
    @param[in]  length
                Block length of the data buffer
*/
/**************************************************************************/
void nsAPI_SSP0_Send(U8 *buf, U16 length)
{
	U8 Dummy = Dummy;

	/* Move on only if NOT busy and TX FIFO not full. */
	while((LPC_SSP0->SR & (SSP_SR_TNF_NOTFULL | SSP_SR_BSY_BUSY)) != SSP_SR_TNF_NOTFULL){}
	LPC_SSP0->DR = *buf;

	while((LPC_SSP0->SR & (SSP_SR_BSY_BUSY|SSP_SR_RNE_NOTEMPTY)) != SSP_SR_RNE_NOTEMPTY){}
	/* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO
	on MISO. Otherwise, when sspReceive is called, previous data byte
	is left in the FIFO. */
	Dummy = LPC_SSP0->DR;
}
/**************************************************************************/
/*!
    @brief Sends a block of data using SSP0

    @param[in]  buf
                Pointer to the data buffer
    @param[in]  length
                Block length of the data buffer
*/
/**************************************************************************/
void nsAPI_SSP1_Send(U8 *buf, U16 length)
{
	U8 Dummy = Dummy;

	/* Move on only if NOT busy and TX FIFO not full. */
	while((LPC_SSP1->SR & (SSP_SR_TNF_NOTFULL | SSP_SR_BSY_BUSY)) != SSP_SR_TNF_NOTFULL){}
	LPC_SSP1->DR = *buf;

	while((LPC_SSP1->SR & (SSP_SR_BSY_BUSY|SSP_SR_RNE_NOTEMPTY)) != SSP_SR_RNE_NOTEMPTY){}
	/* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO
	on MISO. Otherwise, when sspReceive is called, previous data byte
	is left in the FIFO. */
	Dummy = LPC_SSP1->DR;
}
/**************************************************************************/
/*!
    @brief Receives a block of data using SSP0

    @param[in]  buf
                Pointer to the data buffer
    @param[in]  length
                Block length of the data buffer
*/
/**************************************************************************/
void nsAPI_SSP0_Receive(U8 *buf, U16 length)
{
	/* As long as the receive FIFO is not empty, data can be received. */
	LPC_SSP0->DR = 0xFF;

	/* Wait until the Busy bit is cleared */
	while((LPC_SSP0->SR & (SSP_SR_BSY_BUSY|SSP_SR_RNE_NOTEMPTY)) != SSP_SR_RNE_NOTEMPTY ){}
	*buf = LPC_SSP0->DR;
}
/**************************************************************************/
/*!
    @brief Receives a block of data using SSP0

    @param[in]  buf
                Pointer to the data buffer
    @param[in]  length
                Block length of the data buffer
*/
/**************************************************************************/
void nsAPI_SSP1_Receive(U8 *buf, U16 length)
{
	/* As long as the receive FIFO is not empty, data can be received. */
	LPC_SSP1->DR = 0xFF;

	/* Wait until the Busy bit is cleared */
	while((LPC_SSP1->SR & (SSP_SR_BSY_BUSY|SSP_SR_RNE_NOTEMPTY)) != SSP_SR_RNE_NOTEMPTY ){}
	*buf = LPC_SSP1->DR;
}

void nsAPI_SSP_Sensor_Delsyms(U16 u16DelayTime)
{
	while(u16DelayTime > 0)
	{
		nsAPI_SSP_Sensor_Delsyus(1000);/*1ms*/
		u16DelayTime --;
	}
}
#endif

/**
If n = 1 -> 2.14
If n = 2 -> 2.14+1.18x1
If n = 3 -> 2.14+1.18x2
If ....

**/
void nsAPI_SSP_Sensor_Delsyus(U16 u16DelayTime)
{
	//U8 i;
	//base on 2us
	while(u16DelayTime)
	{
		//for (i = 2; i > 0; i--)
		//{					 //if use Optimization then not use "for loop" for Delay time
		
		__NOP();
    __NOP();
    __NOP();
		__NOP();
		
		__NOP();
    __NOP();
    __NOP();
		__NOP();
		
		__NOP();
    __NOP();
    __NOP();
		__NOP();
		
		__NOP();
    __NOP();
		u16DelayTime --;
	};
}

