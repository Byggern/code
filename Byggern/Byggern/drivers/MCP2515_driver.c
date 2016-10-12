#include <stdint.h>
#include <stdio.h>

#define F_CPU 4912000UL
#include <avr/io.h>
#include <util/delay.h>

#define CANINTE 0x2b

#include "SPI_driver.h"

uint8_t MCP_read(uint8_t address){
	
	SPI_slave_select();
	SPI_send(0b00000011); // Read instruction
	SPI_send(address);
	uint8_t read_val = SPI_receive();
	SPI_slave_deselect();
	
	return read_val;
}

void MCP_write(uint8_t address, uint8_t byte) {
	SPI_slave_select();
	SPI_send(0b00000010); // Write instruction
	SPI_send(address);
	SPI_send(byte);
	SPI_slave_deselect();
}

void MCP_request_to_send(uint8_t buffer)
{
	SPI_slave_select();
	SPI_send(0b10000000 | (1 << buffer));
	SPI_slave_deselect();
}

uint8_t MCP_read_status(void)
{
	SPI_slave_select();
	SPI_send(0b10100000);
	uint8_t status_byte = SPI_receive();
	SPI_slave_deselect();
	return status_byte;
}

void MCP_bit_modify(uint8_t reg, uint8_t bit, uint8_t val) {
	uint8_t prev_byte = MCP_read(reg);
	prev_byte = (prev_byte & (~(1 << bit))) | (val << bit);
	MCP_write(reg, prev_byte);
}

void MCP_reset(void){
	SPI_send(0b11000000); // Reset instruction
}

void MCP_init(void){
	SPI_init();
	MCP_reset();
	
	//Set Can interrupt bit for receive
	MCP_bit_modify( CANINTE, 0, 1);
	MCP_bit_modify( CANINTE, 1, 1);
	
	//Set up interrupt handler for 162	
}