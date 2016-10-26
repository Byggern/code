#include <stdint.h>
#include <stdio.h.>
#include "avr/io.h"
#include "SPI_driver.h"

void SPI_init(void) {
	
	/* MISO input */
	SPI_PORT_DIR &= ~(1 << MISO_PIN);
	/* MOSI, SCK and #SS output */
	SPI_PORT_DIR |= (1 << MOSI_PIN) | (1 << SS_PIN) | (1 << SS_PIN_LOGICAL) | (1 << SCK_PIN);
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
	SPI_PORT &= ~(1 << SS_PIN);
}

void SPI_slave_deselect(void){
	/* transmission complete, Drive #SS high to deselect */
	SPI_PORT |= (1 << SS_PIN);
}
