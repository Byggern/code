//-C"C:/Program Files (x86)/Arduino/hardware/tools/avr/etc/avrdude.conf" -v -v -patmega2560 -cwiring -P\\.\COM17 -b115200 -D -Uflash:w:"$(ProjectDir)Debug\$(ItemFileName).hex":i

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "../Byggern/drivers/CAN_driver.h"
#include "../Byggern/drivers/MCP2515_driver.h"
#include "../Byggern/drivers/SPI_driver.h"
#include "../Byggern/drivers/UART_driver.h"
#define UART0_BAUDRATE 9600

const char loop_string[] = "hello,W";


const char recvmsg[] PROGMEM = "Message received: %s\n";

int main(void)
{
	// Heartbeat direction
	//DDRB |= (1 << PB7);
	SPI_init();
	CAN_init(1);
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("communication up! \n");	
	CAN_loopback_init();
	CAN_MESSAGE loop_message = {
		.id = 1,
		.length = strlen(loop_string)+1,
		.data = loop_string
	};
    while(1)
    {
		//printf("Woho\n");
		CAN_send_message(0, 0, &loop_message);
		//SPI_send(0b00001111 );
		if ( message_received){
			printf("--");
			printf_P(recvmsg, CAN_receive_buf.data);
			message_received=false;
		}
		printf("CANCTRL: %x\n", MCP_read(CANCTRL));
		
		
		// Heartbeat
		//PORTB ^= (1 << PB7);
		_delay_us(10);
	
        
    }
}

