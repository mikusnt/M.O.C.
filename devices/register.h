/*!
 * @file register.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-12-07 $
 * $Created: 2017-11-04 $
 * @version 1.1
 * From Matrix Clock project.
 *
 * Used uC pins: ...<br>																										<br>
 * Features:<br>
 * -...
 * -...
 */

#ifndef register_H_
#define register_H_

#include "../group_includes.h"
#include "ds3231.h"

/*
 *
 *		IO macros
 *
 */


/*
 *		Hour
 */
//! direction register of hour serial data input
#define H_DATA_DDR DDRC
//! state register of hour serial data input
#define H_DATA_PORT PORTC
//! address of hour serial data input
#define H_DATA_ADDR (1 << PC0)
//! set high state to hour serial data input
#define H_DATA_HIGH() H_DATA_PORT |= H_DATA_ADDR
//! set low state to hour serial data input
#define H_DATA_LOW() H_DATA_PORT &= ~H_DATA_ADDR


/*
 *		Minute
 */
//! direction register of minute serial data input
#define M_DATA_DDR DDRB
//! state register of minute serial data input
#define M_DATA_PORT PORTB
//! address of minute serial data input
#define M_DATA_ADDR (1 << PB5)
//! set high state to minute serial data input
#define M_DATA_HIGH() M_DATA_PORT |= M_DATA_ADDR
//! set low state to minute serial data input
#define M_DATA_LOW() M_DATA_PORT &= ~M_DATA_ADDR


/*
 * 		Second
 */
//! direction register of second serial data input
#define S_DATA_DDR DDRB
//! state register of second serial data input
#define S_DATA_PORT PORTB
//! address of second serial data input
#define S_DATA_ADDR (1 << PB4)
//! set high state to second serial data input
#define S_DATA_HIGH() S_DATA_PORT |= S_DATA_ADDR
//! set low state to second serial data input
#define S_DATA_LOW() S_DATA_PORT &= ~S_DATA_ADDR

/*
 *		TIME_CLK_DATA
 */
//! direction register of clk of X group serial data inputs
#define TIME_CLK_DATA_DDR DDRB
//! state register of clk of X group serial data inputs
#define TIME_CLK_DATA_PORT PORTB
//! address of clk of X group serial data inputs
#define TIME_CLK_DATA_ADDR (1 << PB3)

/*
 *		Y_RESET, not used
 */
//! direction register of reset Y register
#define Y_RESET_DDR DDRD
//! state register of reset Y register
#define Y_RESET_PORT PORTD
//! address of reset Y register
#define Y_RESET_ADDR (1 << PD3)
//! set ON Y register
#define Y_ON() Y_RESET_PORT &= ~Y_RESET_ADDR
//! set OFF Y register
#define Y_OFF() Y_RESET_PORT |= Y_RESET_ADDR

/*
 *		LATCH
 */
//! direction register of parallel output enable of all registers
#define LATCH_DDR DDRB
//! state register of parallel output enable of all registers
#define LATCH_PORT PORTB
//! address of parallel output enable of all registers
#define LATCH_ADDR (1 << PB2)
//! set high state of parallel output enable of all registers
#define LATCH_HIGH() LATCH_PORT |= LATCH_ADDR
//! set low state of parallel output enable of all registers
#define LATCH_LOW() LATCH_PORT &= ~LATCH_ADDR


/*
 *
 *		Declaration of functions
 *
 */
//! initialize registers
extern void RegistersInit();
//! send byte table to time registers
inline void SendRegistersTime(uint8_t uiHour, uint8_t uiMinute,
		uint8_t uiSecond, bool bWithLoad);
//! send zeros to time registers
inline void ClearRegistersTime(bool bWithLoad);

/*
 *
 * 		Definitions of inline functions
 *
 */

//! to load new bytes to X registers
inline void Time_CLK_01() {
	TIME_CLK_DATA_PORT &= ~TIME_CLK_DATA_ADDR;
	TIME_CLK_DATA_PORT |= TIME_CLK_DATA_ADDR;

}

//! to reload parallel output in all registers
inline void LATCH_01() {
	LATCH_LOW();
	LATCH_HIGH();
}

/*! @param		uitLineBuffer logic bytes of registers state
 *  @param		bWithLoad enable to reload parallel output*/
inline void SendRegistersTime(uint8_t uiHour, uint8_t uiMinute,
		uint8_t uiSecond, bool bWithLoad) {
	register int8_t i;
	register uint8_t uiHourBCD = dec2bcd(uiHour);
	register uint8_t uiMinuteBCD = dec2bcd(uiMinute);
	register uint8_t uiSecondBCD = dec2bcd(uiSecond);
	for (i = 0; i < 8; i++) {
		if (uiHourBCD) H_DATA_HIGH();
			else H_DATA_LOW();
		if (uiMinuteBCD) M_DATA_HIGH();
			else M_DATA_LOW();
		if (uiSecondBCD) S_DATA_HIGH();
			else S_DATA_LOW();
		Time_CLK_01();
		uiHourBCD = uiHourBCD >> 1;
		uiMinuteBCD = uiMinuteBCD >> 1;
		uiSecondBCD = uiSecondBCD >> 1;
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegistersTime

/*! @param		bWithLoad enable to reload parallel output*/
inline void ClearRegistersTime(bool bWithLoad) {
	register uint8_t i;
	H_DATA_LOW();
	M_DATA_LOW();
	S_DATA_LOW();
	for (i = 0; i < 8; i++) {
		Time_CLK_01();
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegistersX

#endif /* 54HC595_H_ */
