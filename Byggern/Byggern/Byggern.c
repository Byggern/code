#include <string.h>
#define F_CPU 4912000UL
#include <avr/io.h>
#include <util/delay.h>

#include "macros.h"
#include "drivers/UART_driver.h"
#include "drivers/EXTMEM_driver.h"
#include "utils/SRAM_utils.h"
#include "drivers/ADC_driver.h"
#include "drivers/HID_driver.h"
#include "drivers/OLED_driver.h"
#define UART0_BAUDRATE 9600
/* default output is to COM1. */

int main(void)
{
	/* System inits */
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("\n\n");
	EXTMEM_init();
	ADC_init();
	HID_calibrate_joystick();
	OLED_init();
	printf("\n");


	set_bit(DDRB,2);  // Output on heartbeat led pin
	
	printf("X\tY\tLS\tRS\tLB\tRB\tDIR\tX\tY\n");
	
	const char* dirNames[] = {"L","R","U","D","C"};
	
    while(1)
    {
		/* Print Joystick */
		/* Print Touch devices */
		printf("%3d\t%3d\t", HID_read_joystick_axis(X_AXIS), HID_read_joystick_axis(Y_AXIS));
		printf("%3d\t%3d\t", HID_read_slider(LEFT_SLIDER), HID_read_slider(RIGHT_SLIDER));
		printf("%1d\t%1d\t", HID_read_touch_button(LEFT_BUTTON), HID_read_touch_button(RIGHT_BUTTON));
		printf("%s\t", dirNames[HID_read_joystick_direction()]);
		JOY_VALS joystickState = HID_read_joystick();
		printf("%4d\t%4d\r", joystickState.x_axis, joystickState.y_axis);

		for( int i = 0; i < 128; i++){
			for ( int j = 0 ; j < 8 ; j++){
				if ( i+j> 40){
					OLED_vram[i+j*128] =255;
				}else{
					OLED_vram[i+j*128] =0 ;
				}
			}
		}
		OLED_draw();
		toggle_bit(PORTB,2);
    }
}