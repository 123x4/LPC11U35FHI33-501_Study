#include "Pixart_Sensor.h"

//U8 flagOne;		// to enable burst mode
//U8 u8motion,u8Squal,u8PixelSum;
//U8 stop_motion = 0;
//s16 s16accuX, s16accuY ;
//u16	u16Pixel,u16Shutter;
//S8 count_z;
//U8 cpi_p_Cntr, cpi_m_Cntr;
//U8 Min_Squal_value;
//U8 Pixel_Threshold_value;
//U8 flag_no_cpi;

// Function Prototype
// SENSOR SPI
void  nsAPI_SSP_Delay1us (U16 u16DelayTime);
//RetU8 nsAPI_SSP0_Sensor_Read (U8 u8Adr);
//void  nsAPI_SSP0_Sensor_Write (U8 u8Adr,U8 u8Com);
//void  nsAPI_SSP_Sensor_LoadSROM (U8 rom);
//void  nsAPI_Sensor_Hard_Reset (void);
//void  nsAPI_Sensor_SROM_Load (U8 rom_no);
//void  nsAPI_Sensor_Init (void);
//RetU8 esd_Check (void);
////void  nsAPI_SSP_Sensor_Monitor (void);
//bool nsAPI_SSP0_Sensor_Monitor(U16 * X_Value, U16* Y_Value);
//bool nsAPI_SSP1_Sensor_Monitor(U16 * X_Value, U16* Y_Value);
//RetU8 nsAPI_MouseScanButton (void);
////void  Z_Wheel (U32 temp0, U32 temp1);
//void  Z_Wheel(void);
//bool Read_Sensor_ID(void);
//bool Is_Sensor_Read_Rom_Complete;
//----------------------------------------------------------------------------------------
//extern U8 PROFILE_CUR_TABLE[24];
//----------------------------------------------------------------------------------------


void nsAPI_SSP_Delay1us(U16 u16DelayTime)
{
	U8 i;
	
	while(u16DelayTime)
	{
		for (i = 4; i > 0; i--){};
			u16DelayTime --;
	}	
}
#if 0
RetU8 nsAPI_SSP0_Sensor_Read(U8 u8Adr)
{
	// This part is to cater GUI usage.
	// Note reg 0x02 and 0x50 is under the same data path 
	
	//if( IS_BIT_SET(mouse_event_ctrl,ENABLE_NON_BURST_MODE_BIT) )
	#if 0
	{ // non burst mode
	   if((u8Adr != 0x02) && (u8Adr != 0x03) && (u8Adr != 0x04) && (u8Adr != 0x05) && (u8Adr != 0x06) )
		 {
				flagOne = ENABLE; // this to clean register 0x02 before normal motion read as other than register read 0x02,0x03,0x04,0x05,0x06 happend.
		 } 
	}
	#else	//2016_0411_Hsiang
	//else  // burst mode
	{
		if(u8Adr != 0x50)
		{
			flagOne = ENABLE;		// this flag is to clear register 0x50 before motion burst read as other read operation happened. .
		}
  	}	
	#endif
		
  	__disable_irq();
	
	NCS_LOW();
	nsAPI_SSP_Delay1us(TNCS_SCLK);
	
	nsAPI_SSP0_Send(&u8Adr,1);	
	nsAPI_SSP_Delay1us(TSRAD);	// 170us
	nsAPI_SSP0_Receive(&u8Adr,1);	
	
	nsAPI_SSP_Delay1us(TSCLK_NCS);	
	NCS_HIGH();
	nsAPI_SSP_Delay1us(TSRR);	// 20us

	__enable_irq(); 

	return u8Adr; 
}

