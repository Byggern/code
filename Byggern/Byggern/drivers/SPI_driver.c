#include <stdint.h>
#include <stdio.h.>
#include <avr/pgmspace.h>
#include "avr/io.h"
#include "SPI_driver.h"


#if defined(__AVR_ATmega162__)
	#define F_CPU 4912000UL
#elif defined(__AVR_ATmega2560__)
	#define F_CPU 16000000L
#endif

#include <util/delay.h>

const char spi_receive_timeout_msg[] PROGMEM = "SPI receive timed out";
const char spi_send_timeout_msg[] PROGMEM = "SPI send timed out";

void SPI_init(void) {
	/* MISO input */
	SPI_PORT_DIR &= ~(1 << MISO_PIN);
	/* MOSI, SCK and #SS output */
	SPI_PORT_DIR |= (1 << MOSI_PIN) | (1 << SS_PIN) | (1 << SS_PIN_LOGICAL) | (1 << SCK_PIN);
	/* Enable SPI, master, clock rate fck*/
	SPCR = (1 << SPE) | (1 << MSTR) ;
	SPCR &= ~(0b11 << SPR0);
	
}

void SPI_send(uint8_t byte) {
	/* Write byte */
	SPDR = byte;
	uint16_t send_count = 0;
	while(!(SPSR & (1 << SPIF)))
	{
		// Wait
		send_count++;
		if (send_count > 1000) {
			printf_P(spi_send_timeout_msg);
			return;
		}
	}
}

uint8_t SPI_receive(void) {
	/* Write dummy byte */
	SPDR = 0;
	uint16_t receive_count = 0;
	while(!(SPSR & (1 << SPIF)))
	{
		// Wait
		receive_count++;
		if (receive_count > 1000) {
			printf_P(spi_receive_timeout_msg);
			return 0;
		}
	}
	return SPDR;
}

void SPI_slave_select(void) {
	/* Drive #SS low to select slave */
	SPI_PORT &= ~(1 << SS_PIN);
	_delay_us(20);
}

void SPI_slave_deselect(void) {
	/* transmission complete, Drive #SS high to deselect */
	SPI_PORT |= (1 << SS_PIN);
	_delay_us(20);
}