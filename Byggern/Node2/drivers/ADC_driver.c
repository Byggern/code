#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

#include "../Node2/drivers/ADC_driver.h"


uint16_t adc_tail = 0;
uint16_t adc_high = 200;
uint16_t adc_low  = 50;

ADC_STATE ADC_current;

float frontweight = 0.2;

uint16_t ADC_smooth(){
	adc_tail = adc_tail*(1-frontweight) + ADC_read(CHANNEL_0)*frontweight;
	return adc_tail;
}

ADC_STATE ADC_state(){
	uint16_t val = ADC_smooth();
	if ( ADC_current == LOW){
		if (val > adc_high){
			ADC_current = HIGH;
		}
	}else{
		if (val < adc_low){
			ADC_current = LOW;
		}
	}
	return ADC_current;
}

void ADC_init(void) {
	DDRF &= ~(1 << PF0);
	ADCSRA |= (1 << ADEN) | (0b111 << ADPS0);
	ADMUX  |=  (1 << REFS1) | (1 << REFS0);;
}

void ADC_start(ADC_CHANNEL channel) {
	// Choose correct channel
	ADMUX |= (channel & 0b111);
	// start conversion
	ADCSRA |= (1 << ADSC);
}

uint16_t ADC_read(ADC_CHANNEL channel){
	ADC_start(channel);
	// Wait for ADC to finish
	_delay_us(300);
	return ADC;
};
