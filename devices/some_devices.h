/*!
 * @file some_devices.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2019-03-03 $
 * $Created: 2017-11-04 $
 * @version 1.0
 * From Matrix Clock project.
 *
 * Used uC pins: 5<br>
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
#define SQW_ADDR (1 << PC3)
//! check state of square wave second signal from DS3231
#define SQW_IS_HIGH() (SQW_PIN & SQW_ADDR)

// Hour
//! direction register of hour button
#define HOUR_DDR DDRB
//! state register of hour button
#define HOUR_PORT PORTB
//! value register of hour button
#define HOUR_PIN PINB
//! address register of hour button
#define HOUR_ADDR (1 << PB1)
//! check state of hour button
#define HOUR_IS_ON() (!(HOUR_PIN & HOUR_ADDR))

// Minute
//! direction register of minute button
#define MINUTE_DDR DDRB
//! state register of minute button
#define MINUTE_PORT PORTB
//! value register of minute button
#define MINUTE_PIN PINB
//! address register of minute button
#define MINUTE_ADDR (1 << PB0)
//! check state of minute button
#define MINUTE_IS_ON() (!(MINUTE_PIN & MINUTE_ADDR))

// Relay mode
//! direction register of minute button
#define RM_DDR DDRB
//! state register of minute button
#define RM_PORT PORTB
//! value register of minute button
#define RM_PIN PINB
//! address register of minute button
#define RM_ADDR ((1 << PB2) | (1 << PB3))
//! check state of minute button
#define RELAY_MODE() ((~((RM_PIN & RM_ADDR) >> 2)) & 0x03)


//! direction register of neon between hours and minutes lamps
#define NEON_DDR DDRD
//! state register of neon between hours and minutes lamps
#define NEON_PORT PORTD
//! address of neon between hours and minutes lamps
#define NEON_ADDR (1 << PD7)
//! set neon state to high
#define NEON_ON() NEON_PORT |= NEON_ADDR
//! set neon state to low
#define NEON_OFF() NEON_PORT &= ~NEON_ADDR

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
//! Initialize reisters of neon
extern void NeonInit();
//! Power reduction
extern void PowerReduction();
//! rename state of point
inline void NeonRename();
//! simple blinking test of neon
extern void NeonTest();
//! rename neon state
inline void NeonRename() {
	NEON_PORT ^= NEON_ADDR;
}

#endif /* SOME_DEVICES_H_ */
