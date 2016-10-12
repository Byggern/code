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
#define UART0_BAUDRATE 9600
/* default output is to COM1. */


const char recvmsg[] PROGMEM = "Message received: %s\n";
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
	CAN_init();
	
	set_bit(DDRB,2);  // Output on heartbeat led pin
	
	CAN_loopback_init();
	
	const char loop_string[] = "hello,W";
	CAN_MESSAGE loop_message = {
		.id = 1,
		.length = strlen(loop_string)+1,
		.data = loop_string
	};
	
    while(1)
    {
		
		//printf("Status val: %d\n", MCP_read_status());
		_delay_ms(200);
		/* Heart beat */
		toggle_bit(PORTB,2);
		//MENU(menus);
	
		CAN_send_message(0, 0, &loop_message);
		if ( message_received){
			printf_P(recvmsg, CAN_receive_buf.data);
			message_received=false;
		}
		
		
    }
}