/*
 * UART_driver.h
 *
 * Created: 31.08.2016 13:39:26
 *  Author: magho
 */ 

#include <stdint.h>
#include <stdio.h>

void UART0_init(uint32_t clock, uint32_t baud);
int UART0_put( char c, FILE * out);
void UART0_transmit(unsigned char c);