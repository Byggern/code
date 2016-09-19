#include "ADC_driver.h"

#include "avr/io.h"

#include <util/delay.h>
#include "../macros.h"

#define F_CPU 4912000UL
#define ADC_ADDRESS (volatile uint8_t*)0x1400


// const uint8_t adc_conv_time = 60; TODO: remove if not used
volatile uint8_t * adc_address = 0x1400;

JOY_CALIBRATE joystick_calibration_values;

void ADC_init(void){
	clear_bit(DDRD,PD5); // Setup a pin to read interrupt line from ADC
}

uint8_t ADC_read_blocking(ADC_CHANNEL channel) {
	ADC_start(channel);
	//_delay_us(adc_conv_time); // Changed to use interrupt line. TODO: remove if not used
	
	while(PIND & (1 << PIND5)) { // Wait for interrupt line from ADC
	}
	
	return ADC_read(channel);
}

void ADC_start(ADC_CHANNEL channel) {
	uint8_t mode_channel = (0b01 << 2) | channel; // ( Single ended mode ) | channel
	*adc_address = mode_channel; // Any value written to this address should activate ADC
}

uint8_t ADC_read(ADC_CHANNEL channel) {
	return *adc_address;
}




/* Joystick and Button Functions - Place in separate driver? */ 
uint8_t ADC_read_joystick(JOY_AXIS axis) {
	uint8_t adc_val = ADC_read_blocking(axis);
	//make som funky function
}

void ADC_calibrate_joystick(void) {
	// ISH VALUES
	joystick_calibration_values.x_axis_min = 0;
	joystick_calibration_values.x_axis_max = 251;
	joystick_calibration_values.y_axis_min = 1;
	joystick_calibration_values.y_axis_max = 251;
	// TODO: Make this interactive, such that it asks the user to press a button while holding the joystick to each side
}