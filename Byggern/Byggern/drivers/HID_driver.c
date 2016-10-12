#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include  "avr/io.h"
#define F_CPU 4912000UL
#include <util/delay.h>

#include "../macros.h"
#include "HID_driver.h"
#include "ADC_driver.h"

#define JOY_DIR_THRESH 30

JOY_CALIBRATE joystick_calibration_values;

int16_t mapToRange(int16_t input, int16_t input_min, int16_t input_max, int16_t output_min, int16_t output_max) {
	return (input - input_min)*(output_max - output_min)/(input_max - input_min) + output_min;
}

/* Joystick and Button Functions */
uint8_t HID_read_joystick_axis(JOY_AXIS axis) {
	uint8_t adc_val = ADC_read_blocking(axis);
	uint8_t current_joy_val;
	if(axis == X_AXIS) {
		current_joy_val = mapToRange(adc_val - joystick_calibration_values.x_offset, 0-joystick_calibration_values.x_offset, 255-joystick_calibration_values.x_offset, 0, 255);
		if((current_joy_val > 127 + joystick_calibration_values.x_deadzone) | (current_joy_val < 127 - joystick_calibration_values.x_deadzone)) {
			return current_joy_val;
		} else {
			return 127;
		}
	} else {
		current_joy_val = mapToRange(adc_val - joystick_calibration_values.y_offset, 0-joystick_calibration_values.y_offset, 255-joystick_calibration_values.y_offset, 0, 255);
		if((current_joy_val > 127 + joystick_calibration_values.y_deadzone) | (current_joy_val < 127 - joystick_calibration_values.y_deadzone)) {
			return current_joy_val;
		} else {
			return 127;
		}
	}
}

void HID_calibrate_joystick(void) {
	printf("Calibrating joystick...\n");
	
	joystick_calibration_values.x_axis_min = 0;
	joystick_calibration_values.x_axis_max = 255;
	joystick_calibration_values.x_offset = 0;
	joystick_calibration_values.x_deadzone = 5;
	
	joystick_calibration_values.y_axis_min = 0;
	joystick_calibration_values.y_axis_max = 255;
	joystick_calibration_values.y_offset = 0;
	joystick_calibration_values.y_deadzone = 5;
	
	uint16_t x_offset_deadzone_sum = HID_joystick_zero(X_AXIS);
	uint16_t y_offset_deadzone_sum = HID_joystick_zero(Y_AXIS);
	
	if (x_offset_deadzone_sum > 15) {
		printf("X axis calibration failed! (Offset+deadzone out of bounds)\n");
	} else {
		/*printf("X axis calibrated.\n");*/
	}
	
	if (y_offset_deadzone_sum > 15) {
		printf("Y axis calibration failed! (Offset+deadzone out of bounds)\n");
	} else {
		/*printf("Y axis calibrated.\n");*/
	}
	
	if (x_offset_deadzone_sum > 15 || y_offset_deadzone_sum > 15) {
		printf("[!] Joystick calibration failed! Retrying.\n");
		HID_calibrate_joystick();
	} else {
		printf("Joystick calibrated.\n");
	}
}

uint8_t HID_joystick_zero(JOY_AXIS axis) {
	uint16_t center_sum = 0;
	uint8_t axis_max = 127;
	uint8_t axis_min = 127;
	uint8_t current_adc_val;
	int8_t offset;
	uint8_t deadzone;
	
	int i;
	for (i = 0; i < 50; i++) {
		current_adc_val = ADC_read_blocking(axis);
		center_sum += current_adc_val;
		if (current_adc_val > axis_max) {
			axis_max = current_adc_val;
		}
		if (current_adc_val < axis_min) {
			axis_min = current_adc_val;
		}
		_delay_ms(10);
	}
	
	offset = center_sum/50 - 127;
	
	if (abs(axis_max-127) > abs(axis_min-127)) {
		deadzone = abs(axis_max-127);
	} else {
		deadzone = abs(axis_min-127);
	}
	
	if(axis == X_AXIS) {
		joystick_calibration_values.x_offset = offset;
		joystick_calibration_values.x_deadzone = deadzone;
		printf("x offset: %d\n", joystick_calibration_values.x_offset);
		printf("x deadzone: %d\n", joystick_calibration_values.x_deadzone);
	} else {
		joystick_calibration_values.y_offset = offset;
		joystick_calibration_values.y_deadzone = deadzone;
		printf("y offset: %d\n", joystick_calibration_values.y_offset);
		printf("y deadzone: %d\n", joystick_calibration_values.y_deadzone);
	}
	return abs(deadzone)+abs(offset);
}

JOY_VALS HID_read_joystick() {
	JOY_VALS joystick_values;
	joystick_values.x_axis = mapToRange(HID_read_joystick_axis(X_AXIS), 0, 255, -50, 51)*2;
	joystick_values.y_axis = mapToRange(HID_read_joystick_axis(Y_AXIS), 0, 255, -50, 51)*2;
	return joystick_values;
}

JOY_DIR prev_dir = CENTER;

JOY_DIR HID_read_joystick_direction() {
	JOY_DIR current_dir = CENTER;
	int16_t x = HID_read_joystick_axis(X_AXIS)-127;
	int16_t y = HID_read_joystick_axis(Y_AXIS)-127;
	if ((abs(x) < JOY_DIR_THRESH) && (abs(y) < JOY_DIR_THRESH)) {
		current_dir = CENTER;
	} else if ((abs(x) > JOY_DIR_THRESH+10) || (abs(y) > JOY_DIR_THRESH+10)) {
		if (abs(y) > abs(x)) {
			if (y > 0) {
				current_dir = UP;
			} else {
				current_dir = DOWN;
			}
		} else {
			if (x > 0) {
				current_dir = RIGHT;
			} else {
				current_dir = LEFT;
			}
		}
	} else {
		return prev_dir;
	}
	prev_dir = current_dir;
	return current_dir;
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
    switch(device) {
        case LEFT_BUTTON:
            clear_bit(DDRD, PD2);
            if (PIND & (1 << PD2)) {
				return 1;
			} else {
				return 0;
			}
        case RIGHT_BUTTON:
            clear_bit(DDRD, PD3);
            if (PIND & (1 << PD3)){
	            return 1;
	        } else {
	            return 0;
            }
    }
	return 0;
}

uint8_t HID_read_joy_button(void) {
	clear_bit(DDRE, PE0);
	if (PINE & (1 << PE0)) {
		return 1;
	} else {
		return 0;
	}
}
