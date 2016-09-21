#include <stdint.h>
#include <stdio.h>
#include  "avr/io.h"

#include "HID_driver.h"
#include "ADC_driver.h"

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
	uint16_t center_sum = 0;
	int i;
	for (i = 0; i < 100; i++) {
		center_sum += ADC_read_blocking(axis);
	}
	if(axis == X_AXIS) {
		joystick_calibration_values.x_offset = center_sum/100 - 127;
		printf("offset: %d", joystick_calibration_values.x_offset);
	} else {
		joystick_calibration_values.y_offset = center_sum/100 - 127;
		printf("offset: %d", joystick_calibration_values.x_offset);
	}
}

uint8_t HID_read_slider(TOUCH_DEVICE device) {
	if (device != LEFT_SLIDER && device != RIGHT_SLIDER) {
		printf("read_slider called with invalid device");
		return 0;
	}
	return ADC_read_blocking(device);
}

uint8_t HID_read_touch_button(TOUCH_DEVICE device) {
	if (device != LEFT_BUTTON && device != RIGHT_BUTTON) {
		printf("read_touch_button called with invalid device");
		return 0;
	}
	return 
}
