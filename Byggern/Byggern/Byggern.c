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
#include <avr/interrupt.h>
#define UART0_BAUDRATE 9600
/* default output is to COM1. */

unsigned char* loop_string = "From 1";
const char recvmsg[] PROGMEM = "Message received: %s\n";

int main(void) {
	/* System inits */
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("\n\n");
	EXTMEM_init();
	ADC_init();
	HID_calibrate_joystick();
	OLED_init();
	MENU_init();
	CAN_init(0, 0);
	
	set_bit(DDRB, 2);  // Output on heartbeat led pin

	CAN_MESSAGE loop_message = {
		.id = 1,
		.length = strlen((const char*)loop_string),
		.data = loop_string
	};
	
    while(1) {
		/* Heart beat */
		toggle_bit(PORTB, 2);
		//MENU(menus);
		
		JOY_VALS joystick_vals = HID_read_joystick();
		loop_message.length = sizeof(JOY_VALS);
		loop_message.data = &joystick_vals;
		
		CAN_send_message(1, &loop_message);
		
		//_delay_ms(1000);
		
		if (message_received){
			cli();
			printf("--");
			printf_P(recvmsg);
			printf("%s\n",CAN_receive_buf.data);
			message_received = false;
			sei();
		}
    }
}
