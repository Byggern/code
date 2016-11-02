#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>

#if defined(__AVR_ATmega162__)
#define F_CPU 4912000UL

#elif defined(__AVR_ATmega2560__)
#define F_CPU 16000000L
#endif

#include <util/delay.h>

#include "SPI_driver.h"
#include "MCP2515_driver.h"

volatile uint8_t MCP_read(uint8_t address){
	
	SPI_slave_select();
	SPI_send(0b00000011); // Read instruction
	SPI_send(address);
	uint8_t read_val = SPI_receive();
	SPI_slave_deselect();
	
	return read_val;
}
uint8_t MCP_read_selected(uint8_t address){
	
	SPI_send(0b00000011); // Read instruction
	SPI_send(address);
	uint8_t read_val = SPI_receive();
	return read_val;
}
uint8_t MCP_read_buffer(uint8_t buffer){
	
	SPI_slave_select();
	SPI_send(0b1001 | (buffer<1)); // Read instruction
	uint8_t read_val = SPI_receive();
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
	SPI_slave_select();
	SPI_send(0b11000000); // Reset instruction
	SPI_slave_deselect();
}

void MCP_init(void){
	SPI_init();
	MCP_reset();
	_delay_us(10);
	
}

uint8_t MCP_status(void){
	SPI_slave_select();
	SPI_send(0b10100000);
	uint8_t temp_status;
	temp_status = SPI_receive();
	SPI_slave_deselect();
	return temp_status;
}

void MCP_select(){
	SPI_slave_select();
}
void MCP_deselect(){
	SPI_slave_deselect();
}