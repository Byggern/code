#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

#include "PWM_driver.h"

void PWM1_start(void) {
	// Waveform Generation mode: Fast PWM
	uint8_t wgm1 = 0b1110;
	
	TCCR3A |= ((1 << WGM31) | (1 << COM3A1));
	TCCR3B |= ((1 << WGM33) | (1 << WGM32));
	TCCR3B |= (0b010 << CS30);
	TCCR3C = 0;
	
	DDRE |= (1 << DDE3);
	// Set top value
	ICR3 = PWM_TOP_VAL;
	OCR3A = PWM_DUTY_MID;
}

void PWM_set_duty(uint8_t duty) {
	uint16_t duty_ticks = PWM_DUTY_OFFSET +  (uint16_t)duty * 24;
	if(duty_ticks < 1800) {
		duty_ticks = 1800;
	} else if (duty_ticks > 4200) {
		duty_ticks = 4200;
	}
	OCR3A = duty_ticks;
}

void PWM_stop(void) {
	TCCR3B &= ~(0b111 << CS30);
}