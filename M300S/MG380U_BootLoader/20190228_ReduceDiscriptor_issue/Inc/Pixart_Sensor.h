#include "LPC11Uxx.h"
#include "system_LPC11Uxx.h"
#include "lpc11Uxx.h"          /* LPC11xx definitions */
#include "type.h"
#include "stdio.h"
#include "Main.h"
#include "srom.h"
#include "hiduser.h"
#include "Keytbl.h"
#include "ssp.h"
#include "uart.h"
#include "I2C.h"
#include "Gpio.h"
#include "uart.h"
#include "timer16.h"
#include "timer32.h"
#include "hid.h"
#include "usb.h"
#include "usbdesc.h"
#include "usbuser.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "string.h"
#include "flash.h"
#include "stdlib.h"

enum{
	BUTTON = 0,	//0
	X_LOW,		//1
	X_HIGH,		//2
	Y_LOW,		//3
	Y_HIGH,		//4
	WHEEL,		//5
	MOTION,		//	6
	SQUAL,		//	7
	PIXEL_SUM,	//	8
	MAX_PIXEL,	//	9
	MIN_PIXEL,	//	10
	SHUTTER_UPPER,	//11
	SHUTTER_LOWER,	//12
};

//********************************************************************************************
// Operating mode
#define SSP_FIFOSIZE			8
#define POLL_RATE_BTN			10
#define POLL_Z_WHEEL			3
#define ENABLE_BURST_DAT_REPORT	// Custom EP1 USB size
//#define SPI_BURST_ENABLE		// run on spi burst mode
//#define UGS2_VERSION1			// for first customer/version compliance, 
#define RUN_ALL_SROM			
								// for first customer/version compliance, 
								// enable it to run without checking SROM ID
								// So that user can dump the right SROM later from
								// other source. Keeping one source firmware which work
								// for all is better.
#define SOFT_RESET			    // Using Soft reset power up
//#define INVERTED				// For older mouse
//********************************************************************************************


//********************************************************************************************
// Custom EP1 USB size
#define STANDARD_MOUSE_DAT_SIZE	0x0006	//6 bytes. 1byte: button. 4 bytes:XY16bits 1bytes:Zwheel

#ifdef	ENABLE_BURST_DAT_REPORT
	#define BURST_DATA_SIZE		0x000C	// Refers to Datasheets on
	#define EP1_MAX_SIZE		(STANDARD_MOUSE_DAT_SIZE + BURST_DATA_SIZE)
//	#define USB_PACKET_SIZE		12
#else
	#define EP1_MAX_SIZE		STANDARD_MOUSE_DAT_SIZE
#endif
//********************************************************************************************


//********************************************************************************************
// Reference Mouse Information for usbcore.c
//
// Reference Mouse Default Value
#define	MOUSE_INFO				0x01
#define	MOUSE_EVENT_CTRL		0x7f	 
#define	MOUSE_FIRMWARE_major 	0x03
#define	MOUSE_FIRMWARE_minor 	0x01	
// switch cases 
#define REF_MOUSE_INFO_ADDR						0x00
#define REF_MOUSE_EVENT_CTRL_ADDR				0x03
#define REF_MOUSE_FIRMWARE_MAJOR_ADDR			0x06
#define REF_MOUSE_FIRMWARE_MINOR_ADDR			0x07
#define REF_MOUSE_LED_SW_ADDR					0x08
#define REF_MOUSE_NCS							0x09
#define REF_MOUSE_SPI_BURST_FRAME_CAPTURE		0x0A
#define REF_MOUSE_SPI_BURST_FRAME_CAPTURE_READ	0x0B
#define REF_MOUSE_SROM_LOAD_CTRL_ADDR			0xFB
#define REF_MOUSE_SROM_WRITE_ADDR				0xFF
//unused cases
//#define REF_MOUSE_MOTION_CTRL_ADDR			0x04
//#define REF_MOUSE_CPI_CTRL_ADDR				0x05
//#define REF_MOUSE_SERIAL_LOWER_ADDR			0x01
//#define REF_MOUSE_SERIAL_UPPER_ADDR			0x02
//#define REF_MOUSE_MAN_CAL_CTRL_ADDR			0x20
//#define REF_MOUSE_MIN_SQUAL_ADDR				0x21
//#define REF_MOUSE_MIN_SQUAL_FACTOR_ADDR		0x22
//#define REF_MOUSE_MAX_SHUTTER_LOWER_ADDR		0x23
//#define REF_MOUSE_MAX_SHUTTER_UPPER_ADDR		0x24
//#define REF_MOUSE_BAD_MIN_SQUAL_ADDR			0x25
//#define REF_MOUSE_BAD_MAX_SHUTTER_LOWER_ADDR	0x26
//#define REF_MOUSE_BAD_MAX_SHUTTER_UPPER_ADDR	0x27
//#define REF_MOUSE_TSRAD_DELAY_ADDR			0x40  // SPI Read Address-Data Delay Register 
//********************************************************************************************

