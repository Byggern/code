/*
* GAME_util.c
*
* Created: 20/11/2016 19:56:35
*  Author: Magne
*/
#include <stdint.h>
#include <avr/io.h>
#include "../drivers/HID_driver.h"
#include "../drivers/CAN_driver.h"
#include "../drivers/OLED_driver.h"
#include "../utils/OLED_utils.h"
#include "avr/pgmspace.h"
#include "GAME_util.h"

uint8_t message_buf[8]={};

CAN_MESSAGE game_msg = {
	.id = 1,
	.length = 7,
	.data = message_buf
};

void GAME_send_controls(void) {
	
	message_buf[0] = GAME_CONTROLS;
	
	// Link variables to message positions
	JOY_VALS *joy_ptr = &message_buf[1];
	uint8_t * button_ptr = &message_buf[5];
	uint8_t * slider_ptr = &message_buf[6];
	
	// Collect values
	*joy_ptr    = HID_read_joystick();
	*button_ptr = HID_read_touch_button(LEFT_BUTTON);
	*slider_ptr = HID_read_slider(LEFT_SLIDER);
	// Link game_msg to message_buf
	game_msg.length = 7;
	game_msg.data = message_buf;
	
	// Send controls
	CAN_send_message(1, &game_msg);
}

uint8_t GAME_check_miss(void) {
	if ( message_received){
		
		switch(CAN_receive_buf.data[0]) {
			
			case GAME_MISS:
			message_received = false;
			return 1;
			
			default:
			message_received = false;
			return 0;
		}
	}
}

const char game_start_msg[] PROGMEM = "Welcome to Pong!";
const char lives_left[] PROGMEM = "Health left: ";

const char hearts[] PROGMEM = "<3";
const char game[] PROGMEM = "GAME";
const char over[] PROGMEM = "OVER";

void GAME_func(void){
	uint8_t health = 5;
	OLED_lr_bar_clear();
	OLED_clear_screen();
	
	OLED_set_cursor(1,0);
	for( uint8_t i = 0 ; i < health;i++){
		OLED_write_string_P(hearts);
		OLED_draw();
	}
	http://documentiation.url.here
	while( health > 0){
		
		if ( GAME_check_miss()){
			health--;
			
			OLED_clear_screen();
			
			OLED_set_cursor(0,0);
			OLED_write_string_P(lives_left);
			
			OLED_set_cursor(1,8);
			for( uint8_t i = 0 ; i < health;i++){
					OLED_write_string_P(hearts);
					OLED_draw();
			}
		}
		
		GAME_send_controls();
	}
	OLED_clear_screen();
	OLED_set_cursor(3,4);
	OLED_typewrite_string_P(game);
	OLED_set_cursor(4,4);
	OLED_typewrite_string_P(over);
}

void GAME_restart(void) {

	message_buf[0] = GAME_RESTART;

	game_msg.length = 1;
	game_msg.data = message_buf;

	CAN_send_message(1, &game_msg);
}