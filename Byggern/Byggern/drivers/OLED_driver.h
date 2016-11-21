#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

void OLED_init();
void OLED_reset();
void OLED_draw();

extern uint8_t* OLED_vram;

void OLED_set_cursor(uint8_t page, uint8_t column);
void OLED_write_char(uint8_t character);
void OLED_clear_screen(void);
void OLED_write_string(const char* string);
void OLED_write_string_P(const char* pgmptr);
void OLED_typewrite_string_P(const char* pgmptr);
void OLED_cursor_increment(uint16_t steps);
void OLED_write_pixel(uint8_t x, uint8_t y, uint8_t state);
void OLED_write_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void OLED_write_line_state(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint8_t state);

#endif /* OLED_DRIVER_H_ */
