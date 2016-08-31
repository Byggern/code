/*
 * UART_driver.h
 *
 * Created: 31.08.2016 13:39:26
 *  Author: magho
 */ 

void UART0_init(unsigned int clock, int baud);

void UART0_transmit(unsigned char c);