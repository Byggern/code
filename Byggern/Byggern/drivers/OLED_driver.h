#pragma once
#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

void OLED_init();
void OLED_reset();
void OLED_draw();

extern uint8_t * OLED_vram;

void OLED_set_cursor(uint8_t page, uint8_t column);
void OLED_write_char(uint8_t character);
void OLED_clear_screen(void);
void OLED_write_string(const char* string);

#endif /* OLED_DRIVER_H_ */