//***********************************************************************
//*Writer						ADAM									*
//*Function						GPIO									*
//***********************************************************************
//***********************************************************************
//*								INCLUDE									*
//***********************************************************************
#include "../inc/LPC11Uxx.h"
//#include "../inc/device.h"
//#include "../inc/OSC.h"
//#include "../inc/GPIO.h"
#include "usbhw.h"
#include "usbcore.h"
#include "gloab.h"
//***********************************************************************
//*								REGISTER								*
//***********************************************************************

//***********************************************************************
//*								DECLARE									*
//***********************************************************************
extern uint8_t  sus_res;
extern UINT16 latency_count;
extern BOOL latency_key_trigger;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					GOIO							~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InitGPIO(void) ;
void DrvGPIOInput(unsigned char Port ,unsigned char Set) ;
void DrvGPIOOutput(unsigned char Port ,unsigned char Set , unsigned char HL) ;
unsigned char DrvGPIORead(unsigned char Port ,unsigned char Set) ;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					FLEX							~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InitGPIOFlexInterrupt(unsigned char ChannelNum , unsigned char PortNum , unsigned char BitPosi , unsigned char Trig , unsigned char Event ) ;
void GPIOFlexIntEnable(unsigned char ChannelNum , unsigned char Event ) ;
void GPIOFlexIntDisable(unsigned char ChannelNum , unsigned char Event ) ;
unsigned char GPIOFlexIntStatus(unsigned char ChannelNum ) ;
void GPIOFlexIntClear(unsigned char ChannelNum ) ;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					GROUP							~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InitGPIOGroupedInterrupt(unsigned char GroupNum , unsigned int *bitPattern, unsigned char Logic , unsigned char Trig , unsigned int *eventPattern ) ;
//***********************************************************************
//*								INTERRUPT_FUNCTION						*
//***********************************************************************
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					FLEX							~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-----FLEX_INT0_IRQHandler-----
void FLEX_INT0_IRQHandler(void)
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<0) )							//Check This interrupt Requested
	{
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<0) )					//Level Trig
		{

		}
		else														//Edge Trig
		{
			if ( LPC_GPIO_PIN_INT->RISE & (0x1<<0) )				//Rising Trig
			{
				LPC_GPIO_PIN_INT->RISE = 0x1<<0 ;
			}
			else if ( LPC_GPIO_PIN_INT->FALL & (0x1<<0) )			//Falling Trig
			{
				LPC_GPIO_PIN_INT->FALL = 0x1<<0 ;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<0 ;						//Clear interrupt Requested
		}
	}
	return ;
}
//-----FLEX_INT1_IRQHandler-----
void FLEX_INT1_IRQHandler(void)
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<1) )
	{
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<1) )
		{
		}
		else
		{
			if ( LPC_GPIO_PIN_INT->RISE & (0x1<<1) )
			{
				LPC_GPIO_PIN_INT->RISE = 0x1<<1 ;
			}
			else if ( LPC_GPIO_PIN_INT->FALL & (0x1<<1) )
			{
				LPC_GPIO_PIN_INT->FALL = 0x1<<1 ;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<1 ;
		}
	}		
	return ;
}
//-----FLEX_INT2_IRQHandler-----
void FLEX_INT2_IRQHandler(void)
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<2) )
	{
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<2) )
		{
		}
		else
		{
			if ( LPC_GPIO_PIN_INT->RISE & (0x1<<2) )
			{
				LPC_GPIO_PIN_INT->RISE = 0x1<<2 ;
			}
			else if ( LPC_GPIO_PIN_INT->FALL & (0x1<<2) )
			{
				LPC_GPIO_PIN_INT->FALL = 0x1<<2 ;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<2 ;
		}
	}		
	return ;
}
//-----FLEX_INT3_IRQHandler-----
void FLEX_INT3_IRQHandler(void)
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<3) )
	{
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<3) )
		{
		}
		else
		{
			if ( LPC_GPIO_PIN_INT->RISE & (0x1<<3) )
			{
				LPC_GPIO_PIN_INT->RISE = 0x1<<3 ;
			}
			else if ( LPC_GPIO_PIN_INT->FALL & (0x1<<3) )
			{
				LPC_GPIO_PIN_INT->FALL = 0x1<<3 ;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<3 ;
		}
	}
	return ;
}
//-----FLEX_INT4_IRQHandler-----
void FLEX_INT4_IRQHandler(void)
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<4) )
	{
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<4) )
		{
		}
		else
		{
			if ( LPC_GPIO_PIN_INT->RISE & (0x1<<4) )
			{
				LPC_GPIO_PIN_INT->RISE = 0x1<<4 ;
			}
			else if ( LPC_GPIO_PIN_INT->FALL & (0x1<<4) )
			{
				LPC_GPIO_PIN_INT->FALL = 0x1<<4 ;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<4 ;
		}
	}		
	return ;
}
//-----FLEX_INT5_IRQHandler-----
void FLEX_INT5_IRQHandler(void)
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<5) )
	{
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<5) )
		{
		}
		else
		{
			if ( LPC_GPIO_PIN_INT->RISE & (0x1<<5) )
			{
				LPC_GPIO_PIN_INT->RISE = 0x1<<5 ;
			}
			else if ( LPC_GPIO_PIN_INT->FALL & (0x1<<5) )
			{
				LPC_GPIO_PIN_INT->FALL = 0x1<<5 ;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<5 ;
		}
	}		
	return ;
}
//-----FLEX_INT6_IRQHandler-----
void FLEX_INT6_IRQHandler(void)
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<6) )
	{
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<6) )
		{
		}
		else
		{
			if ( LPC_GPIO_PIN_INT->RISE & (0x1<<6) )
			{
				LPC_GPIO_PIN_INT->RISE = 0x1<<6 ;
			}
			else if ( LPC_GPIO_PIN_INT->FALL & (0x1<<6) )
			{
				LPC_GPIO_PIN_INT->FALL = 0x1<<6 ;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<6 ;
		}
	}
	return ;
}
//-----FLEX_INT7_IRQHandler-----
void FLEX_INT7_IRQHandler(void)
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<7) )
	{
		if ( LPC_GPIO_PIN_INT->ISEL & (0x1<<7) )
		{
		}
		else
		{
			if ( LPC_GPIO_PIN_INT->RISE & (0x1<<7) )
			{
				LPC_GPIO_PIN_INT->RISE = 0x1<<7 ;
			}
			else if ( LPC_GPIO_PIN_INT->FALL & (0x1<<7) )
			{
				LPC_GPIO_PIN_INT->FALL = 0x1<<7 ;
			}
			LPC_GPIO_PIN_INT->IST = 0x1<<7 ;
		}
	}		
	return ;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					GROUP							~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-----GINT0_IRQHandler-----
