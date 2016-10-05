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

	/* Write byte*/
	SPDR = byte;
	while(! (SPSR & (1 << SPIF)))
	{
		// Wait
	}
}

uint8_t SPI_receive(void){
	/* Write dummy byte*/
	SPDR = 0;
	while(!(SPSR & (1 << SPIF)))
	{
		// Wait
	}
	return SPDR;
}

void SPI_slave_select(void){
	/* Drive #SS low to select slave*/
	PORTB &= ~(1 << PB4);
}

void SPI_slave_deselect(void){
	/* transmission complete, Drive #SS high to deselect */
	PORTB |= (1 << PB4);
}