RetU8 nsAPI_SSP1_Sensor_Read(U8 u8Adr)
{
	// This part is to cater GUI usage.
	// Note reg 0x02 and 0x50 is under the same data path 
	#if 0	//2016_0411_Hsiang
	if( IS_BIT_SET(mouse_event_ctrl,ENABLE_NON_BURST_MODE_BIT) )
	{ // non burst mode
	   if((u8Adr != 0x02) && (u8Adr != 0x03) && (u8Adr != 0x04) && (u8Adr != 0x05) && (u8Adr != 0x06) )
		 {
				flagOne = ENABLE; // this to clean register 0x02 before normal motion read as other than register read 0x02,0x03,0x04,0x05,0x06 happend.
		 } 
	}
	else  // burst mode
	#endif
	{
		if(u8Adr != 0x50)
		{
			flagOne = ENABLE;		// this flag is to clear register 0x50 before motion burst read as other read operation happened. .
		}
  	}	
	
		
  	__disable_irq();
	
	NCS_LOW();
	nsAPI_SSP_Delay1us(TNCS_SCLK);
	
	nsAPI_SSP1_Send(&u8Adr,1);	
	nsAPI_SSP_Delay1us(TSRAD);	// 170us
	nsAPI_SSP1_Receive(&u8Adr,1);	
	
	nsAPI_SSP_Delay1us(TSCLK_NCS);	
	NCS_HIGH();
	nsAPI_SSP_Delay1us(TSRR);	// 20us

	__enable_irq(); 

	return u8Adr; 
}

void nsAPI_SSP0_Sensor_Write(U8 u8Adr,U8 u8Com)
{
	if(u8Adr == 0x00)		// if register 0x00 was write.	
 	{	
		stop_motion	= u8Com; 
 	}	 
 	else
 	{	
	
    		__disable_irq();
	
		u8Adr = u8Adr|BIT7;
		NCS_LOW();
		nsAPI_SSP_Delay1us(5);  	// 5us
		nsAPI_SSP0_Send(&u8Adr,1);	// Send Address
		nsAPI_SSP_Delay1us(180);	// 180us
		nsAPI_SSP0_Send(&u8Com,1);	// Data	
		nsAPI_SSP_Delay1us(35);				
		NCS_HIGH();
		nsAPI_SSP_Delay1us(200);
	
		__enable_irq();
	}
}

void nsAPI_SSP1_Sensor_Write(U8 u8Adr,U8 u8Com)
{
	if(u8Adr == 0x00)		// if register 0x00 was write.	
 	{	
		stop_motion	= u8Com; 
 	}	 
 	else
 	{	
	
    		__disable_irq();
	
		u8Adr = u8Adr|BIT7;
		NCS_LOW();
		nsAPI_SSP_Delay1us(5);  	// 5us
		nsAPI_SSP1_Send(&u8Adr,1);	// Send Address
		nsAPI_SSP_Delay1us(180);	// 180us
		nsAPI_SSP1_Send(&u8Com,1);	// Data	
		nsAPI_SSP_Delay1us(35);				
		NCS_HIGH();
		nsAPI_SSP_Delay1us(200);
	
		__enable_irq();
	}
}


void nsAPI_SSP_Sensor_LoadSROM(U8 rom)
{
	U8 	u8SromTemp; 
	U8 *ptr;
     	U16 	sromCntr;      

	nsAPI_SSP0_Sensor_Write(CONFIG2_REG,0x00); 		// Disable rest mode	->Config2	
	nsAPI_SSP0_Sensor_Write(SROM_ENABLE_REG,0x1D);	// SROM Enable
		
	for(sromCntr = 0; sromCntr < 10; sromCntr++)
		nsAPI_SSP_Delay1us(1000); // 1ms

	nsAPI_SSP0_Sensor_Write(SROM_ENABLE_REG,0x18);
	nsAPI_SSP_Delay1us(120);//120us	
	
	NCS_LOW();

	u8SromTemp = (SROM_LOAD_BURST_REG|0x80);
	nsAPI_SSP0_Send(&u8SromTemp,1);
	nsAPI_SSP_Delay1us(140);//140us
   	if(rom==0x81)
		ptr=(U8*)SROM_81;
	else
		ptr=(U8*)SROM;
	for(sromCntr = 0; sromCntr < SROMSIZE; sromCntr++)
	{	
		//u8SromTemp = SROM[sromCntr]; 
		u8SromTemp = ptr[sromCntr]; 
		nsAPI_SSP0_Send(&u8SromTemp,1); 		
		nsAPI_SSP_Delay1us(30);//30us	
	}
	
	NCS_HIGH();
	nsAPI_SSP_Delay1us(180);//180us
}



void nsAPI_Sensor_Hard_Reset(void)
{
	U8 initCntr;
	NDrv_GPIO_SetBitValue(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, HIGH);	// Hard Reset
	nsAPI_SSP_Delay1us(50);
	NDrv_GPIO_SetBitValue(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, LOW);
	nsAPI_SSP_Delay1us(50);
	NDrv_GPIO_SetBitValue(SENSOR_NRESET_PORT, SENSOR_NRESET_PIN, HIGH);

	for(initCntr = 0; initCntr < 60; initCntr++)	// 60ms  
		nsAPI_SSP_Delay1us(1000);					// 1ms
}

