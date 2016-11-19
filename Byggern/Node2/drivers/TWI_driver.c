/*
 * TWI_driver.c
 *
 * Created: 19.11.2016 00:17:17
 *  Author: torjehoa
 */ 

#include <avr/io.h>

void TWI_init(void){
	TWBR |= 0xff;
	TWSR |= (1 << TWPS1) | (1 << TWPS0);
}

void TWI_write(uint8_t *data, uint8_t len) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	while(!(TWCR & (1 << TWINT)));
	
	for(uint8_t i = 0; i < len; i++) {
		TWDR = data[i];
		TWCR = (1 << TWINT) | (1 << TWEN);
		while(!(TWCR & (1 <<TWINT)));
	}
	
	TWCR = (1 << TWINT)| (1 << TWSTO) | (1 << TWEN);
}