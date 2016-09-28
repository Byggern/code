#include "ADC_driver.h"

#include "avr/io.h"

#define F_CPU 4912000UL
#include <util/delay.h>
#include <stdio.h>
#include "../macros.h"

#define ADC_CONV_TIME 40

// const uint8_t adc_conv_time = 60; TODO: remove if not used
volatile uint8_t * adc_address = 0x1400;

void ADC_init(void){
	printf("Initializing ADC...\n");
	clear_bit(DDRD,PD5); // Setup a pin to read interrupt line from ADC
	if (ADC_test() == 1) {
		printf("ADC initialized.\n");
	} else {
		printf("[!] ADC initialization failed!\n");
	}
}

uint8_t ADC_test(void) {
	printf("Running ADC test...\n");
	int i;
	uint16_t ADC_test_sum = 0;
	uint8_t ADC_equal_last_sum = 0;
	uint8_t ADC_current_value = 0;
	uint8_t ADC_previous_value = ADC_read_blocking(CHANNEL_0);
	
	for (i = 0; i < 100; i++) {
		ADC_current_value = ADC_read_blocking(CHANNEL_0);
		ADC_test_sum += ADC_current_value;
		if (ADC_previous_value == ADC_current_value) {
			ADC_equal_last_sum++;
		}
		ADC_previous_value = ADC_current_value;
	}
	
	//printf("ADC running test sum: %d\n", ADC_test_sum);

	//printf("Number of consequent equal values: %d\n", ADC_equal_last_sum);
	
	if (ADC_equal_last_sum > 95 || ADC_test_sum < 5) {
		/* fail the test */
		printf("ADC test failed!\n");
		return 0;
		} else {
		/* pass the test */
		printf("ADC test passed.\n");
		return 1;
	}
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
