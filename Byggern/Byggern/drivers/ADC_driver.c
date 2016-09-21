#include "ADC_driver.h"

#include "avr/io.h"

#define F_CPU 4912000UL
#include <util/delay.h>
#include "../macros.h"


#define ADC_CONV_TIME 40

// const uint8_t adc_conv_time = 60; TODO: remove if not used
volatile uint8_t * adc_address = 0x1400;

void ADC_init(void){
	clear_bit(DDRD,PD5); // Setup a pin to read interrupt line from ADC
}

uint8_t ADC_read_blocking(ADC_CHANNEL channel) {
	ADC_start(channel);
	_delay_us(ADC_CONV_TIME); // Wait for conversion to finish
	
	/*
	while(PIND & (1 << PIND5)) { // Wait for interrupt line from ADC
	}
	*/
	
	return ADC_read(channel);
}

void ADC_start(ADC_CHANNEL channel) {
	uint8_t mode_channel = (0b01 << 2) | channel; // ( Single ended mode ) | channel
	*adc_address = mode_channel; // Any value written to this address should activate ADC
}

uint8_t ADC_read(ADC_CHANNEL channel) {
	return *adc_address;
}
