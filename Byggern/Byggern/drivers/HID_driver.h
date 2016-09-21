#pragma once
#ifndef HID_DRIVER_H_
#define HID_DRIVER_H_

typedef enum {
	X_AXIS = 0,
	Y_AXIS
} JOY_AXIS;

typedef enum {
	LEFT_SLIDER = 3,
	RIGHT_SLIDER,
	LEFT_BUTTON,
	RIGHT_BUTTON
	} TOUCH_DEVICE;

typedef struct {
	uint8_t x_axis_max;
	uint8_t x_axis_min;
	uint8_t y_axis_max;
	uint8_t y_axis_min;
} JOY_CALIBRATE;

uint8_t HID_read_joystick(JOY_AXIS axis);

void HID_calibrate_joystick(void);

#endif /* HID_DRIVER_H_ */