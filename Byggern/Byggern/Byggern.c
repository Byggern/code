#include <string.h>
#define F_CPU 4912000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

#include "macros.h"
#include "drivers/UART_driver.h"
#include "drivers/EXTMEM_driver.h"
#include "utils/SRAM_utils.h"
#include "drivers/ADC_driver.h"
#include "drivers/HID_driver.h"
#include "drivers/OLED_driver.h"
#include "utils/MENU_utils.h"
#include "utils/DEBUG_utils.h"
#include "drivers/CAN_driver.h"
#include "drivers/MCP2515_driver.h"
#include "utils/GAME_util.h"
#include <avr/interrupt.h>
#define UART0_BAUDRATE 9600

const char sys_init_msg[] PROGMEM =  "\n\n";

void SYS_init(void) {
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf_P(sys_init_msg);
	EXTMEM_init();
	ADC_init();
	HID_init();
	OLED_init();
	MENU_init();
	CAN_init(0, 0);
}

int main(void) {
	SYS_init();
	
	while(1) {	
		MENU(menus);
	}
}
