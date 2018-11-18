/*!
 * @file text_eeprom.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see text_eeprom.h*/

#include "text_eeprom.h"

char EEMEM eeTextView[TEXT_EEPROM_SIZE][1] = {"time and date"};
char ctTextView[TEXT_EEPROM_SIZE][1];

uint8_t EEMEM eeTextParams[TEXT_EEPROM_SIZE][2] = {{1, 5}, {1, 5}, {1, 5}, {1, 5}, {1, 5}, {1, 5}};
uint8_t uitTextViewParams[TEXT_EEPROM_SIZE][2];

/*! @param 		addr of text to write from RAM to EEPROM */
void WriteTextToEEProm(uint8_t addr) {
	if (addr < TEXT_EEPROM_SIZE) {
		eeprom_update_block(&uitTextViewParams[addr], &eeTextParams[addr], 2);
		eeprom_busy_wait();

		eeprom_update_block(&ctTextView[addr], &eeTextView[addr], 1);
		eeprom_busy_wait();
	}
} // END void WriteTextToEEProm

void TextEEPromInit() {
	for (uint8_t i = 0; i < TEXT_EEPROM_SIZE; i++) {
		eeprom_read_block(&ctTextView[i], &eeTextView[i], 1);
		eeprom_busy_wait();

		eeprom_read_block(&uitTextViewParams[i], &eeTextParams[i], 2);
		eeprom_busy_wait();
	}
} // END void TextEEPromInit
