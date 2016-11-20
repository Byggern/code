#ifndef MCP2515_DRIVER_H_
#define MCP2515_DRIVER_H_

#define CANINTE 0x2b
#define CANINTF 0x2c
#define CANCTRL 0xf
#define CANSTAT 0xe
#define EFLG 0x2d
#define BFPCTRL 0x0c
#define TXRTSCTRL 0x0d

#define TXB0_OFFSET 0x00
#define TXB1_OFFSET 0x10
#define TXB2_OFFSET 0x20
#define TXBnCTRL 0x30
#define TXBnSIDH 0x31
#define TXBnSIDL 0x32
#define TXBnDm   0x36
#define TXBnDLC 0x35

#define RXBnCTRL 0x60
#define RXBnSIDH 0x61
#define RXBnSIDL 0x62
#define RXBnDLC 0x65
#define RXBnDm   0x66
#define RXB0_OFFSET 0x00
#define RXB1_OFFSET 0x10

#include <stdint.h>

volatile uint8_t MCP_read(uint8_t address);

uint8_t MCP_read_selected(uint8_t address);

uint8_t MCP_read_buffer(uint8_t buffer);

void MCP_write(uint8_t address, uint8_t byte);

void MCP_request_to_send(uint8_t buffer);

uint8_t MCP_read_status(void);

void MCP_bit_modify(uint8_t reg, uint8_t bit, uint8_t val);

void MCP_reset(void);

void MCP_init(void);

uint8_t MCP_status(void);

void MCP_select();

void MCP_deselect();
#endif /* MCP2515_DRIVER_H_ */