void nsAPI_Sensor_SROM_Load (U8 rom_no)
{

#ifndef RUN_ALL_SROM	//2016_0722_Hsiang
	U8 tmp=0;
SROM_LOAD: 	 
#endif
	
	Is_Sensor_Read_Rom_Complete=TRUE;
	nsAPI_SSP_Sensor_LoadSROM(rom_no);
	//tmp=nsAPI_SSP0_Sensor_Read(SROM_ID_REG);
	if(nsAPI_SSP0_Sensor_Read(SROM_ID_REG) !=0x03 )
	{
#if 1
#ifndef RUN_ALL_SROM 
		
		goto SROM_LOAD;
#endif
#else
		Is_Sensor_Read_Rom_Complete=FALSE;
		return;
#endif
	}
  	nsAPI_SSP0_Sensor_Write(0x50, 0x01); // equivalent to write any value to register 0x02 for non burst mode.

}
//----------------------------------------------------------------------------------------------------
void nsAPI_Sensor_Shutdown(void)
{
	nsAPI_SSP0_Sensor_Write(0x3B, 0xB6);		// Soft Reset
}
//----------------------------------------------------------------------------------------------------
void nsAPI_Sensor_Init(void)
{	
	U8 ctr = 0;
	//U8 u8InitTemp = 3;	// Default for the LED setting
	flagOne = ENABLE;
	s16accuX.w = 0;		// Reset USB XY Accumulator
	s16accuY.w = 0;
	count_z = 0;

	

	#ifdef LED_ENABLE	//2016_0421_Hsiang edit
	#define LED_INIT_SETUP	u8InitTemp
	NDrv_GPIO_SetBitValue(PORT0,11,HIGH);	// Sensor's LED2_P SW3 OPEN
	TURN_ON_20R_SW1_LED(HIGH);	// 3rd highest light  default
	TURN_ON_39R_SW2_LED(LOW);
	// Default LED setting
	NDrv_GPIO_SetBitValue(PORT0,20,(LED_INIT_SETUP & BIT2)^BIT2); // DLED 1  PORT 0 _20
	NDrv_GPIO_SetBitValue(PORT1,26,(LED_INIT_SETUP & BIT1)^BIT1); // DLED 2  Port 1 _26				
	NDrv_GPIO_SetBitValue(PORT1,27,(LED_INIT_SETUP & BIT0)^BIT0); // DLED 3  Port 1 _27	
	NDrv_GPIO_SetBitValue(PORT1,21,HIGH);	// Must default high prevent NRESET Trigger
											// This effect the soft reset
	#endif
	/*	for(ctr = 0; ctr< 10; ctr++)// 10ms  
		nsAPI_SSP_Delay1us(1000);
	*/	
	NCS_HIGH();	
	nsAPI_SSP_Delay1us(1);
	NCS_LOW();
	
	#ifdef SOFT_RESET			
	nsAPI_SSP0_Sensor_Write(0x3A, 0x5A);		// Soft Reset
	for(ctr = 0; ctr< 60; ctr++)					// 60ms  
		nsAPI_SSP_Delay1us(1000);			// 1ms
	#else
	nsAPI_Sensor_Hard_Reset();				// Hard Reset (default)
	#endif

	ctr = (nsAPI_SSP0_Sensor_Read(MOTION_REG));	
	ctr = (nsAPI_SSP0_Sensor_Read(DELTA_X_L_REG));
	ctr = (nsAPI_SSP0_Sensor_Read(DELTA_X_H_REG));
	ctr = (nsAPI_SSP0_Sensor_Read(DELTA_Y_L_REG));
	ctr = (nsAPI_SSP0_Sensor_Read(DELTA_Y_H_REG));
	
	nsAPI_Sensor_SROM_Load(0x03);

	nsAPI_SSP0_Sensor_Write(CONFIG2_REG, 0x0B);		// CPI default value is 1200
	
	//	nsAPI_SSP_Sensor_Write(0x3D,0x96);	   // before 20/09/2013 change as per DS.
	//	nsAPI_SSP_Sensor_Write(0x3D,0x16);
	//	nsAPI_SSP_Sensor_Write(0x3D,0x95);	   // 20/09/2013 change as per DS.
	//	nsAPI_SSP_Sensor_Write(0x3D,0x15);	   // 12/6/2014 build in SROM
	//	nsAPI_SSP_Sensor_Write(0x2C,0x18);	   // Surface tuning 3/9/2013 pkc requested
	//	nsAPI_SSP_Sensor_Write(0x10,0x00);	   // Force run turn off 12/6/2014 by request of joon chok for testing purpose

	/*
	g_Main_Setting.EPInOutReq &= ~EP1_IN;				// Reset EP1 flag
	nsAPI_USB_WriteEP(	HID_EP1_IN,
						&g_Main_Setting.InReport[0],	// Dennis:
						g_Main_Setting.InReportSize);	// This one is a must, else usb won't work
	*/
	while(Read_Sensor_ID()==FALSE);		//check sensor ID

	/*
	R_Sensor_RepoetRate_CNT=(((U16)PROFILE_CUR_TABLE[EEPROM_POLLING_RATE_LOC])<<8)|PROFILE_CUR_TABLE[EEPROM_POLLING_RATE_LOC+1];// *((U16*)&(PROFILE_CUR_TABLE+EEPROM_POLLING_RATE_LOC)); 	//get polling rate from eeprom
	
	switch(R_Sensor_RepoetRate_CNT)
	{
		case 0:
			R_Sensor_RepoetRate_CNT = V_ReportRate_1000;
		break;
		
		case 1:
			R_Sensor_RepoetRate_CNT = V_ReportRate_500;
		break;
		
		case 2:
			R_Sensor_RepoetRate_CNT = V_ReportRate_250;
		break;
		
		case 3:
			R_Sensor_RepoetRate_CNT = V_ReportRate_125;
		break;
	}
	*/

	
}
//----------------------------------------------------------------------------------------------------

