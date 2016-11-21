#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

#include "../Node2/drivers/ADC_driver.h"

#define ADC_CONV_TIME 300
uint16_t adc_tail = 0;
#define ADC_HIGH 250
#define ADC_LOW 100
#define FRONT_WEIGHT 0.5

ADC_STATE ADC_current;

uint16_t ADC_smooth() {
	adc_tail = adc_tail * (1 - FRONT_WEIGHT) + ADC_read(CHANNEL_0) * FRONT_WEIGHT;
	return adc_tail;
}

ADC_STATE ADC_state() {
	uint16_t val = ADC_smooth();
	if (ADC_current == LOW) {
		if (val > ADC_HIGH) {
			ADC_current = HIGH;
		}
	} else {
		if (val < ADC_HIGH) {
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
	// choose correct channel
	ADMUX |= (channel & 0b111);
	// start conversion
	ADCSRA |= (1 << ADSC);
}

uint16_t ADC_read(ADC_CHANNEL channel) {
	ADC_start(channel);
	// wait for ADC to finish
	_delay_us(ADC_CONV_TIME);
	return ADC;
}
