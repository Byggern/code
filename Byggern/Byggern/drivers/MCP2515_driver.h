

#ifndef MCP2515_DRIVER_H_
#define MCP2515_DRIVER_H_

#include <stdint.h>

uint8_t MCP_read(uint8_t address);

void MCP_write(uint8_t byte);

void MCP_request_to_send(void);

void MCP_read_status(void);

void MCP_bit_modify(uint8_t bit);

void MCP_reset(void);

void MCP_init(void);

#endif /* MCP2515_DRIVER_H_ */