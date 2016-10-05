#include <string.h>
#define F_CPU 4912000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "macros.h"
#include "drivers/UART_driver.h"
#include "drivers/EXTMEM_driver.h"
#include "utils/SRAM_utils.h"
#include "drivers/ADC_driver.h"
#include "drivers/HID_driver.h"
#include "drivers/OLED_driver.h"
#include "utils/MENU_utils.h"
#include "utils/DEBUG_utils.h"
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
	MENU_init();
	
	set_bit(DDRB,2);  // Output on heartbeat led pin
	
    while(1)
    {

		/*OLED_clear_screen();
		OLED_write_pixel(20,0,1);
		OLED_write_line(5,8,120,16);
		OLED_write_line(0,0,115,8);
		OLED_draw();*/
		/* Heart beat */
		toggle_bit(PORTB,2);
		MENU(menus);
		
    }
}