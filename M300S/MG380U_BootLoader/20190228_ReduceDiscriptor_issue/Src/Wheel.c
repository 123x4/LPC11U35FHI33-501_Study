#include <stdbool.h>
#include "LPC11Uxx.h"/* LPC11xx Peripheral Registers */
#include "Mouse.h"
#include "Main.h"
#include "gpio.h"
#include "Wheel.h"



#if 0
static const uint8_t ENCODE_STATUS_TABLE[]=
{
	0x00,          //0000
       0x01,          //0001       //D+ D=1
       0x02,          //0010       //D- D=2
       0x03,          //0011       //increase/decreas according Direction
       0x02,          //0100       //decrease
       0x00,          //0101
       0x03,          //0110       //increas/decrease according Direction
       0x01,          //0111       //increase
       0x01,          //1000       //increase
       0x03,          //1001       //increas/decrease according Direction
       0x00,          //1010
       0x02,          //1011       //drease
       0x03,          //1100       //increas/decrease according Direction
       0x02,          //1101       //D- D=2
       0x01,          //1110       //D+ D=1
       0x00           //1111
};

typedef struct
{
    	int8_t  Z_delta;
    	uint8_t last_state;
    	bool	ZA_f;
    	bool	ZB_f;
} wheel_status_t;
bool wheel_poll_en=TRUE;
static wheel_status_t wheel_state; 	//= { 0,0, false, false};
static uint8_t wheel_get_wheel_state(void);


void Wheel_Init(void)
{

	// Set Za & Zb to input and pull-ups if no extand pull up register
	GPIOSetIOConfig(WHEEL_ZA_PORT, WHEEL_ZA_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(WHEEL_ZA_PORT, WHEEL_ZA_PIN, GPIOIN);			// ZA Set as Input
	GPIOSetIOConfig(WHEEL_ZB_PORT, WHEEL_ZB_PIN, 0, IOCON_MODE_PULLUP); //(IOCON_MODE_INACT/IOCON_MODE_PULLDOWN/IOCON_MODE_PULLUP) 
	NDrv_GPIO_SetDir(WHEEL_ZB_PORT, WHEEL_ZB_PIN, GPIOIN);			// ZB Set as Input

#ifdef 	ZCOM_ENABLE
	// set Zcomm to output and low level.	
	NDrv_GPIO_SetDir(ZCOM_PORT, ZCOM_PIN, GPIOOUT);			// ZA Set as Input
	LPC_IOCON->PIO0_xx &= ~0x07;			
	LPC_IOCON->PIO0_xx |= BIT4;
#endif

	// delay 30 uS
	S_Idle_Delay_uSec(30);	

	//get current I/O status to the status buffer.
	wheel_state.Z_delta = 0;
	wheel_state.ZA_f = false;
	wheel_state.ZB_f = false;
	wheel_get_wheel_state();
	wheel_poll_en = true;
}

static uint8_t wheel_get_wheel_state(void)
{
    	uint8_t level=0;
		
    	// Read wheel A, B voltage levels in
#ifndef INVERSE_ZA_ZB			// if ZA ZB inverse
	if(NDrv_GPIO_GetPinValue(WHEEL_ZA_PORT, WHEEL_ZA_PIN))	// ZA is high?
    		level |= 0x01;
	if(NDrv_GPIO_GetPinValue(WHEEL_ZB_PORT, WHEEL_ZB_PIN))	// ZB is high?
		level |= 0x02;
#else
	if(NDrv_GPIO_GetPinValue(WHEEL_ZB_PORT, WHEEL_ZB_PIN))	// ZB is high?
		level |= 0x01;
    	if(NDrv_GPIO_GetPinValue(WHEEL_ZA_PORT, WHEEL_ZA_PIN))	// ZA is high?
		level |= 0x02;
#endif

	//update new wheel data 
	wheel_state.last_state = (wheel_state.last_state << 2) & 0xFC;
	wheel_state.last_state =	(level & 0x03) | wheel_state.last_state;

	//return check data	
    	return (wheel_state.last_state & 0x0F);
}

//--------------------------------------------------------------------------
// wheel_get_report
//--------------------------------------------------------------------------

bool wheel_get_report(int8_t* report)			// get Z data back, if data is not zero, get true
{
	int8_t tmp = wheel_state.Z_delta;
	
    	*report = tmp;			//wheel_state.Z_delta;
    	wheel_state.Z_delta = 0;
		
    	return (tmp == 0) ? false : true;
}
//--------------------------------------------------------------------------------------------------
// wheel_poll
//--------------------------------------------------------------------------------------------------
bool wheel_poll(void)
{
	uint8_t state;
    	uint8_t index; 
    	bool result = false;

    	// Read wheel voltage levels
	index = wheel_get_wheel_state();			

    	// Check state change
    	state = ENCODE_STATUS_TABLE[index];
 	/*   
    	if( state == 0 )
    	{
        	return false;
    	}
	*/

	switch(state)
	{
		case 1:			// Z+ 
			if(wheel_state.ZA_f == true)		// is last state is Z+?
			{
				wheel_state.Z_delta ++;			// yes, increase Z data
				wheel_state.ZA_f = false;			// clear Z+ flag
				result = true;								// Z data active
			}
			else
			{
				if(wheel_state.ZB_f == true)	// is last state is Z-? 
					wheel_state.ZB_f = false;		// yes, clear Z- flag
				wheel_state.ZA_f = true;			// set Z+ flag
			}
		break;
		case 2:			//Z-
			if(wheel_state.ZB_f == true)		// is last state is Z-?
			{
				wheel_state.Z_delta --;			// yes, decrease Z data
				wheel_state.ZB_f = false;			// clear Z- flag
				result = true;								// Z data active
			}
			else
			{
				if(wheel_state.ZA_f == true) 	// is last state is Z+?
					wheel_state.ZA_f = false;		// yes, clear Z+ flag
				wheel_state.ZB_f = true;			// set Z- flag
			}
		break;
		case 3:
			wheel_state.ZA_f = false;				// clear Z+ flag
			wheel_state.ZB_f = false;				// clear Z- flag
		default:
		break;
			
	}
    return result;
}
//--------------------------------------------------------------------------
// wheel_poll
//--------------------------------------------------------------------------
void Wheel_Polling(void)
{
	int8_t Z_data;
	if(wheel_poll_en == true)
    	{
		if (wheel_poll())
		{
			if(wheel_get_report(&Z_data))
			{
				//org size is 8 byte, 2016_0608 become 6 byte and remove id
				// report Z data By USB
				//Z_data = 0;						// When UpData Than Clear Data Ram
				//g_Main_Setting.MouseInReport[0] = 0x01;	//Mouse report ID			
				//g_Main_Setting.MouseInReport[6] = Z_data;	
				g_Main_Setting.MouseInReport[5] = Z_data;	
				g_Repot_Process.Mouse_Report_In = 1;		// Set UpReport Flag		
			
			}
		}	
    	}
    
}
#endif
