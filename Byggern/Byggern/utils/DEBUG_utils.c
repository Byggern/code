#include "../macros.h"
#include "../drivers/UART_driver.h"
#include "../drivers/EXTMEM_driver.h"
#include "SRAM_utils.h"
#include "../drivers/ADC_driver.h"
#include "../drivers/HID_driver.h"
#include "../drivers/OLED_driver.h"

void DEBUG_print_HID_values_header(void) {
	printf("X\tY\tLS\tRS\tLB\tRB\tDIR\tX\tY\n");
}

void DEBUG_print_HID_values(void) {
	const char* dirNames[] = {"L","R","U","D","C"};
	
	printf("%3d\t%3d\t", HID_read_joystick_axis(X_AXIS), HID_read_joystick_axis(Y_AXIS));
	printf("%3d\t%3d\t", HID_read_slider(LEFT_SLIDER), HID_read_slider(RIGHT_SLIDER));
	printf("%1d\t%1d\t", HID_read_touch_button(LEFT_BUTTON), HID_read_touch_button(RIGHT_BUTTON));
	printf("%s\t", dirNames[HID_read_joystick_direction()]);
	JOY_VALS joystickState = HID_read_joystick();
	printf("%4d\t%4d\r", joystickState.x_axis, joystickState.y_axis);
}