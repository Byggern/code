#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "MENU_utils.h"
#include "../drivers/HID_driver.h"
#include "../drivers/OLED_driver.h"
#include "../utils/OLED_utils.h"
#include "DEBUG_utils.h"

const char mainmenu[] PROGMEM = "Main Menu\0";
const char utilsmenu[] PROGMEM = "Utilities\0";
const char hellomenu[] PROGMEM = "Test function\0";
const char calibratemenu[] PROGMEM = "Calibrate Joystick\0";
const char debugmenu[] PROGMEM = "Show debug output\0";
const char wipemenu[] PROGMEM = "Wipe screen\0";

Menu menus[MENU_COUNT] = {
	{.header = mainmenu,		.function = NULL,					.length=2},
	{.header = utilsmenu,		.function = NULL,					.length=3},
	{.header = hellomenu,		.function = DEBUG_OLED_hello,		.length=0},
	{.header = calibratemenu,	.function = HID_calibrate_joystick, .length=0},
	{.header = debugmenu,		.function = DEBUG_run_HID_debug,	.length=0},
	{.header = wipemenu,		.function = OLED_lr_bar_clear,		.length=0}
};

void MENU_link_menus(void) {
	menus[0].submenus[0] = &menus[1];
	menus[0].submenus[1] = &menus[2];
	menus[1].submenus[0] = &menus[3];
	menus[1].submenus[1] = &menus[4];
	menus[1].submenus[2] = &menus[5];
}

void MENU_init(void) {
	MENU_link_menus();
}

void MENU_redraw(Menu* menu, uint8_t position) {
	OLED_clear_screen();
	OLED_set_cursor(0, 8);
	OLED_write_string_P(menu->header);
	for (int i = 0 ; i < (menu->length + 1); i++) {
		if (i == position) {
			OLED_set_cursor(i, 0);
			if (position) {
				OLED_write_string(">");
			} else {
				OLED_write_string("<");
			}
		}
	}
	for (int i = 0; i < menu->length; i++) {
		OLED_set_cursor(i + 1,16);
		OLED_write_string_P(menu->submenus[i]->header);
	}
	OLED_draw();
}

void MENU(Menu* menu) {
	uint8_t position = 1;
	if (menu->function) {
		menu->function();
	} else if (menu->submenus) {
		MENU_redraw(menu, position);
		bool not_finished = true;

		/* Button state vars */
		JOY_DIR prev_direction = HID_read_joystick_direction_change();
		JOY_DIR curr_direction = prev_direction;
		uint8_t prev_button = HID_read_touch_button(LEFT_BUTTON);
		uint8_t curr_button = prev_button;

		while (not_finished) {
			curr_button = HID_read_touch_button(LEFT_BUTTON);
			JOY_DIR curr_direction = HID_read_joystick_direction_change();

			if (curr_button && curr_button != prev_button) {
				/* draw star indicating 'busy' */
				OLED_set_cursor(position, 8);
				OLED_write_char('*');
				OLED_draw();
				/* enter child menu */
				MENU(menu->submenus[position - 1]);
				/* redraw to remove star */
				MENU_redraw(menu,position);
				prev_button = curr_button;
			} else if (!curr_button && curr_button != prev_button) {
				prev_button = curr_button;
			} else if (curr_direction != NOCHANGE) {
				switch(curr_direction) {
					case RIGHT:
						if (position == 0) {
							not_finished = true;
						} else {
							/* draw star indicating 'busy' */
							OLED_set_cursor(position,8);
							OLED_write_char('*');
							OLED_draw();
							/* enter child menu */
							MENU(menu->submenus[position - 1]);
							/* redraw to remove star */
							MENU_redraw(menu,position);
						}
						break;
					case LEFT:
						not_finished = false;		
					case DOWN:
						if (position < menu->length) {
							position++;
							MENU_redraw(menu, position);
						}
						break;
					case UP:
						if (position - 1 >= 0) {
							position--;
							MENU_redraw(menu, position);
						}
						break;
					case CENTER:
						break;
					default:
						break;
				}
			}
		}
	}
	// go to parent menu
	return;
}
