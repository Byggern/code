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

#include "GAME_util.h"

uint8_t message_buf[8]={};
	
CAN_MESSAGE game_msg = {
	.id = 1,
	.length = 8,
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
	game_msg.length = 8;
	game_msg.data = message_buf;
	
	// Send controls
	CAN_send_message(1, &game_msg);
}

void GAME_check_messages(void) {
	if ( message_received){
		//cli();
		
		switch(CAN_receive_buf.data[0]) {
			case GAME_MISS:

			break;
			
			//sei();
		}
	}
}

void GAME_restart(void) {
	
	message_buf[0] = GAME_RESTART;
	
	game_msg.length = 1;
	game_msg.data = message_buf;
	
	CAN_send_message(1, &game_msg);
}
