#include <stdint.h>
#include <stdio.h>
#include  "avr/io.h"

#include "HID_driver.h"

JOY_CALIBRATE joystick_calibration_values;

/* Joystick and Button Functions - Place in separate driver? */
uint8_t HID_read_joystick(JOY_AXIS axis) {
	uint8_t adc_val = ADC_read_blocking(axis);
	return adc_val;
}

void HID_calibrate_joystick(void) {
	// ISH VALUES
	joystick_calibration_values.x_axis_min = 0;
	joystick_calibration_values.x_axis_max = 255;
	joystick_calibration_values.y_axis_min = 0;
	joystick_calibration_values.y_axis_max = 255;
	// TODO: Make this interactive, such that it asks the user to press a button while holding the joystick to each side
}

uint8_t HID_read_slider(SLIDER_BUTTON slider) {
	
}