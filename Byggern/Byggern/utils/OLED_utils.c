#define F_CPU 4912000UL
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "../drivers/OLED_driver.h"

const char byggern[] PROGMEM = "BYGGERN\0";
const char copy[] PROGMEM = "(C) Gruppe 27\0";
const char year[] PROGMEM = "2016\0";

void OLED_lr_bar(uint8_t x) {
	OLED_write_line_state(x, 0, x + 1, 64, 1);
	OLED_write_line_state(x - 1, 0, x, 64, 0);
	OLED_draw();
}

void OLED_lr_bar_clear(void) {
	for (uint8_t x = 1; x < 127;x++) {
		OLED_lr_bar(x);
	}
}

void OLED_magic(void) {
	for (uint8_t c = 0; c < 96; c++) {
		OLED_clear_screen();
		OLED_draw();
		OLED_set_cursor(0, 0);
		for (uint8_t pos = 0; pos < 96; pos++) {
			if (pos == c) {
				OLED_write_char(c);
			}
			OLED_cursor_increment(8);
		}
		OLED_set_cursor(2, 40);
		OLED_write_string_P(byggern);
		OLED_draw();
	}
	OLED_clear_screen();
	OLED_set_cursor(2, 32);
	OLED_typewrite_string_P(byggern);
	OLED_set_cursor(4, 10);
	OLED_typewrite_string_P(copy);
	OLED_set_cursor(5, 50);
	OLED_typewrite_string_P(year);
	_delay_ms(1000);
	OLED_lr_bar_clear();
}