//********************************************************************************************
// TCL USER - USB Commands & Control
#define TCL_CONTROL_0_READ		0xC0
#define TCL_CONTROL_0_WRITE		0x40
//********************************************************************************************

//********************************************************************************************
// SROM flash
#define START_OF_FLASH_SECTOR	0x00006000UL
#define END_OF_FLASH_SECTOR		0x00007000UL
#define BUSY_RW					0x00
#define FREE					0xAA
#define READY_TO_FLASH			1
#define INIT_TCL_FLASH			1
#define TCL_END_FLASH			2  
#define REBOOT					4
#define SROM_ID					SROM[1]
#define SROMSIZE				sromTableSize	// from srom.c
#define SROM_BUFFER_SIZE 		256				// RAM bytes allocation flash purpose
//********************************************************************************************
//#define FRAME_CAPTURE_SIZE		1296
//********************************************************************************************
// Sensor motion monitor
#define ENABLE_MOTION_BIT			BIT0	// If this is 0 turn off motion
#define ENABLE_BUTTON_SCROLL_BIT	BIT1	// If this is 0 turn off all buttons and scroll
#define ENABLE_ALWAYS_POLL_BIT		BIT2	// for burst_mode, always poll
#define ENABLE_NON_BURST_MODE_BIT	BIT3	// enable non burst mode.
#define ENABLE						1
#define DISABLE						0
#define ARR_SIZE					7		// 16 bytes
#define ESD_POLL_TIME				3000	// 3000ms
#define SROM_RUN_BIT				BIT6	// observation bit check
#define MOTION_BIT					BIT7	// Motion bit check
#define LIFT_BIT					BIT3	// Lift bit check

//********************************************************************************************

//********************************************************************************************
// DELAYS SPI
#define TMOT_RST				60 		// 6x1000 = 60ms
#define TSRAD_MOTBR				35		// 35us
#define TNCS_SCLK				1 		// 120ns, but 1us min
#define TSCLK_NCS				1
#define TSRR					20 		// 20us
#define TSRAD					180
//********************************************************************************************

//********************************************************************************************
// SROM_ID
#define SROM_ID					SROM[1]
#define SROMSIZE				sromTableSize
//Register Address
#define PRODUCT_ID_REG				0x00
#define PRODUCT_REV_REG			0x01
#define MOTION_REG					0x02	
#define DELTA_X_L_REG				0x03
#define DELTA_X_H_REG				0x04
#define DELTA_Y_L_REG				0x05
#define DELTA_Y_H_REG				0x06
#define CPI_REG						0x0F
#define CONFIG2_REG					0x10
#define SROM_ENABLE_REG			0x13
#define OBSERVATION_REG			0x24
#define SROM_ID_REG					0x2a
#define MIN_SQ_RUN_REG				0x2b
#define PIXEL_THRESHOLD_REG		0x2c
#define POWER_RESET_REG			0x3a
#define LIFTCUTOFF_TUNE3_REG		0x41
#define LIFTCUTOFF_TUNE1_REG		0x4A
#define SROM_LOAD_BURST_REG		0x62
#define LIFTCUTOFF_TUNE2_REG		0x65

//********************************************************************************************
#define CHECK_MOTION_PIN()		NDrv_GPIO_GetPinValue(PORT1,15)
//#define NCS_HIGH()				NDrv_GPIO_SetBitValue(PORT0,2,HIGH) 
//#define NCS_LOW()				NDrv_GPIO_SetBitValue(PORT0,2,LOW)
#define IS_BIT_SET(REG,MASK)   	((REG & MASK)==MASK)
//********************************************************************************************

