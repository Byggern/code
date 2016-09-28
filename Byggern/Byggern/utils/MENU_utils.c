#include "MENU_utils.h"
#include "stdbool.h"
#include "../drivers/HID_driver.h"

/* struct Menu {
char * header;
void (* function)(void);
struct Menu * parent;
struct Menu * submenus;
uint8_t length;
uint8_t position;
};*/

static Menu * current;

void MENU_redraw( Menu * menu, uint8_t position){
	OLED_set_cursor(0,0);
	OLED_write_string(menu->header);
	for ( int i = 0 ; i < menu->length;i++){
		int j=i+1;
		if (i == position) {
			OLED_set_cursor(0,j);
			OLED_write_string(">");
		}
		OLED_set_cursor(8,j);
		OLED_write_string(menu->submenus[i]->header);
	}
}



void MENU( Menu * menu){
	uint8_t position = 1;
	if (menu->function){
		function();
		//menu_redraw(menu)
	} else if (menu->submenus) {
		MENU_redraw(menu, position);
		bool not_finished = true;
		JOY_DIR prev_direction = HID_read_joystick_direction();
		JOY_DIR curr_direction = prev_direction;
		while (not_finished) {
			if (HID_read_joy_button()) {
				if (position == 0) {
					/* header clicked */
					not_finished = false;
				} else {
					MENU(menu->submenus[position]);
				}
			} else {
				curr_direction = HID_read_joystick_direction();
				if (prev_direction != curr_direction) {
					prev_direction = curr_direction;
					if (curr_direction == UP) {
						position++;
					} else if (curr_direction == DOWN) {
						position--;
					}
					if (curr_direction == UP || curr_direction == DOWN) {
						MENU_redraw(menu, position);
					}
				}
			}
		}
	}
	// go to parent
	return;
}