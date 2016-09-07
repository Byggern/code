#include <avr/io.h>
#include <string.h>

#define F_CPU 4912000UL
#include <util/delay.h>

#include "macros.h"
#include "drivers/UART_driver.h"
#include "drivers/SRAM_driver.h"
#include "utils/SRAM_utils.h"


#define UART0_BAUDRATE 9600
/* default output is to COM1. */


int main(void)
{
	UART0_init(F_CPU, UART0_BAUDRATE);
	SRAM_init();
	printf("Program Start");
	set_bit(DDRB,2);
	set_bit(PORTB,2);	
	unsigned int inc = 1;
	unsigned char * addr = 0;
    while(1)
    {
		SRAM_test();
		/*
		if (  addr == 0x1900 ) {
			addr = (void*)0x1800;
		}else{
			//addr = (void *)( ((int)addr)*2);
			addr++;
		}
		*addr += 1;
		*/
		//inc++;
		//printf("%c",*addr);
		//_delay_us(100);
		//SRAM_test();
		/*
		printf("%d", inc);
		printf(" stuff\n");
		_delay_ms(500);
		*/
		toggle_bit(PORTB,2);
    }
}