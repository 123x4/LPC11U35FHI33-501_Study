//--------------------------------------------------------------------------
//
// This module implements the Wheel functionality in the mouse application.
//
//--------------------------------------------------------------------------
//
// Copyright 2006, Primax.
//
// This software is owned by Primax Electronics LTD (Primax)
// Use may be limited by and subject to the applicable Primax software
// license agreement.
//
//--------------------------------------------------------------------------


#ifndef WHEEL_H
#define WHEEL_H

//--------------------------------------
// Included files
//--------------------------------------
#include "GPIO.h"


//--------------------------------------
// API Definitions and Types
//--------------------------------------
//#define INVERSE_ZA_ZB

//#define ZA_PORT	0
//#define ZA_PIN	18

//#define ZB_PORT	0		
//#define ZB_PIN	17		


#ifdef P502
//#define WHEEL_ZA_PIN_CONFIG 	LPC_IOCON->PIO0_18
#define WHEEL_ZA_PIN 		18
#define WHEEL_ZA_PORT 		PORT0
//#define WHEEL_ZB_PIN_CONFIG 	LPC_IOCON->PIO1_17
#define WHEEL_ZB_PIN 		17
#define WHEEL_ZB_PORT 		PORT0

#ifdef 	ZCOM_ENABLE
//#define WHEEL_ZCOM_PIN_CONFIG 	LPC_IOCON->PIO0_18
#define WHEEL_ZCOM_PIN 		18
#define WHEEL_ZCOM_PORT 		PORT0
#endif


#if (ZCOM_PORT == 0xFF && ZCOM_PIN == 0xFF)
#define ZCOM_DISABLE
#endif
#else
#define WHEEL_ZA_PIN 		22
#define WHEEL_ZA_PORT 		PORT1

#define WHEEL_ZB_PIN 		16
#define WHEEL_ZB_PORT 		PORT0
#endif

extern bool wheel_poll_en;

//--------------------------------------
// API Function Declarations
//--------------------------------------

extern bool wheel_poll(void);
extern void Wheel_Init(void);
extern bool wheel_get_report(int8_t* report);
//extern void wheel_power_up(void);
//extern void wheel_power_down(void);
extern void Wheel_Polling(void);
//extern void wheel_sleep_setting(void);
//extern void wheel_wakeup_setting(void);

#endif   // WHEEL_H



