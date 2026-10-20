#include "button.h"
//#include "GPIO.h"


BUTTON_SCAN_DATA buttonScan_stru;

extern BOOL latency_key_confirm;

//void Button_Init(void)
//{
//	buttonScan_stru.debounce = BUTTON_DEBOUNCE_COUNT;
//}





UINT32 Button_Location_Data(UINT32 port,UINT32 mask,UINT32 position,UINT32 reportPosition)
{
	UINT32 reportMask;
	UINT32 offSet;

	reportMask = port & mask;

	if (reportPosition > position)
	{
		offSet = reportPosition - position;
		reportMask <<= offSet;

	}else
	{
		offSet = position - reportPosition;
		reportMask >>= offSet;
	}

	return reportMask;
}



UINT32 Button_GetState(void)
{
	//static UINT8 lastP3;
	//static UINT8 lastP2;
	UINT32 button_port_state = 0;
//	UINT32 button_port1_state=0;
	UINT32 button_port0_state=0;
	///Profile DPI,K5,K4,WHEEL_DOWN,WHEEL_UP,MB,   RB, LB;
	///	8		7 , 6  ,5 ,   4        3,     2,   	1,  0; 
	button_port0_state = BUTTON_PORT0;
//	button_port1_state = BUTTON_PORT1;
	//button_port3_state = BUTTON_PORT3;

//	button_port_state |= Button_Location_Data(button_port0_state,VOLUME_UP_BUTTON_MASK, VOLUME_UP_BUTTON_POSITION ,VOLUME_UP_BUTTON_REPORT_BIT);
//	button_port_state |= Button_Location_Data(button_port0_state,VOLUME_DOWN_BUTTON_MASK,VOLUME_DOWN_BUTTON_POSITION ,VOLUME_DOWN_BUTTON_REPORT_BIT);
	button_port_state |= Button_Location_Data(button_port0_state,MIDDLE_BUTTON_MASK,MIDDLE_BUTTON_POSITION,MIDDLE_BUTTON_REPORT_BIT);
	button_port_state |= Button_Location_Data(button_port0_state,K4_BUTTON_MASK,K4_BUTTON_POSITION,K4_BUTTON_REPORT_BIT);
	button_port_state |= Button_Location_Data(button_port0_state,K5_BUTTON_MASK,K5_BUTTON_POSITION,K5_BUTTON_REPORT_BIT);
	
	button_port_state |= Button_Location_Data(button_port0_state,LEFT_BUTTON_MASK,LEFT_BUTTON_POSITION,LBFT_BUTTON_REPORT_BIT);
	button_port_state |= Button_Location_Data(button_port0_state,RIGHT_BUTTON_MASK,RIGHT_BUTTON_POSITION,RIGHT_BUTTON_REPORT_BIT);
	button_port_state |= Button_Location_Data(button_port0_state,DPI_CYCLE_BUTTON_MASK,DPI_CYCLE_BUTTON_POSITION,DPI_CYCLE_BUTTON_REPORT_BIT);

	return button_port_state;
}




BOOL Button_Scan(void)
{
	UINT8 i;
	UINT32 button;
   
	buttonScan_stru.button_changed = FALSE;

	button = Button_GetState();

	for(i = 0; i <= 2; i++)
	{
		if((button ^ buttonScan_stru.scan_result_cur) & (0x01 << i))
		{
			if(button & (0x01 << i))
			{
				buttonScan_stru.scan_result_cur |= (0x01 << i);
				buttonScan_stru.debounce[i] = 5;
			}
			else
			{
				buttonScan_stru.scan_result_cur &= ~(0x01 << i);
				buttonScan_stru.debounce[i] = 8;
			}
		}
		else
		{
			if(buttonScan_stru.debounce[i])
			{
				buttonScan_stru.debounce[i]--;
				if(buttonScan_stru.debounce[i] == 0)
				{
					if((buttonScan_stru.scan_result_pre ^ buttonScan_stru.scan_result_cur) & (0x01 << i))
					{
						if(buttonScan_stru.scan_result_cur & (0x01 << i))
						{
                            if(i == 0)
                            {
                                latency_key_confirm = TRUE;
                            }
							buttonScan_stru.scan_result_pre |= (0x01 << i);
							buttonScan_stru.button_changed = TRUE;
						}
						else
						{
							buttonScan_stru.scan_result_pre &= ~(0x01 << i);
							buttonScan_stru.button_changed = TRUE;
						}
					}
				}
			}
		}
	}
	
	for(i = 5; i <= 6; i++)
	{
		if((button ^ buttonScan_stru.scan_result_cur) & (0x01 << i))
		{
			if(button & (0x01 << i))
			{
				buttonScan_stru.scan_result_cur |= (0x01 << i);
				buttonScan_stru.debounce[i - 2] = 5;
			}
			else
			{
				buttonScan_stru.scan_result_cur &= ~(0x01 << i);
				buttonScan_stru.debounce[i - 2] = 8;
			}
		}
		else
		{
			if(buttonScan_stru.debounce[i - 2])
			{
				buttonScan_stru.debounce[i - 2]--;
				if(buttonScan_stru.debounce[i - 2] == 0)
				{
					if((buttonScan_stru.scan_result_pre ^ buttonScan_stru.scan_result_cur) & (0x01 << i))
					{
						if(buttonScan_stru.scan_result_cur & (0x01 << i))
						{
							buttonScan_stru.scan_result_pre |= (0x01 << i);
							buttonScan_stru.button_changed = TRUE;
						}
						else
						{
							buttonScan_stru.scan_result_pre &= ~(0x01 << i);
							buttonScan_stru.button_changed = TRUE;
						}
					}
				}
			}
		}
	}
	
	for(i = 9; i <= 9; i++)
	{
		if((button ^ buttonScan_stru.scan_result_cur) & (0x01 << i))
		{
			if(button & (0x01 << i))
			{
				buttonScan_stru.scan_result_cur |= (0x01 << i);
				buttonScan_stru.debounce[i - 4] = 5;
			}
			else
			{
				buttonScan_stru.scan_result_cur &= ~(0x01 << i);
				buttonScan_stru.debounce[i - 4] = 8;
			}
		}
		else
		{
			if(buttonScan_stru.debounce[i - 4])
			{
				buttonScan_stru.debounce[i - 4]--;
				if(buttonScan_stru.debounce[i - 4] == 0)
				{
					if((buttonScan_stru.scan_result_pre ^ buttonScan_stru.scan_result_cur) & (0x01 << i))
					{
						if(buttonScan_stru.scan_result_cur & (0x01 << i))
						{
							buttonScan_stru.scan_result_pre |= (0x01 << i);
							buttonScan_stru.button_changed = TRUE;
						}
						else
						{
							buttonScan_stru.scan_result_pre &= ~(0x01 << i);
							buttonScan_stru.button_changed = TRUE;
						}
					}
				}
			}
		}
	}

	return buttonScan_stru.button_changed;
}
