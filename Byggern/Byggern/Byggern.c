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
#define UART0_BAUDRATE 9600
/* default output is to COM1. */


const char progmemtest[] PROGMEM = "jeg er fra progmem!\0";

void hellofunction(){
	
}

const char mainmenu[] PROGMEM = "Main Menu";
const char utilsmenu[] PROGMEM = "Utils";
const char hellomenu[] PROGMEM = "Hello World!";
const char calibratemenu[] PROGMEM = "Calibrate Joystick";

const Menu menus[4] = {
	{.header = mainmenu,		.function = NULL,					.length=1},
	{.header = utilsmenu,		.function = NULL,					.length=1},
	{.header = hellomenu,		.function = hellofunction,			.length=0},
	{.header = calibratemenu ,	.function = HID_calibrate_joystick, .length=0}		
};
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
	//uint16_t i = 0;
	DEBUG_print_HID_values_header();
	
    while(1)
    {
		
		DEBUG_print_HID_values();
		//OLED_write_char( pgm_read_byte(&progmemtest[0]));
		OLED_write_string_P( progmemtest);
		OLED_draw();
		toggle_bit(PORTB,2);
		//i++;
    }
}