/*!
 * @file relay.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2019-03-03 $
 * $Created: 2017-11-04 $
 * @version 1.2
 * From Matrix Clock project.
 *
 * Used uC pins: 1<br>
 * Header file containing relay, which signals binary data which start sequence.
 * Bit 1 represents by high and low state proportion.
 * Bit 0 represents by other proportion.
 * After binary data is start sequence, relay vibration and pause.
 */

#ifndef DEVICES_RELAY_H_
#define DEVICES_RELAY_H_

#include "../group_includes.h"


/*
 *
 * 		Macros
 *
 */

// IO
//! direction register of relay
#define RELAY_DDR DDRD
//! state register of relay
#define RELAY_PORT PORTD
//! relay address
#define RELAY_ADDR (1 << PD0)
//! reverse relay state
#define RELAY_CH() RELAY_PORT ^= RELAY_ADDR
//! checks relay state
#define RELAY_IS_ON() (RELAY_PORT & RELAY_ADDR)

// time const
//! sum of time multipliers
//! @see		 RELAY_LONG_HIGH_TIME
//! @see		 RELAY_LONG_LOW_TIME
#define RELAY_MUL_SUM 10

//! one relay period time in ms
#define RELAY_INTERVAL_MS 1500

//! part of one relay period in ms
#define RELAY_STATE_MS (RELAY_INTERVAL_MS / RELAY_MUL_SUM)

#define RELAY_LONG_HIGH_TIME RELAY_STATE_MS * 5
#define RELAY_LONG_LOW_TIME RELAY_STATE_MS * 5
#define RELAY_SHORT_HIGH_TIME RELAY_STATE_MS * 1
#define RELAY_SHORT_LOW_TIME RELAY_STATE_MS * 9


//! period to rename start sequence when hour mode in ms
#define RELAY_HIGH_START_MS_HOUR 100
//! period to rename start sequence when minute mode in ms
#define RELAY_HIGH_START_MS_MINUTE 75
//! period to rename start sequence when numbers mode
#define RELAY_HIGH_START_MS_NUMBER 35

//! number of change relay state of start sequence when hours mode, should be even number
#define RELAY_START_H_COUNT 6

//! number of change relay state of start sequence when minutes mode, should be even number
#define RELAY_START_M_COUNT 0

//! number of change relay state of start sequence when numbers mode, should be even number
#define RELAY_START_N_COUNT 12

//! time of pause between start sequence and number code
#define RELAY_START_PAUSE_VALUE_MS 1500

/*
 *
 *		Main data types
 *
 */

//! type of data sending to relay structure
typedef enum {
	RelayDataHours = 0,//!< RelayDataHours
	RelayDataMinutes,  //!< RelayDataMinutes
	RelayDataNumber    //!< RelayDataNumber
} RelayDataType;

//! type of relay one click time
typedef enum {
	RelayClickStartFast = 0,
	RelayClickStart,
	RelayClickFast,
	RelayClickSlow
} RelayClickType;

typedef enum {
	RelayOFF = 0,		//!< relay not work
	RelaySilent = 1,	//!< relay clicks twice per hour (full and half)
	RelayBinaryFull = 2 //!< relay clicks hours and quaters by binary code
} RelayMode;

//! main relay structure
typedef struct {
	//! actual time counter in ms, when 0 relay change state and new value from ui16tTimeMSToClick
	/*! @see		ui16TimeMsToClick*/
	uint16_t ui16ActTimeMS;
	//! binary info to click by relay
	uint8_t uiByteInfo;
	//! length of binary info
	uint8_t uiByteLength;
	//! length of start info
	uint16_t ui16StartLength;
	//! time to change relay state in start sequence
	uint16_t ui16StartTimeMS;
	//! state of relay, modify by
	//! @see SetRelayState
	RelayMode eState;
	//! flag of enable data information after start
	bool bWithData;
	//! type of input data
	RelayDataType eDataType;
} Relay;

/*
 *
 *		Declarations of functions
 *
 */

//! initialize relay and load relay state from eeprom
extern void RelayInit(volatile Relay *r);
//! load required data in structure to binary clicking with start seq
extern void RelayTimeClicking(volatile Relay *relay, uint8_t uiByteInfo, RelayDataType dataType);
//! try to click bysed by data structure, called by 1 ms
extern void RelayTryClickMS(volatile Relay *relay);
//! rename relay state
extern void SetRelayState(volatile Relay *relay, RelayMode eState);
//! single click after reset relay structure, click time by variable
void RelayClicking(volatile Relay *relay, uint16_t ui16TimeMS, uint8_t uiNumber);
//! simple tests of relay dunctions, relay must be initialized
void RelayTest(volatile Relay *relay, bool bForeverLoop);
#endif /* DEVICES_RELAY_H_ */
