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

#define UART0_BAUDRATE 9600
/* default output is to COM1. */

int main(void)
{
	/* System inits */
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("\n\n- - - N o p e - - -\n\nSerial port initialized. Hello, world!\n");
	EXTMEM_init();
	ADC_init();
	HID_calibrate_joystick();
	printf("\nProgram Start\n");

	
	set_bit(DDRB,2);  // Output on heartbeat led pin

    while(1)
    {
		/* Print Joystick */
		printf("X: %3d Y: %3d \n", HID_read_joystick(X_AXIS), HID_read_joystick(Y_AXIS));

		/* Print Touch devices */
		printf("Left slider: %d3 Right slider %d3 \n", HID_read_slider(LEFT_SLIDER), HID_read_slider(RIGHT_SLIDER));
		printf("Left button: %d1 Right button %d1 \n", HID_read_touch_button(LEFT_BUTTON), HID_read_touch_button(RIGHT_BUTTON));

		/* Wait and Toggle heartbeat LED */
		_delay_ms(150);
		toggle_bit(PORTB,2);
    }
}