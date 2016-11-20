#define F_CPU 4912000UL
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "../drivers/OLED_driver.h"

const char byggern[] PROGMEM = "BYGGERN\0";

void OLED_lr_bar(uint8_t x) {
	OLED_write_line_state(x, 0, x+1, 64, 1);
	OLED_write_line_state(x - 1, 0, x, 64, 0);
	OLED_draw();
}

void OLED_lr_bar_clear() {
	for( uint8_t x= 1; x < 127;x++){
		OLED_lr_bar(x);
	}
}

void OLED_magic(void){
	for (int c = 0; c < 96; c++) {
		OLED_set_cursor(0, 0);
		for (int pos = 0; pos < 96; pos++) {
			OLED_write_char(c);
			OLED_cursor_increment(8);
		}
		OLED_set_cursor(3, 20);
		OLED_write_string_P(byggern);
		OLED_draw();
	}
}