RetU8 esd_Check(void)
{
	U8 esdTemp = 0;

	esdTemp = nsAPI_SSP0_Sensor_Read(0x24);
	nsAPI_SSP_Delay1us(20);
  	nsAPI_SSP0_Sensor_Write(0x50, 0x01);
	nsAPI_SSP_Delay1us(180);

	if(!IS_BIT_SET( esdTemp,BIT6))
	{
		nsAPI_Sensor_Init();			// Re init Sensor
		return 1;
	}
	else
		return 0;
}
//----------------------------------------------------------------------------------------------------
bool nsAPI_SSP0_Sensor_Monitor(U16 * X_Value, U16* Y_Value)
{ 	
	U8 u8temp[ARR_SIZE] = {0}; //burst
	U8 tempBurst = 0;
	s16 u16temp_x;
	s16 u16temp_y;	
	bool rtn=FALSE;
	u16temp_x.w  = 0;
	u16temp_y.w  = 0;
	#if 0			//2016_0411_Hsiang
	//if( IS_BIT_SET(mouse_event_ctrl,ENABLE_NON_BURST_MODE_BIT) )
	{
		// Note: reading observation bit incorrectly will cause the motion erased.
		// u8temp[1] =	((nsAPI_SSP_Sensor_Read(OBSERVATION_REG))&SROM_RUN_BIT);

		if(flagOne == 1)
		{
			nsAPI_SSP0_Sensor_Write(0x02, 0x01);
			flagOne = 0;
		}
		
		u8temp[0] = ((nsAPI_SSP0_Sensor_Read(MOTION_REG))&MOTION_BIT);		
		
		if(IS_BIT_SET(u8temp[0],MOTION_BIT))
		{
			u8temp[2]	= (nsAPI_SSP0_Sensor_Read(DELTA_X_L_REG));
			nsAPI_SSP_Delay1us(10);
			u8temp[3]   = (nsAPI_SSP0_Sensor_Read(DELTA_X_H_REG));
			nsAPI_SSP_Delay1us(10);
			u8temp[4]	= (nsAPI_SSP0_Sensor_Read(DELTA_Y_L_REG));
			nsAPI_SSP_Delay1us(10);
			u8temp[5]	= (nsAPI_SSP0_Sensor_Read(DELTA_Y_H_REG));
			nsAPI_SSP_Delay1us(10);
			__disable_irq();
			//if( !IS_BIT_SET(u8temp[0],LIFT_BIT)) 
			{
				u16temp_x.b[0] = u8temp[2];
				u16temp_x.b[1] = u8temp[3];
				u16temp_y.b[0] = u8temp[4];
				u16temp_y.b[1] = u8temp[5];
				*X_Value = u16temp_x.b[1];	// X_H
	
				*X_Value = (*X_Value) << 8;
				(*X_Value) &= 0xff00;
				(*X_Value) |= u16temp_x.b[0];	// X_L

				(*Y_Value) = u16temp_y.b[1];	// Y_H
	
				(*Y_Value) = (*Y_Value) << 8;
				(*Y_Value) &= 0xff00;
				(*Y_Value) |= u16temp_y.b[0];	// Y_L

				//*X_Value = (~(*X_Value))+1;
				//*Y_Value = (~(*Y_Value))+1;
				rtn=TRUE;
			}
			__enable_irq();
			
		}
	}
	//else // burst mode
	#else
	{
		__disable_irq();
		if(flagOne == 1)
		{
			nsAPI_SSP0_Sensor_Write(0x50, 0x01);
			flagOne = 0;
		}

		NCS_LOW();
		nsAPI_SSP_Delay1us(TNCS_SCLK);				
		tempBurst = 0x50;
		nsAPI_SSP0_Send(&tempBurst,1);
		nsAPI_SSP_Delay1us(TSRAD_MOTBR);

		for(tempBurst = 0; tempBurst < ARR_SIZE; tempBurst++)
			nsAPI_SSP0_Receive( &u8temp[tempBurst],1);
		
		nsAPI_SSP_Delay1us(TSCLK_NCS);
		NCS_HIGH();
		__enable_irq();
	}
	
	if( IS_BIT_SET(u8temp[0],MOTION_BIT))
	{
		__disable_irq();
		
		if( !IS_BIT_SET(u8temp[0],LIFT_BIT)) 
		{
		//	flag_no_cpi = 1;
			#ifdef ENABLE_BURST_DAT_REPORT
			u16temp_x.b[0] = u8temp[2];
			u16temp_x.b[1] = u8temp[3];
			u16temp_y.b[0] = u8temp[4];
			u16temp_y.b[1] = u8temp[5];			
			//u8motion		= u8temp[0];
			//u8Squal			= u8temp[6];
			//u8PixelSum		= u8temp[7];
			//u16Pixel.b[1]	= u8temp[8]; // Min_Pixel
			//u16Pixel.b[0]	= u8temp[9]; // Max_Pixel
			//u16Shutter.b[1]	= u8temp[10];// shutter upper
			//u16Shutter.b[0]	= u8temp[11];// shutter lower
			#endif
			//s16accuX.w += u16temp_x.w;
			//s16accuY.w += u16temp_y.w;
			//g_Main_Setting.MouseEvent	   |= (MAIN_FLAG_MOUSE_EVENT|MAIN_FLAG_MOUSE_REPORT_READY);

			*X_Value = u16temp_x.b[1];	// X_H
	
			*X_Value = (*X_Value) << 8;
			(*X_Value) &= 0xff00;
			(*X_Value) |= u16temp_x.b[0];	// X_L

			(*Y_Value) = u16temp_y.b[1];	// Y_H
	
			(*Y_Value) = (*Y_Value) << 8;
			(*Y_Value) &= 0xff00;
			(*Y_Value) |= u16temp_y.b[0];	// Y_L

			//*X_Value = (~(*X_Value))+1;
			//*Y_Value = (~(*Y_Value))+1;
			rtn=TRUE;
		}
		__enable_irq();
	}		
	#endif
	return rtn;
}
//----------------------------------------------------------------------------------------------------
bool nsAPI_SSP1_Sensor_Monitor(U16 * X_Value, U16* Y_Value)
{ 	
	U8 u8temp[ARR_SIZE] = {0}; //burst
	//U8 tempBurst = 0;
	s16 u16temp_x;
	s16 u16temp_y;	
	bool rtn=FALSE;
	u16temp_x.w  = 0;
	u16temp_y.w  = 0;
	#if 1			//2016_0411_Hsiang
	{
		// Note: reading observation bit incorrectly will cause the motion erased.

		if(flagOne == 1)
		{
			nsAPI_SSP1_Sensor_Write(0x02, 0x01);
			flagOne = 0;
		}
		
		u8temp[0] = ((nsAPI_SSP1_Sensor_Read(MOTION_REG))&MOTION_BIT);		
		
		if(IS_BIT_SET(u8temp[0],MOTION_BIT))
		{
			u8temp[2]	= (nsAPI_SSP1_Sensor_Read(DELTA_X_L_REG));
			nsAPI_SSP_Delay1us(10);
			u8temp[3]   = (nsAPI_SSP1_Sensor_Read(DELTA_X_H_REG));
			nsAPI_SSP_Delay1us(10);
			u8temp[4]	= (nsAPI_SSP1_Sensor_Read(DELTA_Y_L_REG));
			nsAPI_SSP_Delay1us(10);
			u8temp[5]	= (nsAPI_SSP1_Sensor_Read(DELTA_Y_H_REG));
			nsAPI_SSP_Delay1us(10);
		}
	}
	//else // burst mode
	#else
	{
		__disable_irq();
		if(flagOne == 1)
		{
			nsAPI_SSP_Sensor_Write(0x50, 0x01);
			flagOne = 0;
		}

		NCS_LOW();
		nsAPI_SSP_Delay1us(TNCS_SCLK);				
		tempBurst = 0x50;
		nsAPI_SSP_Send(&tempBurst,1);
		nsAPI_SSP_Delay1us(TSRAD_MOTBR);

		for(tempBurst = 0; tempBurst < ARR_SIZE; tempBurst++)
			nsAPI_SSP_Receive( &u8temp[tempBurst],1);
		
		nsAPI_SSP_Delay1us(TSCLK_NCS);
		NCS_HIGH();
		__enable_irq();
	}
	#endif
	if( IS_BIT_SET(u8temp[0],MOTION_BIT))
	{
		__disable_irq();
		
		if( !IS_BIT_SET(u8temp[0],LIFT_BIT)) 
		{
		//	flag_no_cpi = 1;
			u16temp_x.b[0] = u8temp[2];
			u16temp_x.b[1] = u8temp[3];
			u16temp_y.b[0] = u8temp[4];
			u16temp_y.b[1] = u8temp[5];
#ifdef ENABLE_BURST_DAT_REPORT
			//u8motion		= u8temp[0];
			//u8Squal			= u8temp[6];
			//u8PixelSum		= u8temp[7];
			//u16Pixel.b[1]	= u8temp[8]; // Min_Pixel
			//u16Pixel.b[0]	= u8temp[9]; // Max_Pixel
			//u16Shutter.b[1]	= u8temp[10];// shutter upper
			//u16Shutter.b[0]	= u8temp[11];// shutter lower
#endif
			//s16accuX.w += u16temp_x.w;
			//s16accuY.w += u16temp_y.w;
			//g_Main_Setting.MouseEvent	   |= (MAIN_FLAG_MOUSE_EVENT|MAIN_FLAG_MOUSE_REPORT_READY);

			*X_Value = u16temp_x.b[1];	// X_H
	
			*X_Value = (*X_Value) << 8;
			(*X_Value) &= 0xff00;
			(*X_Value) |= u16temp_x.b[0];	// X_L

			(*Y_Value) = u16temp_y.b[1];	// Y_H
	
			(*Y_Value) = (*Y_Value) << 8;
			(*Y_Value) &= 0xff00;
			(*Y_Value) |= u16temp_y.b[0];	// Y_L

			*X_Value = (~(*X_Value))+1;
			*Y_Value = (~(*Y_Value))+1;
			rtn=TRUE;
		}
		__enable_irq();
	}		
	return rtn;
}

