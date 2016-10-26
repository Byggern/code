
#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_
#include <avr/io.h>

#if defined(__AVR_ATmega162__)

#define SPI_PORT_DIR DDRB
#define SPI_PORT PORTB
#define SS_PIN PB4
#define SS_PIN_LOGICAL PB4
#define MOSI_PIN PB5
#define MISO_PIN PB6
#define SCK_PIN PB7

#elif defined(__AVR_ATmega2560__)

#define SPI_PORT_DIR DDRB
#define SPI_PORT PORTB
#define SS_PIN PB7 
#define SS_PIN_LOGICAL PB0
#define MOSI_PIN PB2
#define MISO_PIN PB3
#define SCK_PIN PB1

#endif


void SPI_send(uint8_t byte);

uint8_t SPI_receive(void);


void SPI_init(void);

void SPI_slave_select(void);

void SPI_slave_deselect(void);

#endif /* SPI_DRIVER_H_ */