#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <stdint.h>
#include <stdio.h>

void UART0_init(uint32_t clock, uint32_t baud);
int UART0_put( char c, FILE * out);
void UART0_transmit(unsigned char c);

#endif /* UART_DRIVER_H_ */