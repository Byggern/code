#pragma once
#ifndef HID_DRIVER_H_
#define HID_DRIVER_H_

typedef enum {
	X_AXIS = 0,
	Y_AXIS
} JOY_AXIS;

typedef enum {
	LEFT_SLIDER = 2, // number corresponds to adc channel 
	RIGHT_SLIDER,
	LEFT_BUTTON,
	RIGHT_BUTTON
	} TOUCH_DEVICE;

typedef struct {
	uint8_t x_axis_max;
	uint8_t x_axis_min;
	uint8_t y_axis_max;
	uint8_t y_axis_min;
	int8_t x_offset;
	int8_t y_offset;
	uint8_t x_deadzone;
	uint8_t y_deadzone;
} JOY_CALIBRATE;

uint8_t HID_read_joystick(JOY_AXIS axis);

void HID_calibrate_joystick(void);

uint8_t HID_joystick_zero(JOY_AXIS axis);

long mapToRange(long input, long input_min, long input_max, long output_min, long output_max);

uint8_t HID_read_slider(TOUCH_DEVICE device);

uint8_t HID_read_touch_button(TOUCH_DEVICE device);

#endif /* HID_DRIVER_H_ */
