/*!
 * @file some_devices.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see other_devices.h*/

#include "some_devices.h"

void ButtonsInit() {
	HOUR_DDR &= ~HOUR_ADDR; // pull up resistors and input mode
	HOUR_PORT |= HOUR_ADDR;

	MINUTE_DDR &= ~MINUTE_ADDR;
	MINUTE_PORT |= MINUTE_ADDR;

	RM_DDR &= ~RM_ADDR;
	RM_PORT |= RM_ADDR;
} // END void ButtonsInit

//! 1ms period
void Timer2Init() {
	// tryb CTC
	TCCR2A |= (1 << WGM21);
	// preskaler 256
	TCCR2B |= (1 << CS22) | (1 << CS21);
	// odblokowanie przerwan
	TIMSK2 |= (1 << OCIE2A);
	OCR2A = 78; //dla 20 MHz
	//OCR2A = 39; // 8 MHz
} // END void Timer2Init

void PCINTInit() {
	SQW_DDR &= ~SQW_ADDR;

	PCICR |= (1 << PCIE1) | (1 << PCIE2); // unlock interrupt group 8-14 and 16-23
	PCMSK1 |= (1 << PCINT9); // SQW
	PCMSK2 |= (1 << PCINT23); // Bluetooth
} // END void PCINTInit()

void PowerReduction() {
	PRR |= (1 << PRTIM1); // turn off Timer1
}
