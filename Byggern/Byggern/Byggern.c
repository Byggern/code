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
#include "drivers/SPI_driver.h"
#include "drivers/MCP2515_driver.h"
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
	SPI_init();
	MCP_init();
	
	set_bit(DDRB,2);  // Output on heartbeat led pin
	
    while(1)
    {

		printf("MCP REG: %hu \n", MCP_read(0x0c));
		
		_delay_us(50);
		/* Heart beat */
		toggle_bit(PORTB,2);
		//MENU(menus);
		
		
		
    }
}