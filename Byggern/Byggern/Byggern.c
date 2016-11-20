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

unsigned char* loop_string = "From 1";
const char recvmsg[] PROGMEM = "Message received: %s\n";

void SYS_init(void) {
	/* System inits */
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("\n\n");
	EXTMEM_init();
	ADC_init();
	HID_calibrate_joystick();
	OLED_init();
	MENU_init();
	CAN_init(0, 0);
	
	set_bit(DDRB,2);  // Output on heartbeat led pin
}

int main(void)
{
	SYS_init();


	
	CAN_MESSAGE loop_message = {
		.id = 1,
		.length = 6,
		.data = loop_string
	};
	
	while(1)
	{
		/* Heart beat */
		toggle_bit(PORTB,2);
		
		//MENU(menus);
		
		GAME_send_controls();
		
		
		if ( message_received){
			switch(CAN_receive_buf.data[0]) {
				case GAME_MISS:
					printf("GAME MISS\n");
					message_received = false;
				break;
			}
			
			/*cli();
			printf("--");
			printf_P(recvmsg);
			printf("\nLen: %d\n", CAN_receive_buf.length);
			printf("%s\n", CAN_receive_buf.data);
			message_received = false;
			sei();*/
		}
		
		//CAN_send_message(1, &loop_message);
	}
}
