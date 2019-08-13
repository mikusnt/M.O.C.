/*!
 * @file main.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl

 * $Modified: 2019-04-15 $
 * $Created: 2018-11-04 $
 * @version 1.0
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



/*
 *
 *		Variables
 *
 */

//! internal time in milliseconds, [0 - 1000]
volatile uint16_t ui16Ms;
//! Relay struct
volatile Relay relay;
//! flag of new second, setting by DS3231 SQW overflow
//! @see ISR(PCINT1_vect)
volatile bool bNewTime;
//! time from RTC
TimeDate RTCTime;
//! actual time displaying on matrix
//! @see 	LoadTimeToMatrix
TimeDate actTime;
//! flag of slow decrement to high values to lowest [0 - 150]
volatile bool bNewDecrement;
//! time in milliseconds of presed buttons, reset when button not pressed and incremented value
volatile uint8_t uiPressedCounter;
//! time in milliseconds, counts to change state to normal time mode [0 - 3000]
volatile uint16_t ui16SetTimeMode;

bool bRefreshLampsMode;

/*
 *
 *		Overflows
 *
 */

// ISR(TIMER2_COMPA_vect)
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
	Timer2Init();
	PCINTInit();
	RelayInit(&relay);



	NeonInit();
	ButtonsInit();

	RegistersInit();

	I2C_Init();
	DS3231_Init();

	DS3231_GetTime(&RTCTime.uiHour, &RTCTime.uiMinute, &RTCTime.uiSecond);
	TimeInit(&actTime, 9);
////	DS3231_GetDate(&RTCTime.uiDay, &RTCTime.uiMonth, &RTCTime.uiYear);
	LoadToSingleTime(&RTCTime);
	RelayTimeClicking(&relay, 15 * RELAY_MODE(), RelayDataMinutes);
	sei();

	//RelayTest(&relay);
//	NeonTest();
	//RegistersTest();
	//wdt_enable(WDTO_2S);
	/*
	 *
	 *		Main loop
	 *
	 */
	while(1) {
		// buttons
		if (uiPressedCounter >= 150) {

			RTCTime.uiSecond = 0;
			if (MINUTE_IS_ON()) {
				RTCTime.uiMinute++;
				if (RTCTime.uiMinute >= 60)
					RTCTime.uiMinute = 0;
			}
			if (HOUR_IS_ON()) {
				RTCTime.uiHour++;
				if (RTCTime.uiHour >= 24)
					RTCTime.uiHour = 0;
			}

			SendRegistersTime(RTCTime.uiHour, RTCTime.uiMinute, RTCTime.uiSecond, true);
			LoadToSingleTime(&RTCTime);

			uiPressedCounter = 0;;
			ui16SetTimeMode = 1;
		}

		if (ui16SetTimeMode >= 3000) {
			NEON_OFF();
			DS3231_SetTime(RTCTime.uiHour, RTCTime.uiMinute, RTCTime.uiSecond);
			CopyDateTime(&RTCTime, &actTime);

			ui16SetTimeMode = 0;

		}

		// all cathodes heat on mode
		if (bRefreshLampsMode) {
			uint8_t value = RTCTime.uiSecond / 6;
			value += value * 10;
			SendRegistersTime(value, value, value, true);
			if ((RTCTime.uiHour < 6) && ((RTCTime.uiMinute % 6 == 1))) {
				bRefreshLampsMode = false;
				TimeInit(&actTime, 9);
			}
		}

		// time mode
		if (!ui16SetTimeMode) {


			if (bNewDecrement && CompareTime(&actTime, &RTCTime)) {
				bNewDecrement = false;
				SlowlyDecrementTime(&actTime, &RTCTime);
				if ((RTCTime.uiHour < 6) && ((RTCTime.uiMinute % 6) == 0)) {
					bRefreshLampsMode = true;
					continue;
				}
				if (!bRefreshLampsMode)
					SendRegistersTime(actTime.uiHour, actTime.uiMinute, actTime.uiSecond, true);
			}
			if (bNewTime) {

				DS3231_GetTime(&RTCTime.uiHour, &RTCTime.uiMinute, &RTCTime.uiSecond);
				LoadToSingleTime(&RTCTime);

				bNewTime = false;
				if (RTCTime.uiSecond % 2)
					NEON_ON();
				else
					NEON_OFF();



				// Relay
				if (RTCTime.uiSecond == 0) {
					relay.eState = RELAY_MODE();
					if (relay.eState == RelaySilent) {
						if (RTCTime.uiMinute == 0) {
							if ((RTCTime.uiHour == 0) || (RTCTime.uiHour == 12))
								RelayClicking(&relay, 750, 3);
							else
								RelayClicking(&relay, 750, 1);
						} else if (RTCTime.uiMinute == 30) {
							RelayClicking(&relay, 375, 1);
						}
					} else if (relay.eState == RelayBinaryFull) {
						if ( RTCTime.uiMinute == 0) {
							RelayTimeClicking(&relay, RTCTime.uiHour, RelayDataHours);
						} else if ((RTCTime.uiMinute == 15)
							|| (RTCTime.uiMinute == 30)
							|| (RTCTime.uiMinute == 45))
							RelayTimeClicking(&relay, RTCTime.uiMinute, RelayDataMinutes);
					}
				}
			}
		}
	}
} // END int main (void)

//! CTC timer2 overflow with 1ms period, internal time
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
	ui16Ms++;

	// HINT bNewDecrement time
	// basic - 100
	// big - 120
	if ((ui16Ms % 100) == 1)
		bNewDecrement = true;



	if (MINUTE_IS_ON() || HOUR_IS_ON()) {
		NEON_ON();
		ui16SetTimeMode = 1;
		uiPressedCounter++;
	} else {
		uiPressedCounter = 0;
	}

	if (ui16SetTimeMode && !uiPressedCounter)
		ui16SetTimeMode++;

} // END ISR(TIMER2_COMPA_vect)


//! SQW signal from DS3231 interrupt, set new time flag
ISR(PCINT1_vect) {
	//wdt_reset();
	if (SQW_IS_HIGH()) {
		bNewTime = true;
		ui16Ms = 0;
	}
} // END ISR(PCINT1_vect)


