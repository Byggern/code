#include <stdint.h>
#include <stdio.h>

#define F_CPU 4912000UL
#include <avr/io.h>
#include <util/delay.h>

#include "SPI_driver.h"

uint8_t MCP_read(uint8_t address){
	SPI_slave_select();
	SPI_send(0b00000011); // Read instruction
	SPI_send(address);
	
	uint8_t read_val = SPI_receive();
	SPI_slave_deselect();
	
	return read_val;
}

void MCP_write(uint8_t byte){
	
}

void MCP_request_to_send(void){
	
}

void MCP_read_status(void){
	
}

void MCP_bit_modify(uint8_t bit){
	
}

void MCP_reset(void){
	SPI_send(0b11000000); // Reset instruction
}

void MCP_init(void){
	MCP_reset();
}