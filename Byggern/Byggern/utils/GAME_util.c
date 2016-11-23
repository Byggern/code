/*
* GAME_util.c
*
* Created: 20/11/2016 19:56:35
*  Author: Magne
*/
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#define F_CPU 4912000UL
#include <util/delay.h>
#include "../drivers/HID_driver.h"
#include "../drivers/CAN_driver.h"
#include "../drivers/OLED_driver.h"
#include "../utils/OLED_utils.h"
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
	*slider_ptr = HID_read_slider(RIGHT_SLIDER);
	// Link game_msg to message_buf
	game_msg.length = 7;
	game_msg.data = message_buf;
	
	// Send controls
	CAN_send_message(1, &game_msg);
}

uint8_t GAME_check_miss(void) {
	if(message_received) {
		switch(CAN_receive_buf.data[0]) {
			case GAME_MISS:
				message_received = false;
				return 1;
		
			default:
				message_received = false;
				return 0;
		}
	} else {
		return 0;
	}
}

const char game_start_msg[] PROGMEM = "Welcome to Pong!";
const char lives_left[] PROGMEM = "Health left: ";

const char hearts[] PROGMEM = "<3";
const char game[] PROGMEM = "GAME";
const char over[] PROGMEM = "OVER";

void GAME_draw_status(uint8_t health, uint16_t time_alive) {
	OLED_clear_screen();
	OLED_set_cursor(0,2*8);
	OLED_write_string_P(lives_left);
	
	OLED_set_cursor(3,1*8);
	for( uint8_t i = 0 ; i < health;i++){
		OLED_write_string_P(hearts);
		OLED_draw();
	}
}

void GAME_func(void){
	GAME_restart();
	uint8_t health = 7;
	OLED_lr_bar_clear();
	OLED_clear_screen();
	GAME_draw_status(health, 0);
	while( health > 0){
		if ( GAME_check_miss()){
			health--;
			GAME_draw_status(health, 0);
		}
		
		GAME_send_controls();
	}
	for ( uint8_t i = 0 ; i < 10 ; i++ ){
		OLED_clear_screen();
		OLED_set_cursor(3,6*8);
		OLED_typewrite_string_P(game);
		OLED_set_cursor(4,6*8);
		OLED_typewrite_string_P(over);
		_delay_ms(200);
		OLED_clear_screen();
		_delay_ms(200);
	}
}


void GAME_stop(void){
	message_buf[0]=GAME_STOP;
	game_msg.length=1;
	game_msg.id = 0;
	CAN_send_message(0,&game_msg);
}

void GAME_restart(void){
	message_buf[0]=GAME_RESTART;
	game_msg.length=1;
	game_msg.id = 0;
	CAN_send_message(0,&game_msg);
}

void GAME_calibrate_board(void){
	message_buf[0]=GAME_CALIBRATE_BOARD;
	game_msg.length=1;
	game_msg.id = 0;
	CAN_send_message(0,&game_msg);
	_delay_ms(1000);
}