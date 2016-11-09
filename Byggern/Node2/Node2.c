//-C"C:/Program Files (x86)/Arduino/hardware/tools/avr/etc/avrdude.conf" -v -v -patmega2560 -cwiring -P\\.\COM17 -b115200 -D -Uflash:w:"$(ProjectDir)Debug\$(ItemFileName).hex":i

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/interrupt.h>

#include "../Byggern/drivers/CAN_driver.h"
#include "../Byggern/drivers/MCP2515_driver.h"
#include "../Byggern/drivers/SPI_driver.h"
#include "../Byggern/drivers/UART_driver.h"
#include "../Byggern/drivers/HID_driver.h"
#define UART0_BAUDRATE 9600

unsigned char * loop_string = "From 2";
const char recvmsg[] PROGMEM = "Message received: %s\n";

int main(void)
{
	
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("\n Communication up! \n");
	_delay_ms(500);
	
	CAN_init(1,0);

	
	CAN_MESSAGE loop_message = {
		.id = 0,
		.length = strlen((const char*)loop_string),
		.data = loop_string
	};
	
    while(1)
    {
		//_delay_ms(1000);
		
		CAN_send_message(0, 0, &loop_message);
		if ( message_received){
			JOY_VALS * joystick_vals = CAN_receive_buf.data;
			cli();
			
			printf("X val: %d, Y val: %d\n ", joystick_vals->x_axis, joystick_vals->y_axis);
			message_received = false;
			sei();
		}

    }
}

