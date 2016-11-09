#include <stdint.h>

#include <avr/io.h>

#include "../Node2/drivers/ADC_driver.h"


void ADC_init(void) {
	DDRF &= ~(1 << PF0);
	ADCSRA |= (1<<ADEN);
}

void ADC_start(ADC_CHANNEL channel) {
	// Choose correct channel
	ADMUX |= (channel & 0b111);
	// start conversion
	ADCSRA |= (1 << ADSC);
	
}

uint16_t ADC_read(ADC_CHANNEL channel){
	ADC_start(channel);
	int i = 0;
	while(ADCSRA & (1 << ADIF)){
		if (i > 100) {
			printf("ADC read timed out\n");
			return 0;
		}
	}
	return ADC;
};