//********************************************************************************************
// UGS 2 Mouse button configuration.
#define LEFT_CLICKED			BIT0
#define RIGHT_CLICKED			BIT1
#define MID_CLICKED				BIT2
#define CPI_P_CLICKED			BIT3
#define CPI_M_CLICKED			BIT4
#define BUTTON_RIGHT			BIT15// Port1
#define BUTTON_MID				BIT17// Port0
#define BUTTON_LEFT				BIT18// Port0
#define BUTTON_CPI_P			BIT19// Port0
#define BUTTON_CPI_M			BIT16// Port1
// UGS 2 Mouse mask button configuration 
#define BUTTON_MASK				(BIT31|BIT28|BIT27|BIT26|BIT25)
#define BUTTON_MASK_0			(BIT19|BIT18|BIT17)
#define BUTTON_MASK_1			(BIT16|BIT15)
// Z wheel configuration
#define ZB_WHEEL 				BIT7		// Port0_7
#define ZA_WHEEL 				BIT28		// Port1_28

// SW1 & SW2 LED UGS2 ROW
#define TURN_ON_20R_SW1_LED(MASK) NDrv_GPIO_SetBitValue(PORT0,14,MASK)// Sensor's LED1_P SW2
#define TURN_ON_39R_SW2_LED(MASK) NDrv_GPIO_SetBitValue(PORT0,13,MASK)// Sensor's LED1_P SW1
//********************************************************************************************

//********************************************************************************************
// CPI value in decimal
#define CPI_CLICK_DURATION		200	   // 15/10/2014 
//#define UGS2_NON_MOUSE_VERSION

#ifdef UGS2_VERSION1 	// UGS2 Version 1
#define CPI_0			0x03		  // 200cpi
#define CPI_1			0x28		  // 2000cpi
#define CPI_2			0x3B		  // 3000cpi
#define CPI_3			0x62		  // 4950cpi(default)
#define CPI_4			0x77		  // 6000cpi
#define CPI_5			0x8B		  // 7000cpi
#define CPI_6			0xC7		  // 10000cpi
#define CPI_7			0xFF		  // 12800cpi
#elseif UGS2_NON_MOUSE_VERSION
#define CPI_0			0x01		  // 200cpi
#define CPI_1			0x13		  // 2000cpi
#define CPI_2			0x1D		  // 3000cpi
#define CPI_3			0x31		  // 5000cpi
#define CPI_4			0x3B		  // 6000cpi
#define CPI_5			0x45		  // 7000cpi
#define CPI_6			0x4F		  // 8000cpi
#define CPI_7			0x51		  // 8200cpi
#else 					// UGS2 version 2 row
#define CPI_0			0x01		  // 200cpi
#define CPI_1			0x13		  // 2000cpi
#define CPI_2			0x1D		  // 3000cpi
#define CPI_3			0x31		  // 5000cpi
#define CPI_4			0x3B		  // 6000cpi
#define CPI_5			0x45		  // 7000cpi
#define CPI_6			0x63		  // 10000cpi
#define CPI_7			0x77		  // 12000cpi
#endif
//********************************************************************************************

//********************************************************************************************
//Extern variable
extern U8	mouse_event_ctrl;//motion_flag;
extern U8	flagOne;
extern U8	u8motion,u8Squal,u8PixelSum;
extern u16	u16Pixel,u16Shutter;
extern s16	s16accuX, s16accuY;
extern U16	poll_tick_btn, poll_usb, poll_id,poll_z_wheel,glitch_prevention, cpi_clicked_flag;
extern U8 	cpi_p_Cntr, cpi_m_Cntr;
extern S8 	count_z;
extern U8	R_Sensor_RepoetRate_CNT;
extern bool 	Is_Sensor_Read_Rom_Complete;
//********************************************************************************************

//********************************************************************************************
//Extern function prototype
extern void nsAPI_SSP_Delay1us(U16 u16DelayTime);
extern RetU8 nsAPI_SSP0_Sensor_Read(U8 u8Adr);
extern RetU8 nsAPI_SSP1_Sensor_Read(U8 u8Adr);
extern void nsAPI_SSP0_Sensor_Write(U8 u8Adr,U8 u8Com);
extern void nsAPI_SSP1_Sensor_Write(U8 u8Adr,U8 u8Com);
extern void nsAPI_SSP_Sensor_LoadSROM(U8 rom);
extern void nsAPI_Sensor_Hard_Reset(void);
extern void nsAPI_Sensor_SROM_Load (U8 rom_no);
extern void nsAPI_Sensor_Init(void);
//extern void nsAPI_SSP_Sensor_Monitor(void);
extern bool nsAPI_SSP0_Sensor_Monitor(U16 * X_Value, U16* Y_Value);
extern bool nsAPI_SSP0_Sensor_Monitor(U16 * X_Value, U16* Y_Value);
extern RetU8 nsAPI_MouseScanButton(void);
extern void Z_Wheel(void);
extern void SENSOR_DPI_SETTING(U8 dpi_index);
//********************************************************************************************


