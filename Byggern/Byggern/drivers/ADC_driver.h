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

typedef enum {
	X_AXIS = 0,
	Y_AXIS
	} JOY_AXIS;
	
typedef struct {
	uint8_t x_axis_max;
	uint8_t x_axis_min;
	uint8_t y_axis_max;
	uint8_t y_axis_min;
} JOY_CALIBRATE;

void ADC_init(void);

uint8_t ADC_read_blocking(ADC_CHANNEL channel);

void ADC_start(ADC_CHANNEL channel);

uint8_t ADC_read(ADC_CHANNEL channel);

uint8_t ADC_read_joystick(JOY_AXIS axis);

void ADC_calibrate_joystick(void);

#endif /* ADC_DRIVER_H_ */