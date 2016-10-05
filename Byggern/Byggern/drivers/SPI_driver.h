
#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_


void SPI_send(char byte);

uint8_t SPI_receive(void);

void SPI_init(void);

void SPI_slave_select(void);

void SPI_slave_deselect(void);

#endif /* SPI_DRIVER_H_ */