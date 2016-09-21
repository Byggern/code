#include <stdint.h>
#include <stdio.h>
#include  "avr/io.h"

#include "HID_driver.h"

JOY_CALIBRATE joystick_calibration_values;

/* Joystick and Button Functions */
uint8_t HID_read_joystick(JOY_AXIS axis) {
	uint8_t adc_val = ADC_read_blocking(axis);
	if(axis == X_AXIS) {
		return adc_val - joystick_calibration_values.x_offset;
	} else {
		return adc_val - joystick_calibration_values.y_offset;
	}
}

void HID_calibrate_joystick(void) {
	joystick_calibration_values.x_axis_min = 0;
	joystick_calibration_values.x_axis_max = 255;
	joystick_calibration_values.x_offset = 0;
	
	joystick_calibration_values.y_axis_min = 0;
	joystick_calibration_values.y_axis_max = 255;
	joystick_calibration_values.y_offset = 0;
	
	HID_joystick_zero(X_AXIS);
	HID_joystick_zero(Y_AXIS);
}

void HID_joystick_zero(JOY_AXIS axis) {
	uint8_t center_sum = 0;
	int i;
	for (i = 0; i < 10000; i++) {
		center_sum += ADC_read_blocking(axis);
	}
	if(axis == X_AXIS) {
		joystick_calibration_values.x_offset = center_sum/10000 - 127;
	} else {
		joystick_calibration_values.y_offset = center_sum/10000 - 127;
	}
}

uint8_t HID_read_slider(SLIDER_BUTTON slider) {
	
}