#if 0
RetU8 nsAPI_MouseScanButton(void)
{
	static U8 cpiTemp	 = 3;
//	static U8 cpi_p_Cntr = 0;
//	static U8 cpi_m_Cntr = 0;
	static U8 trigger_send_cpi_click = 0;

	U8 Ret = FALSE;
	U32 buttonTemp0 = 0;
	U32 buttonTemp1 = 0;
		
	U8 cpi_data[8] = {	CPI_0, CPI_1, CPI_2, CPI_3,
						CPI_4, CPI_5, CPI_6, CPI_7};


	buttonTemp0 = nsAPI_GPIO_GetPinValue(PORT0,0xFF);
	buttonTemp1 = nsAPI_GPIO_GetPinValue(PORT1,0xFF);

	if( ( (buttonTemp0 & BUTTON_MASK_0) != BUTTON_MASK_0) || 
		( (buttonTemp1 & BUTTON_MASK_1) != BUTTON_MASK_1) )
	{
		if(g_GpioButtonEvent.ButtonStatus != GPIO_SCANKEY_PR)
		{
			if(!(buttonTemp1 & BUTTON_RIGHT))
			{
				g_GpioButtonEvent.ButtonEvent = RIGHT_CLICKED;
			}
			else if(!(buttonTemp0 & BUTTON_MID))
			{
				g_GpioButtonEvent.ButtonEvent = MID_CLICKED;
			}
			else if(!(buttonTemp0 & BUTTON_LEFT))
			{
				g_GpioButtonEvent.ButtonEvent = LEFT_CLICKED;
			}
			else if(!(buttonTemp0 & BUTTON_CPI_P))
			{
				g_GpioButtonEvent.ButtonEvent = CPI_P_CLICKED;
			}
			else if(!(buttonTemp1 & BUTTON_CPI_M))
			{
				g_GpioButtonEvent.ButtonEvent = CPI_M_CLICKED;
			}
			else
			{
				g_GpioButtonEvent.ButtonEvent = 0;
				g_Main_Setting.MouseEvent &= ~MAIN_FLAG_MOUSE_BUTTON_EVENT;
			}
	
			if(g_GpioButtonEvent.ButtonEvent > 0)
			Ret = TRUE;
		}
	}
	else
	{
		if(g_GpioButtonEvent.ButtonStatus == GPIO_SCANKEY_PR)
		{	
			g_GpioButtonEvent.ButtonEvent = 0;
		}
	}

	//if((CHECK_MOTION_PIN() == HIGH) && (glitch_prevention == 0))  
	if((glitch_prevention == 0))  
	{
	
		if( ( g_GpioButtonEvent.ButtonEvent == CPI_P_CLICKED) || 
			( g_GpioButtonEvent.ButtonEvent == CPI_M_CLICKED) )
		{
			if(trigger_send_cpi_click == 0) 
			{
				cpi_clicked_flag = 1;

				/*
				if(g_GpioButtonEvent.ButtonEvent == CPI_P_CLICKED)
					cpi_p_Cntr++;
				else
					cpi_m_Cntr++;
				*/

				if( (cpi_p_Cntr > CPI_CLICK_DURATION) || 
					(cpi_m_Cntr > CPI_CLICK_DURATION) )
				{
					trigger_send_cpi_click = 1;
	
					if(cpi_p_Cntr > CPI_CLICK_DURATION)
					{
						cpi_p_Cntr = 0;	// send P to sensor
		
						if(cpiTemp < 7)
							cpiTemp = cpiTemp + 1;
					}
					if(cpi_m_Cntr > CPI_CLICK_DURATION)
					{
						cpi_m_Cntr = 0;	// send M to sensor
		
						if(cpiTemp > 0)
							cpiTemp = cpiTemp - 1;
					}
					nsAPI_SSP_Sensor_Write(0x0F, cpi_data[cpiTemp] );
					
					// LED Setting				
					NDrv_GPIO_SetBitValue(PORT0,20,(cpiTemp & BIT2)^BIT2); // DLED 1  PORT 0 _20
					NDrv_GPIO_SetBitValue(PORT1,26,(cpiTemp & BIT1)^BIT1); // DLED 2  Port 1 _26				
					NDrv_GPIO_SetBitValue(PORT1,27,(cpiTemp & BIT0)^BIT0); // DLED 3  Port 1 _27	
				}
			}
		}
		else
		{
				trigger_send_cpi_click = 0;
				cpi_m_Cntr = 0;
				cpi_p_Cntr = 0;
				cpi_clicked_flag = 0;
		}
	}
	return Ret;
}


