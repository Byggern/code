#pragma once
#include <stdint.h>
#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

typedef enum {
	CHANNEL_0 = 0,
	CHANNEL_1,
	CHANNEL_2,
	CHANNEL_3
} ADC_CHANNEL;

void ADC_init(void);

uint8_t ADC_read_blocking(ADC_CHANNEL channel);

void ADC_start(ADC_CHANNEL channel);

uint8_t ADC_read(ADC_CHANNEL channel);

uint8_t ADC_test(void);

#endif /* ADC_DRIVER_H_ */