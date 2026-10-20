//***********************************************************************
//*Writer						ADAM									*
//*Function						Main									*
//***********************************************************************
//***********************************************************************
//*								INCLUDE									*
//***********************************************************************
#define _MAIN_C_
#include "../inc/LPC11Uxx.h"
#include "../inc/Main.h"
#include "../inc/OSC.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbuser.h"
#include "hiduser.h"

//***********************************************************************
//*								REGISTER								*
//***********************************************************************
// 7 SEG Data
unsigned char Show_Status =0;
unsigned char Show8_Data =0;

unsigned int Data =0;
unsigned int A , B, C, D ;
unsigned int Show_Data = (20<<24 | 20<<16 | 20<<8 | 0) ;
//***********************************************************************
//*								DECLARE									*
//***********************************************************************
void DrvDelay(unsigned long DelayData) ;
//***********************************************************************
//*								MAIN_FUNCTION							*
//***********************************************************************
int main (void)
{
    //-----Setup Main Frequency-----
	InitSystemOSC( MAIN_SEL , PSEL , MSEL , SYS_AHB_DIV_VAL ) ;
	InitUSBOSC( USB_SEL , USB_PSEL , USB_MSEL , USB_DIV_VAL) ;
    //-----MCU Periphert Init-----
    InitGPIO( ) ;
    InitSPI1Master( ) ;
    InitCT16B0(TIME_CT16B0_PR ,TIME_CT16B0_COUNT ) ;                //7 SEG Time
                                                 //Setup SPI1 TO 74LVC595
    Show_Data = 0 ;
    Data = 0 ;
	while(1)
	{
        Data++ ;
        if(Data >=10000)
        {
            Data = 0 ;
        }
        A = Data/1000 ;
        if(A == 0)
        {
            A = 20 ;
        }
        B =  (Data%1000) /100 ;
        if(A == 20 && B==0)
        {
            B = 20 ;
        }
        C = (Data%100) /10 ;
        if(A == 20 && B==20 && C==0)
        {
            C = 20 ;
        }
        D = (Data%10) ;
        Show_Data = A<<24 | B<<16 | C<<8 | D ;
        DrvDelay(1000) ;
				
				
	}
}
//***********************************************************************
//*								INTERRUPT_FUNCTION						*
//***********************************************************************

//***********************************************************************
//*								SUB_FUNCTION							*
//***********************************************************************
//-----DrvDelay-----
void DrvDelay(unsigned long DelayData)
{
    unsigned int DelayLoop ;
    while(DelayData!=0)
    {
        DelayLoop = DELAY_COUNT ;
        while(DelayLoop!=0)
        {	  
            DelayLoop-- ;
            if(DelayLoop==0)
            {					 
                DelayData-- ;
            }
        }
    }
}
//***********************************************************************
//*								END										*
//***********************************************************************