void Z_Wheel()
{
	#define SCROLL_TREND	3
	#define SCROLL_SUBSIDY	1
	#define Z_COUNT			1
	static U8 ZA_PRE = 0;
	static U8 ZB_PRE = 0;
	//static S8 temp_z_ctr = 0;
	static U8 temp_z_up_trend = 0;
	static S8 temp_z_down_trend = 0;

	U32 temp0 = 0;
	U32 temp1 = 0;
	U8 ZA_CUR = 0;
	U8 ZB_CUR = 0;

	temp0 = nsAPI_GPIO_GetPinValue(PORT0,0xFF);
	temp1 = nsAPI_GPIO_GetPinValue(PORT1,0xFF);

	if(IS_BIT_SET(temp0,ZB_WHEEL))
		ZB_CUR = 1;
	
	if(IS_BIT_SET(temp1,ZA_WHEEL))
		ZA_CUR = 1;

	if(ZA_PRE == 1 && ZA_CUR == 1 && ZB_PRE == 0 && ZB_CUR == 1)
	{
		count_z++;
	}
	else if(ZA_PRE == 1 && ZA_CUR == 1 && ZB_PRE == 1 && ZB_CUR == 0)
	{
		count_z--;
	}
	else if(ZA_PRE == 0 && ZA_CUR == 1 && ZB_PRE == 1 && ZB_CUR == 1)
	{
		count_z--;
	}
	else if(ZA_PRE == 1 && ZA_CUR == 0 && ZB_PRE == 1 && ZB_CUR == 1)
	{
		count_z++;
	}

	if(count_z != 0x00)
	{
		g_Main_Setting.MouseEvent |= (MAIN_FLAG_MOUSE_EVENT|MAIN_FLAG_MOUSE_REPORT_READY);		
	}
	
	ZA_PRE = ZA_CUR;
	ZB_PRE = ZB_CUR;
	
}
#endif

