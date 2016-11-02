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
#define UART0_BAUDRATE 9600

unsigned char * loop_string = "From 20";
const char recvmsg[] PROGMEM = "Message received: %s\n";

int main(void)
{
	
	// Heartbeat direction
	//DDRB |= (1 << );
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("\n Communication up! \n");
	_delay_ms(500);
	
	CAN_init(1,1);
	printf_P(recvmsg, CAN_receive_buf.data);
	
	printf("Status val: %x\n", MCP_read(CANSTAT));
	printf("Status val: %x\n", MCP_read(CANSTAT));
	
	
	CAN_MESSAGE loop_message = {
		.id = 1,
		.length = strlen((const char*)loop_string) + 1,
		.data = loop_string
	};
	
    while(1)
    {
	    printf("%c",11);
		for (int i = 0 ; i< 3; i++)
		{
		//printf("Woho\n");
		
		CAN_send_message(1, 0, &loop_message);
		if ( message_received){
			cli();
			printf("--");
		
			printf_P(recvmsg);
			printf(CAN_receive_buf.data);
			message_received = false;
			sei();
		}
		/*printf("Status val: %x\n", MCP_read(CANSTAT));
		printf("Control val: %x\n", MCP_read(CANCTRL));
		printf("CanintE val: %x\n", MCP_read(CANINTE));
		printf("CanintF val: %x\n", MCP_read(CANINTF));
		printf("ERRFLG val: %x\n", MCP_read(EFLG));
		
		printf("RCB0CTRL: %x\n", MCP_read(RXBnCTRL + RXB0_OFFSET));
		printf("RCB1CTRL: %x\n", MCP_read(RXBnCTRL + RXB1_OFFSET));*/
		
		// Heartbeat
		//PORTB ^= (1 << );
		_delay_ms(1000);
        }
    }
}

