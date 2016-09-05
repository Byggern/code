/*
 * InitialRS232.c
 *
 * Created: 31.08.2016 11:53:37
 *  Author: magho
 */ 


#include <avr/io.h>
#include <string.h>
#include "UART_driver.h"
#define F_CPU 4912000UL
#define UART0_BAUDRATE 1200

#include <util/delay.h>

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define toggle_bit(reg, bit) (reg ^= (1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

int main(void)
{
	UART0_init(F_CPU, UART0_BAUDRATE);
	set_bit(DDRA,0);
	set_bit(DDRA,1);
	set_bit(PORTA,0);
	clear_bit(PORTA,1);	
	unsigned char msg[] = "heisann\n";
    while(1)
    {
		printf("stuff\n");
		_delay_ms(500);
		toggle_bit(PORTA,0);
		toggle_bit(PORTA,1);
    }
}