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
	Y_ON();


	H_DATA_DDR |= H_DATA_ADDR;
	M_DATA_DDR |= M_DATA_ADDR;
	//S_DATA_DDR |= S_DATA_ADDR;
	TIME_CLK_DATA_DDR |= TIME_CLK_DATA_ADDR;

	ClearRegistersTime(true);
} // END void RegistersInit

void RegistersTest() {
	RegistersInit();
	uint8_t i;
	while(1) {
		for (i = 0; i < 10; i++) {
			SendRegistersTime(0, i, 0, true);
			D_MS(1000);
			SendRegistersTime(0, i * 10, 0, true);
			D_MS(1000);
			SendRegistersTime(i, 0, 0, true);
			D_MS(1000);
			SendRegistersTime(i * 10, 0, 0, true);
			D_MS(1000);
		}
	}
} // END void RegistersTest()