void GINT0_IRQHandler(void)
{
	if ( LPC_GPIO_GROUP_INT0->CTRL & 0x1 )
	{
		if ( LPC_GPIO_GROUP_INT0->CTRL & (0x1<<4) )
		{
		}
		else
		{
		}
//		 if(g_USBCore_Setting.Enumeration_Finish == FALSE)
//		 if( (g_USBCore_Setting.USB_Device_Status & USB_DEVICE_STATUS_SUSPEND)==
//		if(( g_USBCore_Setting.USB_DeviceStatus & USB_GETSTATUS_REMOTE_WAKEUP)==\
//			 USB_GETSTATUS_REMOTE_WAKEUP)

		if(suspendFlag==1)
		{
			g_USBCore_Setting.USB_Device_Status|= USB_DEVICE_STATUS_RESUME;
			SuspendFlag = 1;
			suspendFlag=0;
			sus_res=1; 
//			NVIC_EnableIRQ(I2C_IRQn);
			NVIC_EnableIRQ(SSP1_IRQn);
			NVIC_EnableIRQ(UART_IRQn);
			NVIC_EnableIRQ(TIMER_16_0_IRQn);
			NVIC_EnableIRQ(SysTick_IRQn);
		}
        else
        {
            latency_count = 0;
            latency_key_trigger = TRUE;
        }
	
//		g_USBCore_Setting.USB_DeviceStatus |= USB_GETSTATUS_REMOTE_WAKEUP;
		LPC_GPIO_GROUP_INT0->CTRL |= 0x1;
	}		
	return ;
}
//-----GINT1_IRQHandler-----
void GINT1_IRQHandler(void)
{
	if ( LPC_GPIO_GROUP_INT1->CTRL & 0x1 )
	{
		if ( LPC_GPIO_GROUP_INT1->CTRL & (0x1<<4) )
		{
		}
		else
		{
		}
//		 if(g_USBCore_Setting.Enumeration_Finish == FALSE)
//		 {
//		if( (g_USBCore_Setting.USB_Device_Status & USB_DEVICE_STATUS_SUSPEND)==\
//			 USB_DEVICE_STATUS_SUSPEND)
//		if(( g_USBCore_Setting.USB_DeviceStatus & USB_GETSTATUS_REMOTE_WAKEUP)==\
//			 USB_GETSTATUS_REMOTE_WAKEUP)
		{
		g_USBCore_Setting.USB_Device_Status|= USB_DEVICE_STATUS_RESUME;
		}
//		 }
//		g_USBCore_Setting.USB_DeviceStatus |= USB_GETSTATUS_REMOTE_WAKEUP;
		LPC_GPIO_GROUP_INT1->CTRL |= 0x1;
	}		
	return;
}
//***********************************************************************
//*								SUB_FUNCTION							*
//***********************************************************************
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					GOIO							~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-----InitGPIO-----
void InitGPIO(void)
{
    U32 ENA[2] = {0x00000080,0x00000000};
    U32 POL[2] = {0x00000000,0x00000000};
    
	LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<6)|(1<<19)|(1<<23)|(1<<24)) ;							//Setup AHB Clock To The GPIO Domain
    //-----IOCON Init-----
