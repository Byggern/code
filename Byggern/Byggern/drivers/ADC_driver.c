#include "ADC_driver.h"

#include "avr/io.h"
#include <avr/pgmspace.h>

#define F_CPU 4912000UL
#include <util/delay.h>
#include <stdio.h>
#include "../macros.h"

const char adc_init_msg[] PROGMEM =  "Initializing ADC...\n";
const char adc_init_done_msg[] PROGMEM = "ADC initialized.\n";
const char adc_init_fail_msg[] PROGMEM = "[!] ADC initialization failed!\n";
const char adc_init_run_test_msg[] PROGMEM = "Running ADC test...\n";
const char adc_test_pass_msg[] PROGMEM = "ADC test passed.\n";
const char adc_test_fail_msg[] PROGMEM = "ADC test failed!\n";

#define ADC_CONV_TIME 40

volatile uint8_t* adc_address = (uint8_t*)0x1400;

void ADC_init(void) {
	printf_P(adc_init_msg);
	clear_bit(DDRD, PD5); // Setup a pin to read interrupt line from ADC
	if (ADC_test() == 1) {
		printf_P(adc_init_done_msg);
	} else {
		printf_P(adc_init_fail_msg);
	}
}

uint8_t ADC_test(void) {
	printf_P(adc_init_run_test_msg);
	uint16_t ADC_test_sum = 0;
	uint8_t ADC_equal_last_sum = 0;
	uint8_t ADC_current_value = 0;
	uint8_t ADC_previous_value = ADC_read_blocking(CHANNEL_0);
	
	int i;
	for (i = 0; i < 100; i++) {
		ADC_current_value = ADC_read_blocking(CHANNEL_0);
		ADC_test_sum += ADC_current_value;
		if (ADC_previous_value == ADC_current_value) {
			ADC_equal_last_sum++;
		}
		ADC_previous_value = ADC_current_value;
	}
	
	if (ADC_equal_last_sum > 95 || ADC_test_sum < 5) {
		printf_P(adc_test_fail_msg);
		return 0;
	} else {
		printf_P(adc_test_pass_msg);
		return 1;
	}
}

uint8_t ADC_read_blocking(ADC_CHANNEL channel) {
	ADC_start(channel);
	_delay_us(ADC_CONV_TIME); // Wait for conversion to finish
	return ADC_read(channel);
}

void ADC_start(ADC_CHANNEL channel) {
	uint8_t mode_channel = (0b01 << 2) | channel; // ( Single ended mode ) | channel
	*adc_address = mode_channel; // Any value written to this address should activate ADC
}

uint8_t ADC_read(ADC_CHANNEL channel) {
	return *adc_address;
}
