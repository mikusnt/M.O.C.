/*!
 * @file register.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2019-03-03 $
 * $Created: 2017-11-04 $
 * @version 1.2
 * From Matrix Clock project.
 *
 * Used uC pins: 4<br>
 * Features:<br>
 * -...
 * -...
 */

#ifndef register_H_
#define register_H_

#include "../group_includes.h"
#include "ds3231.h"


// HINT optional seconds register
#define WITH_SECONDS 1

/*
 *		Hour, optional register, when no seconds minute addr will be hour addr and second addr will be minute addr, see Eagle schematic
 */
//! direction register of hour serial data input
#define H_DATA_DDR DDRD
//! state register of hour serial data input
#define H_DATA_PORT PORTD
//! address of hour serial data input
#define H_DATA_ADDR (1 << PD1)
//! set high state to hour serial data input
#define H_DATA_HIGH() H_DATA_PORT |= H_DATA_ADDR
//! set low state to hour serial data input
#define H_DATA_LOW() H_DATA_PORT &= ~H_DATA_ADDR


/*
 *		Minute
 */
//! direction register of minute serial data input
#define M_DATA_DDR DDRC
//! state register of minute serial data input
#define M_DATA_PORT PORTC
//! address of minute serial data input
#define M_DATA_ADDR (1 << PC2)
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
#define S_DATA_ADDR (1 << PB5)
//! set high state to second serial data input
#define S_DATA_HIGH() S_DATA_PORT |= S_DATA_ADDR
//! set low state to second serial data input
#define S_DATA_LOW() S_DATA_PORT &= ~S_DATA_ADDR


//! direction register of clk of time data input
#define TIME_CLK_DATA_DDR DDRC
//! state register of clk of time data input
#define TIME_CLK_DATA_PORT PORTC
//! address of clk of time data input
#define TIME_CLK_DATA_ADDR (1 << PC1)
/*

		Y_RESET, not used
//! direction register of reset Y register
#define Y_RESET_DDR DDRD
//! state register of reset Y register
#define Y_RESET_PORT PORTD
//! address of reset Y register
#define Y_RESET_ADDR (1 << PD3)
//! set ON Y register
#define Y_ON() Y_RESET_PORT &= ~Y_RESET_ADDR
//! set OFF Y register
#define Y_OFF() Y_RESET_PORT |= Y_RESET_ADDR*/

/*
 *		LATCH
 */
//! direction register of parallel output enable of all registers
#define LATCH_DDR DDRC
//! state register of parallel output enable of all registers
#define LATCH_PORT PORTC
//! address of parallel output enable of all registers
#define LATCH_ADDR (1 << PC0)
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
//! simple test, send all digits to hour and minute registers
void RegistersTest();

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

/*! @param		uiHour value of hour lamps
 *  @param		uiMinute value of minute lamps
 *  @param		uiSecond value of second lamps
 *  @param		bWithLoad enable to reload parallel output*/
inline void SendRegistersTime(uint8_t uiHour, uint8_t uiMinute, uint8_t uiSecond, bool bWithLoad) {
	register int8_t i;
	register uint8_t uiHourBCD = dec2bcdrev(uiHour);
	register uint8_t uiMinuteBCD = dec2bcdrev(uiMinute);
	register uint8_t uiSecondBCD = dec2bcdrev(uiSecond);
	//register uint8_t uiSecondBCD = dec2bcd(uiSecond);
	for (i = 0; i < 8; i++) {
		if (uiHourBCD % 2) H_DATA_HIGH();
			else H_DATA_LOW();
		if (uiMinuteBCD % 2) M_DATA_HIGH();
			else M_DATA_LOW();

		if (WITH_SECONDS) {
			if (uiSecondBCD % 2) S_DATA_HIGH();
				else S_DATA_LOW();
		}
		//if (uiSecondBCD) S_DATA_HIGH();
		//	else S_DATA_LOW();
		Time_CLK_01();
		uiHourBCD >>= 1;
		uiMinuteBCD >>= 1;
		uiSecondBCD >>= 1;
		//uiSecondBCD = uiSecondBCD >> 1;
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegistersTime

/*! @param		bWithLoad enable to reload parallel output*/
inline void ClearRegistersTime(bool bWithLoad) {
	register uint8_t i;
	H_DATA_LOW();
	M_DATA_LOW();
	//S_DATA_LOW();
	for (i = 0; i < 8; i++) {
		Time_CLK_01();
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegistersX

#endif /* 54HC595_H_ */
