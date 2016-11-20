/*
* GAME_util.c
*
* Created: 20/11/2016 21:13:46
*  Author: Magne
*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../Byggern/utils/GAME_util.h"
#include "../../Byggern/drivers/CAN_driver.h"
#include "../../Byggern/drivers/HID_driver.h"
#include "../drivers/ADC_driver.h"
#include "../drivers/MOT_driver.h"

#include "GAME2_util.h"

volatile bool can_timeslot_free;
volatile int16_t reference_motor_position = 0;

void GAME2_init(void) {
	
	// Setup a timer to avoid too frequent CAN messages to slow Node 1
	TCCR4A |= ((0 << WGM41) | (1 << COM4A1));
	TCCR4B |= ((0 << WGM43) | (0 << WGM42));
	TCCR4B |= (0b010 << CS40);
	TCCR4C = 0;
	
	// Set output compare value
	OCR4A = 10000; // 5ms
	
	// Enable output compare A interrupt
	TIMSK4 |= (1 << OCIE4A);
	sei();
	
	can_timeslot_free = true;
}

void GAME2_check_messages(void) {
	if ( message_received){
		//cli();
		
		switch(CAN_receive_buf.data[0]) {
			case GAME_CONTROLS:
			/*JOY_VALS * joystick_vals;
			uint8_t slider;
			uint8_t button
			joystick_vals = CAN_receive_buf.data[1];
			button = CAN_receive_buf.data[5];
			slider = CAN_receive_buf.data[6];
			
			printf("Slider val: %d", slider);
			slider = slider - 128;
			message_received = false;
			*/
			break;
			
			case GAME_RESTART:
			break;
			
			//sei();
		}
	}
}

void GAME2_send_miss(void) {
	
	uint8_t miss_cmd = GAME_MISS;
	
	CAN_MESSAGE miss_message = {
		.id = 0,
		.length = 1,
		.data = &miss_cmd
	};
	
	// Take over CAN bus
	can_timeslot_free = false;
	
	// Send message
	CAN_send_message(0, &miss_message);
	
	// Reset timer value
	TCNT5 = 0;
	// Turn timer on again
	TCCR4B |= (0b010 << CS40);
}


//uint16_t adc_min = 1024;
//uint16_t adc_max = 0x0;

void GAME2_check_sensors(void) {
	
	// Check IR sensor
	static bool should_send = false;
	static ADC_STATE adc_last = 0;
	
	
	ADC_STATE adc_curr = ADC_state();
	if (adc_curr && adc_curr != adc_last) {
		should_send = true;	
	}
	if (should_send && can_timeslot_free) {
		should_send = false;
		GAME2_send_miss();
		printf("Sending miss");
	}
	adc_last = adc_curr;
}

#define Kp 1
#define Ki 1

void GAME2_update_regulator(void) {
	static int16_t regulator_integrand = 0;
	static int16_t motor_position = 0;
	const uint16_t time_constant_inv = 10;
	
	// Update motor position
	int16_t position_difference = MOT_read_encoder();
	motor_position += position_difference;
	
	// Calculate position error
	int16_t pos_error = reference_motor_position - motor_position;
	
	// Update I-term
	regulator_integrand += pos_error * time_constant_inv;
	
	// Calculate motor output
	int16_t output = (Kp * pos_error) + (Ki * regulator_integrand);
	
	// Set motor output
	MOT_set_speed(abs(output));
	if (output > 0) {
		MOT_set_direction(MOTOR_RIGHT);
	} else {
		MOT_set_direction(MOTOR_LEFT);
	}
	printf("Reg output: %d\n", output);
}

// CAN bus flag // NEED more comment
ISR(TIMER4_COMPA_vect) {
	
	// Turn timer off
	TCCR4B  &= ~(0b111 << CS40);
	// Free CAN bus
	can_timeslot_free = true;
	// Clear interrupt flag
	TIFR4 &= ~(1 << OCF4A);
	

}