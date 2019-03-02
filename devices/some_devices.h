/*!
 * @file some_devices.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2019-03-02 $
 * $Created: 2017-11-04 $
 * @version 1.1
 * From Matrix Clock project.
 *
 * Used uC pins: ...<br>
 * Header file containing configuration of timers, rename time and relay mode buttons.
 */

#ifndef SOME_DEVICES_H_
#define SOME_DEVICES_H_

#include "../group_includes.h"

/*
 *
 *		Macros
 *
 */
// SQW
//! direction register of square wave second signal from DS3231
#define SQW_DDR DDRC
//! state register of square wave second signal from DS3231
#define SQW_PORT PORTC
//! value register of square wave second signal from DS3231
#define SQW_PIN PINC
//! address register of square wave second signal from DS3231
#define SQW_ADDR (1 << PC1)
//! check state of square wave second signal from DS3231
#define SQW_IS_HIGH() (SQW_PIN & SQW_ADDR)

// Hour
//! direction register of hour button
#define HOUR_DDR DDRD
//! state register of hour button
#define HOUR_PORT PORTD
//! value register of hour button
#define HOUR_PIN PIND
//! address register of hour button
#define HOUR_ADDR (1 << PD1)
//! check state of hour button
#define HOUR_IS_HIGH() (HOUR_PIN & HOUR_ADDR)

// Minute
//! direction register of minute button
#define MINUTE_DDR DDRD
//! state register of minute button
#define MINUTE_PORT PORTD
//! value register of minute button
#define MINUTE_PIN PIND
//! address register of minute button
#define MINUTE_ADDR (1 << PD2)
//! check state of minute button
#define MINUTE_IS_HIGH() (MINUTE_PIN & MINUTE_ADDR)

// Relay mode
//! direction register of minute button
#define RM_DDR DDRD
//! state register of minute button
#define RM_PORT PORTD
//! value register of minute button
#define RM_PIN PIND
//! address register of minute button
#define RM_ADDR ((1 << PD3) | (1 << PD4))
//! check state of minute button
#define RM_MODE() (~(((RM_PIN & RM_ADDR) >> 3) & 0x03))
//! direction register of dot between hours and minutes lamps
#define POINT_DDR DDRD
//! state register of dot between hours and minutes lamps
#define POINT_PORT PORTD
//! address of dot between hours and minutes lamps
#define POINT_ADDR (1 << PD1)

/*
 *
 *		Declaration of functions
 *
 */

//! Initialize buttons containing hour, minute and relay mode buttons
extern void ButtonsInit();
//! Initialize Timer2 to cunting time between DS3231 interrupts
extern void Timer2Init();
//! Initialize PCINT interrupt
extern void PCINTInit();
//! Initialize reisters of point
extern void PointInit();
//! Power reduction
extern void PowerReduction();
//! rename state of point
inline void RenamePoint();

extern void PointTest();

inline void RenamePoint() {
	POINT_PORT ^= POINT_ADDR;
}

#endif /* SOME_DEVICES_H_ */
