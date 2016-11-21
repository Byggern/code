#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "../../Byggern/utils/GAME_util.h"
#include "../../Byggern/drivers/CAN_driver.h"
#include "../../Byggern/drivers/HID_driver.h"
#include "../drivers/ADC_driver.h"
#include "../drivers/PWM_driver.h"
#include "../drivers/MOT_driver.h"
#include "../drivers/SOL_driver.h"

#include "GAME2_util.h"

volatile bool can_timeslot_free;


#define Kp 1./30
#define Ki 1./2000
#define POS_SCALER 40
static int16_t regulator_integrand = 0;
int16_t reference_motor_position = 0;

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
	
	MOT_init();
}

void GAME2_check_messages(void) {
	if (message_received) {

		switch(CAN_receive_buf.data[0]) {
			
			case GAME_CONTROLS: {
				JOY_VALS joystick_vals = *(JOY_VALS*)&CAN_receive_buf.data[1];
				uint8_t button = (volatile uint8_t)CAN_receive_buf.data[5];
				uint8_t slider = (volatile uint8_t)CAN_receive_buf.data[6];

				// Set motor reference position
				GAME2_set_pos(~slider);
				
				// Possibly fire solenoid
				if (button) {
					SOL_fire();
				}
				
				// Set servo position
				PWM_set_duty((-joystick_vals.x_axis / 2) + 50);
				
				break;
			}
			case GAME_RESTART: {
				regulator_integrand = 0;
				reference_motor_position = MOT_read_encoder();
				MOT_enable();
				break;
			}
			case GAME_STOP: {
				MOT_disable();
				break;
			}
			case GAME_CALIBRATE_BOARD:
				MOT_enable();
				MOT_set_direction(MOTOR_RIGHT);
				MOT_set_speed(100);
				_delay_ms(1000);
				MOT_set_speed(0);
				MOT_reset_encoder();
				regulator_integrand = 0;
				reference_motor_position = MOT_read_encoder();
				MOT_disable();
				break;
			default: {
				
			}
			
		}
		message_received = false;
	}
}

void GAME2_set_pos(uint8_t position) {
	reference_motor_position = position;
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

void GAME2_check_sensors(void) {
	
	// Check IR sensor
	static bool should_send = false;
	static ADC_STATE adc_last = 0;
	
	
	ADC_STATE adc_curr = ADC_state();
	if (adc_curr == LOW && adc_curr != adc_last) {
		should_send = true;
	}
	if (should_send && can_timeslot_free) {
		should_send = false;
		GAME2_send_miss();
		printf("Sending miss\n");
	}
	adc_last = adc_curr;
}

void GAME2_update_regulator(void) {
	int16_t motor_position = 0;
	const uint16_t time_constant_inv = 10;
	
	// Update motor position
	motor_position = MOT_read_encoder();
	
	// Calculate position error
	int16_t pos_error = (reference_motor_position * POS_SCALER) - motor_position;
	
	// Update I-term
	regulator_integrand += pos_error * time_constant_inv;
	
	// Calculate motor output
	int16_t output = (pos_error * Kp) + (Ki  * regulator_integrand);
	
	// Set motor output
	if (output > 0) {
		MOT_set_direction(MOTOR_LEFT);
	} else {
		MOT_set_direction(MOTOR_RIGHT);
	}
	MOT_set_speed(abs(output) + 40);
}

// CAN bus flag
ISR(TIMER4_COMPA_vect) {
	// Turn timer off
	TCCR4B  &= ~(0b111 << CS40);
	// Free CAN bus
	can_timeslot_free = true;
	// Clear interrupt flag
	TIFR4 &= ~(1 << OCF4A);
}