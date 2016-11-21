#include <stdint.h>
#include <stdio.h>
#define F_CPU 4912000UL
#include <util/delay.h>
#include <math.h>

#include <avr/pgmspace.h>
#include "../macros.h"
#include "avr/io.h"
#include "OLED_driver.h"
#include "../utils/SRAM_utils.h"
#include "../utils/FONTS_utils.h"
#include "../utils/OLED_utils.h"

volatile uint8_t* OLED_command_address = (uint8_t*)0x1000;
volatile uint8_t* OLED_data_address = (uint8_t*)0x1200;

uint8_t OLED_page;
uint8_t OLED_column;

void OLED_command(uint8_t command) {
	*OLED_command_address = command;
}

uint8_t * OLED_vram = NULL;
void OLED_init() {
	OLED_reset();
	if (!OLED_vram) {
		OLED_vram = SRAM_allocate(1024);
	}
	OLED_magic();
}

void OLED_reset() {
	OLED_command(0xae); // display off
	OLED_command(0xa1); // map segments to  0-128 in instead of 128-256
	OLED_command(0xda); // common pads hardware: alternative
	OLED_command(0x12);
	OLED_command(0xc8); // common output scan direction:com63~com0
	OLED_command(0xa8); // multiplex ration mode:63
	OLED_command(0x3f);
	OLED_command(0xd5); // display divide ratio/osc. freq. mode
	OLED_command(0x80);
	OLED_command(0x81); // contrast control
	OLED_command(0x50);
	OLED_command(0xd9); // set pre-charge period
	OLED_command(0x21);
	OLED_command(0x20); // set memory addressing mode
	OLED_command(0x00); 		// set memory adressing mode to horizontal
	OLED_command(0x21); // set column address
	OLED_command(0x00);		// start address
	OLED_command(0x7f);		// end address
	OLED_command(0x22); // set page address
	OLED_command(0x00); 	// start address
	OLED_command(0x07); 	// end address
	OLED_command(0xdb); // VCOM deselect level mode
	OLED_command(0x30);
	OLED_command(0xad); // master configuration
	OLED_command(0x00);
	OLED_command(0xa4); // out follows RAM content
	OLED_command(0xa6); // set normal display
	OLED_command(0xaf); // display on
}

void OLED_draw() {
	for (uint16_t i = 0; i < 512; i++) {
		*OLED_data_address = OLED_vram[i]%256;
	}
	for (uint16_t i = 0; i < 512; i++) {
		*OLED_data_address = OLED_vram[i+512]%256;
	}
}

void OLED_set_cursor(uint8_t page, uint8_t column) {
	if ((page <= 7) && (column <= 127)) {
		OLED_page = page;
		OLED_column = column;
	}
}

void OLED_cursor_increment(uint16_t steps) {
	OLED_column += steps;
	OLED_page += OLED_column / 128;
	OLED_column %= 128;
	OLED_page %= 8;
}

void OLED_write_char(uint8_t character) {
	uint16_t bufferIndex = 0;
	for (uint16_t col = 0; col < 8; col++) {
		OLED_vram[col + OLED_column + (128 * OLED_page)] = pgm_read_byte(font[character - 32] + bufferIndex);
		bufferIndex += sizeof(uint8_t);
	}
}

void OLED_clear_screen(void) {
	for (int i = 0; i < 1024; i++) {
		OLED_vram[i] = 0;
	}
}

void OLED_write_string(const char* string) {
	int i = 0;
	while(string[i] != '\0') {
		OLED_write_char(string[i]);
		OLED_cursor_increment(8);
		i++;
	}
}

void OLED_write_string_P(const char* pgmptr) {
	char current;
	int i = 0;
	while((current = pgm_read_byte(pgmptr+i))) {
		OLED_write_char(current);
		i += sizeof(char);
		OLED_cursor_increment(8);
	}
}

void OLED_typewrite_string_P(const char* pgmptr) {
	char current;
	int i = 0;
	while((current = pgm_read_byte(pgmptr+i))) {
		OLED_write_char(current);
		i += sizeof(char);
		OLED_cursor_increment(8);
		OLED_write_char('_');
		_delay_ms(20);
		OLED_draw();
	}
	OLED_write_char(' ');
	OLED_draw();
}

void OLED_write_pixel(uint8_t x, uint8_t y, uint8_t state) {
	if ((x > 127) || (y > 63)) {
		return;
	}
	uint8_t page = y / 8;
	uint8_t bit_pos = y % 8;
	uint8_t byte = (1 << bit_pos);
	OLED_set_cursor(page, x);
	if (state) {
		OLED_vram[OLED_column + (128 * OLED_page)] |= byte;
	} else {
		OLED_vram[OLED_column + (128 * OLED_page)] &= ~byte;
	}
}

void OLED_write_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	OLED_write_line_state(x0, y0, x1, y1, 1);
}

void OLED_write_line_state(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t state) {
	if (x0 > x1) {
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}

	int delta_x = x1 - x0;
	int delta_y = y1 - y0;
	int gentleness = delta_x / delta_y;
	if (abs(gentleness) >= 1) {
		if (gentleness >= 0) {
			for (int rel_y = 0 ; rel_y < delta_y; rel_y++) {
				for (int rel_x = 0 ; rel_x < gentleness; rel_x++) {
					OLED_write_pixel(x0 + rel_x + (rel_y * gentleness), y0 + rel_y, state);
				}
			}
		} else {
			for (int rel_y = 0; rel_y > delta_y; rel_y--) {
				for (int rel_x = 0; rel_x < -gentleness; rel_x++) {
					OLED_write_pixel(x0 + rel_x + (rel_y * gentleness), y0 + rel_y, state);
				}
			}
		}
	} else {
		gentleness = delta_y / delta_x;
		if (gentleness >= 0){
			for (int rel_x = 0; rel_x < delta_x; rel_x++){
				for (int rel_y = 0 ; rel_y < gentleness; rel_y++) {
					OLED_write_pixel(x0 + rel_x, y0 + rel_y + rel_x * gentleness, state);
				}
			}
		} else {
			for (int rel_x = 0; rel_x < delta_x; rel_x++) {
				for (int rel_y = 0; rel_y < -gentleness; rel_y++) {
					OLED_write_pixel(x0 + rel_x, y0 + -rel_y + rel_x * gentleness, state);
				}
			}
		}
	}
}
