/*
 * UART_driver.c
 *
 * Created: 31.08.2016 13:38:48
 *  Author: magho
 */ 
#include <avr/io.h>
#include "UART_driver.h"

void UART0_init(unsigned int clock, int baud){
	int ubrr = (clock /(16 * baud)) - 1;
	ubrr= 15;
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) (ubrr);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Set RX-enable and TX-enable bits
	UCSR0C = (0b11 << UCSZ00) | (0b00 << UPM00) | (1 << USBS0); // ( character size ) | ( parity ) | ( stop bits )
}

void UART0_transmit(unsigned char c) {
	while (  UCSR0B & (1 << UDRE0)) {
	}
	UDR0 = c;
}