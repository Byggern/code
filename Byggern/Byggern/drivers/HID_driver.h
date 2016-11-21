#ifndef HID_DRIVER_H_
#define HID_DRIVER_H_

typedef enum {
	X_AXIS = 0,
	Y_AXIS
} JOY_AXIS;

typedef enum {
	LEFT_SLIDER = 2, // number corresponds to ADC channel 
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

typedef struct {
	int16_t x_axis;
	int16_t y_axis;
} JOY_VALS;

typedef enum {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	CENTER,
	NOCHANGE
} JOY_DIR;

void HID_init(void);

uint8_t HID_read_joystick_axis(JOY_AXIS axis);

void HID_calibrate_joystick(void);

uint8_t HID_joystick_zero(JOY_AXIS axis);

int16_t mapToRange(int16_t input, int16_t input_min, int16_t input_max, int16_t output_min, int16_t output_max);

JOY_VALS HID_read_joystick();

JOY_DIR HID_read_joystick_direction_change();

JOY_DIR HID_read_joystick_direction();

uint8_t HID_read_joy_button(void);

uint8_t HID_read_slider(TOUCH_DEVICE device);

uint8_t HID_read_touch_button(TOUCH_DEVICE device);

#endif /* HID_DRIVER_H_ */
