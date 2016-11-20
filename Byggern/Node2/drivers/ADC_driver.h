#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <stdint.h>

typedef enum {
	CHANNEL_0 = 0,
	CHANNEL_1,
	CHANNEL_2,
	CHANNEL_3
} ADC_CHANNEL;


typedef enum {
	LOW,
	HIGH
} ADC_STATE;

void ADC_init(void);

void ADC_start(ADC_CHANNEL channel);

uint16_t ADC_read(ADC_CHANNEL channel);

uint16_t ADC_smooth();

ADC_STATE ADC_state();

#endif /* ADC_DRIVER_H_ */