//	LPC_IOCON->TDI_PIO0_11 &= ~0x07 ;
//	LPC_IOCON->TDI_PIO0_11 |= 0x01 ;    							//GPIO
	LPC_IOCON->TMS_PIO0_12 &= ~0x07 ;
	LPC_IOCON->TMS_PIO0_12 |= 0x01 ;                                //GPIO
	LPC_IOCON->TDO_PIO0_13 &= ~0x07 ;
	LPC_IOCON->TDO_PIO0_13 |= 0x01 ;								//GPIO
	LPC_IOCON->TRST_PIO0_14 &= ~0x07 ;
	LPC_IOCON->TRST_PIO0_14 |= 0x01 ;								//GPIO
	LPC_IOCON->PIO0_16 &= ~0x07 ;
//	LPC_IOCON->PIO0_16 |= 0x01 ;								//GPIO

	
//	DrvGPIOOutput(0,16,0) ;
//	DrvGPIOOutput(0,13,0) ;
//	DrvGPIOOutput(0,14,0) ;
//	//-----GPIO_Input-----
//      DrvGPIOInput(0,11) ;  
      DrvGPIOInput(0,2) ;
//	    DrvGPIOInput(0,23) ;
//      DrvGPIOInput(0,16) ;
//	    DrvGPIOInput(0,19) ;
//	    DrvGPIOInput(0,7) ;
//      DrvGPIOInput(0,14) ;
//	    DrvGPIOInput(0,17) ;
//	    DrvGPIOInput(0,18) ;
//	    DrvGPIOInput(0,1) ;
//		  DrvGPIOInput(0,12) ;
//	    DrvGPIOInput(0,13) ;
//    DrvGPIOInput(0,8) ;
//    DrvGPIOInput(0,16) ;    
//	//-----GPIO_Output-----
//      DrvGPIOOutput(1,27,1) ;
//      DrvGPIOOutput(1,21,1) ;
//      DrvGPIOOutput(0,12,1) ;
//      DrvGPIOOutput(0,11,1) ;
//      DrvGPIOOutput(0,23,1) ;

