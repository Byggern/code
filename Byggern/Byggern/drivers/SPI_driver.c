#include <stdint.h>
#include <stdio.h.>
#include "avr/io.h"

void SPI_init(void) {
	/* MISO input */
	DDRB &= ~(1 << PB6);
	/* MOSI, SCK and #SS output */
	DDRB |= (1 << PB4) | (1 << PB5) | (1 << PB7);
	/* Enable SPI, master, clock rate fck*/
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_send(uint8_t byte) {
	/* Drive #SS low to select slave*/
	PORTB &= ~(1 << PB4);
	
	/* Write byte*/
	SPDR = byte;
	while(! SPSR & (1 << SPIF))
	{
		// Wait
	}
	
	/* transmission complete, Drive #SS high to deselect */
	PORTB |= (1 << PB4);
}

uint8_t SPI_receive(void){
	/* Drive #SS low to select slave*/
	PORTB &= ~(1 << PB4);
		
	/* Write dummy byte*/
	SPDR = 0;
	while(!(SPSR & (1 << SPIF)))
	{
		// Wait
	}
		
	/* transmission complete, Drive #SS high to deselect, and return */
	PORTB |= (1 << PB4);
	
	return SPDR;
}
