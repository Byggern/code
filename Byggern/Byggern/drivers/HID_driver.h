#pragma once
#ifndef HID_DRIVER_H_
#define HID_DRIVER_H_

typedef enum {
	X_AXIS = 0,
	Y_AXIS
} JOY_AXIS;

typedef struct {
	uint8_t x_axis_max;
	uint8_t x_axis_min;
	uint8_t y_axis_max;
	uint8_t y_axis_min;
	uint8_t x_offset;
	uint8_t y_offset;
} JOY_CALIBRATE;

uint8_t HID_read_joystick(JOY_AXIS axis);

void HID_calibrate_joystick(void);

#endif /* HID_DRIVER_H_ */
