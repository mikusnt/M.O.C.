/*!
 * @file main.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl

 * $Modified: 2018-10-31 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Project main file
 * @see readme.md
 */

#include "group_includes.h"
#include "devices/relay.h"
#include "devices/ds3231.h"
#include "devices/register.h"
#include "data_types/date_time.h"
#include "devices/some_devices.h"
#include "devices/text_eeprom.h"

#include "tests.h"



/*
 *
 *		Variables
 *
 */

//! internal time in milliseconds, [0 - 1000]
volatile uint16_t ui16Ms = 0;
//! internal time in seconds, cooperate with ui16Ms
volatile uint8_t uiSeconds;
//! Relay struct
volatile Relay relay;
//! flag of new second, setting by DS3231 SQW overflow
//! @see ISR(PCINT1_vect)
volatile bool bNewTime = false;
//! time from RTC
TimeDate RTCTime;
//! actual time displaying on matrix
//! @see 	LoadTimeToMatrix
TimeDate actTime;
//! flag of critical section on Timer0 register operations
volatile bool bCriticalY = false;

/*
 *
 *		Overflows
 *
 */

// ISR(TIMER0_COMPA_vect)
// ISR(TIMER1_COMPA_vect)
// ISR(TIMER2_COMPA_vect)
// ISR(ADC_vect)
// ISR(PCINT1_vect)

/*
 *
 *		Main function
 *
 */

int main (void) {
	/*
	 *
	 *		Init block
	 *
	 */
	PowerReduction();
	RegistersInit();
	TextEEPromInit();
	RelayInit(&relay);
	Timer0Init();
	Timer2Init();
	PCINTInit();

	I2C_Init();

	DS3231_Init();
	//eActualSeq = SeqDeCounter;
	RelayStartClicking(&relay, 15 * relay.eState, RelayDataMinutes);

	wdt_enable(WDTO_120MS);
	DS3231_GetTime(&RTCTime.uiHour, &RTCTime.uiMinute, &RTCTime.uiSecond);
	DS3231_GetDate(&RTCTime.uiDay, &RTCTime.uiMonth, &RTCTime.uiYear);
	LoadToSingleTime(&RTCTime);

	sei();
	/*
	 *
	 *		Main loop
	 *
	 */

	while(1) {
		wdt_reset();
	}
} // END int main (void)

//! CTC timer0 overflow, refreshing matrix, required hard optimisation,
//! always critical section, when rename Y, next overflow ignored
ISR(TIMER0_COMPA_vect) {

} // END ISR(TIMER0_COMPA_vect)

//! CTC timer2 overflow with 1ms period, internal time
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
	// reset to 0 in PCINT1_vect
	if (++ui16Ms == 1) {
		if (++uiSeconds >= 60)
			uiSeconds = 0;
	}

	if (ui16Ms >= 1000) ui16Ms = 0;
} // END ISR(TIMER2_COMPA_vect)


//! SQW signal from DS3231 interrupt, set new time flag
ISR(PCINT1_vect) {
	ADCStart();
	if (SQW_IS_HIGH()) {
		bNewTime = true;
		//ui16Ms = 0;
	}
} // END ISR(PCINT1_vect)