bool Read_Sensor_ID(void)
{
	U8 u8temp=0;
	u8temp	= (nsAPI_SSP0_Sensor_Read(0x0));
	nsAPI_SSP_Delay1us(10);
	if(u8temp==0x42)
		return TRUE;
	else
		return FALSE;
}
//--------------------------------------------------------------------------------
void SENSOR_DPI_SETTING(U8 dpi_index)
{
	U8 dpi_value_tmp;
	// 0 =   625
	// 1 = 1250
	// 2 = 2500
	// 3 = 5000
	dpi_value_tmp=(50>>(3-dpi_index))-1;
	nsAPI_SSP1_Sensor_Write(CPI_REG, dpi_value_tmp);
}
//--------------------------------------------------------------------------------
bool Sensor_Lift_Cut_Off_Calibration(void)
{
	U8 u8temp=0;
	U8 times=0;
	NCS_HIGH();	
	nsAPI_SSP_Delay1us(1);
	NCS_LOW();
	
	#ifdef SOFT_RESET			
	nsAPI_SSP0_Sensor_Write(0x3A, 0x5A);				// Soft Reset
	for(u8temp = 0; u8temp< 60; u8temp++)					// 60ms  
		nsAPI_SSP_Delay1us(1000);			// 1ms
	#else
	nsAPI_Sensor_Hard_Reset();				// Hard Reset (default)
	#endif

	u8temp = (nsAPI_SSP0_Sensor_Read(MOTION_REG));	
	u8temp = (nsAPI_SSP0_Sensor_Read(DELTA_X_L_REG));
	u8temp = (nsAPI_SSP0_Sensor_Read(DELTA_X_H_REG));
	u8temp = (nsAPI_SSP0_Sensor_Read(DELTA_Y_L_REG));
	u8temp = (nsAPI_SSP0_Sensor_Read(DELTA_Y_H_REG));
	
	nsAPI_Sensor_SROM_Load(0x81);
	for(u8temp = 0; u8temp< 30; u8temp++)					// 30ms  
		nsAPI_SSP_Delay1us(1000);			// 1ms
	while(1)
	{
		nsAPI_SSP0_Sensor_Write(LIFTCUTOFF_TUNE1_REG, 0x80);
		u8temp	= (nsAPI_SSP0_Sensor_Read(0x0));
		if(u8temp==0x02 || u8temp==0x03)
			break;
		times++;
		if(times==10)
			return FALSE;
	}
	nsAPI_SSP0_Sensor_Write(LIFTCUTOFF_TUNE1_REG, 0x00);
	Min_Squal_value=nsAPI_SSP0_Sensor_Read(LIFTCUTOFF_TUNE3_REG);
	Pixel_Threshold_value=nsAPI_SSP0_Sensor_Read(LIFTCUTOFF_TUNE2_REG);
	nsAPI_Sensor_SROM_Load(0x03);
	nsAPI_SSP0_Sensor_Write(MIN_SQ_RUN_REG, Min_Squal_value);
	nsAPI_SSP0_Sensor_Write(PIXEL_THRESHOLD_REG, Pixel_Threshold_value);
	return TRUE;
}
#endif
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

