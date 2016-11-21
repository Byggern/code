#include <avr/io.h>

void TWI_init(void) {
	// set TWI SCL frequency to 400 kHz
	TWBR |= 0xFF;
	TWSR |= (1 << TWPS1) | (1 << TWPS0);
}

void TWI_write(uint8_t* data, uint8_t len) {
	// send start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	// wait for previous operation to finish
	while(!(TWCR & (1 << TWINT)));
	
	// put data byte(s) on TWI line
	for(uint8_t i = 0; i < len; i++) {
		TWDR = data[i];
		TWCR = (1 << TWINT) | (1 << TWEN);
		while(!(TWCR & (1 <<TWINT)));
	}
	
	// send stop condition
	TWCR = (1 << TWINT)| (1 << TWSTO) | (1 << TWEN);
}