//      DrvGPIOOutput(0,16,0) ;
//			DrvGPIOOutput(0,13,0) ;
//			DrvGPIOOutput(0,14,0) ;
//			DrvGPIOOutput(0,23,1) ;
//	    DrvGPIOOutput(0,19,1) ;
//	    DrvGPIOOutput(0,7,1) ;
//      DrvGPIOOutput(0,14,0) ;
//	    DrvGPIOOutput(0,17,1) ;
//	    DrvGPIOOutput(0,18,1) ;
//	    DrvGPIOOutput(0,1,1) ;
//		  DrvGPIOOutput(0,12,0) ;

//			DrvGPIOOutput(0,3, 1) ;
    InitGPIOGroupedInterrupt(0,&ENA[0],FALSE,0,&POL[0]); 
	return ;
}
//-----DrvGPIOInput-----
void DrvGPIOInput(unsigned char Port ,unsigned char Set)
{
	LPC_GPIO->DIR[Port] &= ~(1<<Set) ;								//Set to Input
	return ;
}
//-----DrvGPIOOutput-----
void DrvGPIOOutput(unsigned char Port ,unsigned char Set , unsigned char HL)
{
	LPC_GPIO->DIR[Port] |= (1<<Set) ;								//Set to Output
	if ( HL )
	{
		LPC_GPIO->SET[Port] = 1<<Set ;
	}
	else
	{
		LPC_GPIO->CLR[Port] = 1<<Set ;
	}
	return ;
}
//-----DrvGPIORead-----
unsigned char DrvGPIORead(unsigned char Port ,unsigned char Set)
{
	unsigned char RegVal ;
	unsigned long Data ;
	Data = (LPC_GPIO->PIN[Port] >> Set) ;
	Data = (Data & 0x01) ; 
	if( Data == 1)
	{
		RegVal = 1 ;
	}
	else
	{
		RegVal = 0 ;
	}
	return ( RegVal );	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					FLEX							~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-----InitGPIOFlexInterrupt-----
void InitGPIOFlexInterrupt(unsigned char ChannelNum , unsigned char PortNum , unsigned char BitPosi , unsigned char Trig , unsigned char Event )
{
	switch ( ChannelNum )
	{
		case 0 :													//Use Channel 0 
            //Pin number select for pin interrupt
            //PIO0_0 to PIO0_23 correspond to numbers 0 to 23
            //PIO1_0 to PIO1_31 correspond to numbers 24 to 55
			if (PortNum==1)											//PORT 1
			{
				LPC_SYSCON->PINTSEL[0] = BitPosi + 24 ; 
			}
			else													//PORT 0
			{
				LPC_SYSCON->PINTSEL[0] = BitPosi ; 
			}
			NVIC_EnableIRQ(FLEX_INT0_IRQn) ;
		break ;
		case 1 :													//Use Channel 1
			if (PortNum==1)											//PORT1
			{
				LPC_SYSCON->PINTSEL[1] = BitPosi + 24 ; 
			}
			else													//PORT0
			{
				LPC_SYSCON->PINTSEL[1] = BitPosi ; 
			}
			NVIC_EnableIRQ(FLEX_INT1_IRQn) ;
		break ;
		case 2 :													//Use Channel 2
			if (PortNum==1)											//PORT 1
			{
				LPC_SYSCON->PINTSEL[2] = BitPosi + 24 ; 
			}
			else													//PORT 0
			{
				LPC_SYSCON->PINTSEL[2] = BitPosi ; 
			}
			NVIC_EnableIRQ(FLEX_INT2_IRQn) ;
		break ;
		case 3 :													//Use Channel 3
			if (PortNum==1)											//PORT 1
			{
				LPC_SYSCON->PINTSEL[3] = BitPosi + 24 ; 
			}
			else													//PORT 0
			{
				LPC_SYSCON->PINTSEL[3] = BitPosi ; 
			}
			NVIC_EnableIRQ(FLEX_INT3_IRQn) ;
		break;
		case 4 :													//Use Chanel 4
			if (PortNum==1)											//PORT 1
			{
				LPC_SYSCON->PINTSEL[4] = BitPosi + 24 ; 
			}
			else													//PORT 0
			{
				LPC_SYSCON->PINTSEL[4] = BitPosi ; 
			}
			NVIC_EnableIRQ(FLEX_INT4_IRQn) ;
		break ;
		case 5 :													//Use Channel 5
			if (PortNum==1)											//PORT 1
			{
				LPC_SYSCON->PINTSEL[5] = BitPosi + 24 ;
			}
			else													//PORT 0
			{
				LPC_SYSCON->PINTSEL[5] = BitPosi ; 
			}
			NVIC_EnableIRQ(FLEX_INT5_IRQn) ;
		break ;	
		case 6 :													//Use Channel 6
			if (PortNum==1)											//PORT 1
			{
				LPC_SYSCON->PINTSEL[6] = BitPosi + 24 ; 
			}
			else													//PORT 0
			{
				LPC_SYSCON->PINTSEL[6] = BitPosi ; 
			}
			NVIC_EnableIRQ(FLEX_INT6_IRQn) ;
		break ;
		case 7 :													//Use Channel 7
			if (PortNum==1)											//PORT 1
			{
				LPC_SYSCON->PINTSEL[7] = BitPosi + 24 ; 
			}
			else													//PORT 0
			{
				LPC_SYSCON->PINTSEL[7] = BitPosi ; 
			}
			NVIC_EnableIRQ(FLEX_INT7_IRQn) ;
		break ;
		default :
		break ;
	}
	if ( Trig == 0 )												//Edge Trig
	{
		LPC_GPIO_PIN_INT->ISEL &= ~(0x1<<ChannelNum) ;
		if ( Event == 0 )
		{
			LPC_GPIO_PIN_INT->IENF |= (0x1<<ChannelNum) ;			//Falling Edge
		}
		else
		{
			LPC_GPIO_PIN_INT->IENR |= (0x1<<ChannelNum) ;			//Rising Edge
		}
	}
	else															//Level Trig
	{
		LPC_GPIO_PIN_INT->ISEL |= (0x1<<ChannelNum) ;
		LPC_GPIO_PIN_INT->IENR |= (0x1<<ChannelNum) ;				//Enable Level Trig
		if ( Event == 0 )
		{
			LPC_GPIO_PIN_INT->IENF &= ~(0x1<<ChannelNum) ;			//Low Trig
		}
		else
		{
			LPC_GPIO_PIN_INT->IENF |= (0x1<<ChannelNum) ;			//High Trig
		}
	}
	return ;
}
//-----
void GPIOFlexIntEnable(unsigned char ChannelNum , unsigned char Event )
{
	if ( !( LPC_GPIO_PIN_INT->ISEL & (0x1<<ChannelNum) ) )
	{
		if ( Event == 0 )
		{
			LPC_GPIO_PIN_INT->SIENF |= (0x1<<ChannelNum);	/* faling edge */
		}
		else
		{
			LPC_GPIO_PIN_INT->SIENR |= (0x1<<ChannelNum);	/* Rising edge */
		}
	}
	else
	{
		LPC_GPIO_PIN_INT->SIENR |= (0x1<<ChannelNum);	/* Level */
	}
	return ;
}
//-----
void GPIOFlexIntDisable(unsigned char ChannelNum , unsigned char Event )
{
	if ( !( LPC_GPIO_PIN_INT->ISEL & (0x1<<ChannelNum) ) )
	{
		if ( Event == 0 )
		{
			LPC_GPIO_PIN_INT->CIENF |= (0x1<<ChannelNum);	/* faling edge */
		}
		else
		{
			LPC_GPIO_PIN_INT->CIENR |= (0x1<<ChannelNum);	/* Rising edge */
		}
	}
	else
	{
		LPC_GPIO_PIN_INT->CIENR |= (0x1<<ChannelNum);	/* Level */
	}
	return ;
}
//-----
unsigned char GPIOFlexIntStatus(unsigned char ChannelNum )
{
	if ( LPC_GPIO_PIN_INT->IST & (0x1<<ChannelNum) )
	{
		return( 1 ) ;
	}
	else
	{
		return( 0 ) ;
	}
}
//-----
void GPIOFlexIntClear(unsigned char ChannelNum )
{
	if ( !( LPC_GPIO_PIN_INT->ISEL & (0x1<<ChannelNum) ) )
	{
		LPC_GPIO_PIN_INT->IST = (1<<ChannelNum) ;
	}
	return ;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~					GROUP							~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InitGPIOGroupedInterrupt(unsigned char GroupNum , unsigned int *bitPattern, unsigned char Logic , unsigned char Trig , unsigned int *eventPattern )
{
	switch ( GroupNum )
	{
		case 0 :													//Group 0
			if ( Trig == 0 )										//Edge Trig
			{
				LPC_GPIO_GROUP_INT0->CTRL &=~ (0x1<<2) ;
			}
			else													//Level Trig
			{
				LPC_GPIO_GROUP_INT0->CTRL |= (0x1<<2) ;
			}
			LPC_GPIO_GROUP_INT0->CTRL |= (Logic<<1) ;				//Single or Total Work

			LPC_GPIO_GROUP_INT0->PORT_POL[0] = *((uint32_t *)(eventPattern + 0)) ;
			LPC_GPIO_GROUP_INT0->PORT_POL[1] = *((uint32_t *)(eventPattern + 1)) ;
			LPC_GPIO_GROUP_INT0->PORT_ENA[0] = *((uint32_t *)(bitPattern + 0)) ;
			LPC_GPIO_GROUP_INT0->PORT_ENA[1] = *((uint32_t *)(bitPattern + 1)) ;
			/* as soon as enabled, an edge may be generated       */
			/* clear interrupt flag and NVIC pending interrupt to */
			/* workaround the potential edge generated as enabled */
			LPC_GPIO_GROUP_INT0->CTRL |= (1<<0) ;					//Clear Interrupt Flag

			NVIC_ClearPendingIRQ(GINT0_IRQn) ;
			NVIC_EnableIRQ(GINT0_IRQn) ;
		break ;
		case 1 :													//Group 1
			if ( Trig == 0 )										//Edge Tric
			{
				LPC_GPIO_GROUP_INT1->CTRL &=~ (0x1<<2) ;
			}
			else													//Level Tric
			{
				LPC_GPIO_GROUP_INT1->CTRL |= (0x1<<2) ;
			}

			LPC_GPIO_GROUP_INT1->CTRL |= (Logic<<1) ;				//Single or Total Work

			LPC_GPIO_GROUP_INT1->PORT_POL[0] = *((uint32_t *)(eventPattern + 0)) ;
			LPC_GPIO_GROUP_INT1->PORT_POL[1] = *((uint32_t *)(eventPattern + 1)) ;
			LPC_GPIO_GROUP_INT1->PORT_ENA[0] = *((uint32_t *)(bitPattern + 0))  ;
			LPC_GPIO_GROUP_INT1->PORT_ENA[1] = *((uint32_t *)(bitPattern + 1)) ;
			/* as soon as enabled, an edge may be generated       */
			/* clear interrupt flag and NVIC pending interrupt to */
			/* workaround the potential edge generated as enabled */

			LPC_GPIO_GROUP_INT1->CTRL |= (1<<0) ;					//Clear Interrupt Flag
			NVIC_ClearPendingIRQ(GINT1_IRQn) ;
			NVIC_EnableIRQ(GINT1_IRQn) ;
		break ;
		default :
		break ;
	}
	return ;
}


//void set_pin_statu(uint8_t port , uint32_t pin_num , uint8_t hi)
//{
//	// set as GPIO first
//	
//	if (port == 0){   //  port 0
//		LPC_GPIO->DIR[0] |= 1<<pin_num ;  // set as output
//		if(hi == 1)  //  high
//		{
//			LPC_GPIO->B0[pin_num] |= 1 ;
//		}
//		else 
//		{
//			LPC_GPIO->B0[pin_num] &= ~1 ;
//		}
//  }
//	else              // port 1
//	{
//		LPC_GPIO->DIR[1] |= 1<<pin_num ;  // set as output
//		if(hi == 1)  // high
//		{
//			LPC_GPIO->B0[pin_num] |= 1 ;
//		}
//		else 
//		{
//			LPC_GPIO->B1[pin_num] &= ~0 ;
//		}

//	}
//}


//***********************************************************************
//*								END										*
//***********************************************************************
