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
	
		
	OLED_clear_screen();
	OLED_set_cursor(7,120);
	OLED_write_string("Hello, world!");
	
	OLED_draw();
    while(1)
    {
		DEBUG_print_HID_values();
		
		OLED_draw();
		toggle_bit(PORTB,2);
    }
}