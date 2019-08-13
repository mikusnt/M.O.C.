/*!
 * @file register.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see register.h*/

#include "register.h"


/*! initialize IO registers to work which 74HC595*/
void RegistersInit() {
	/*Y_DATA_DDR |= Y_DATA_ADDR;
	Y_CLK_DATA_DDR |= Y_CLK_DATA_ADDR;
	Y_RESET_DDR |= Y_RESET_ADDR;*/

	LATCH_DDR |= LATCH_ADDR;
	//Y_ON();

	SECOND_DATA_DDR |= SECOND_DATA_ADDR;
	FIRST_DATA_DDR |= FIRST_DATA_ADDR;
	if (WITH_SECONDS)
		THIRD_DATA_DDR |= THIRD_DATA_ADDR;
	//S_DATA_DDR |= S_DATA_ADDR;
	TIME_CLK_DATA_DDR |= TIME_CLK_DATA_ADDR;

	ClearRegistersTime(true);
} // END void RegistersInit

void RegistersTest() {
	RegistersInit();
	uint8_t i;
	uint16_t time = 1000;
	while(1) {
		for (i = 1; i < 10; i++) {
			if (WITH_SECONDS) {
				SendRegistersTime(0, 0, i, true);
				D_MS(time);
				SendRegistersTime(0, 0, i * 10, true);
				D_MS(time);
			}
			SendRegistersTime(0, i, 0, true);
			D_MS(time);
			SendRegistersTime(0, i * 10, 0, true);
			D_MS(time);
			SendRegistersTime(i, 0, 0, true);
			D_MS(time);
			SendRegistersTime(i * 10, 0, 0, true);
			D_MS(time);
		}
	}
} // END void RegistersTest()







