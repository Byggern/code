#include "ADC_driver.h"
#define F_CPU 4912000UL
#include <util/delay.h>


#define ADC_ADDRESS (volatile uint8_t*)0x400

const uint8_t adc_conv_time = 100;

void ADC_init(void){
	
}

uint8_t ADC_read_blocking(ADC_CHANNEL channel){
	ADC_start(channel);
	_delay_us(adc_conv_time);
	return ADC_read(channel);
}

void ADC_start(ADC_CHANNEL channel){
	volatile uint8_t *adc_address = 0x400;
	uint8_t mode_channel = (0b01<<2)|channel;
	*adc_address = mode_channel; // any value written to ADC should activate it
}
uint8_t ADC_read(ADC_CHANNEL channel){
	
	volatile uint8_t volatile *adc_address = 0x400;
	return *adc_address;
}