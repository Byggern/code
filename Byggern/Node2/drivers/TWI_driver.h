/*
 * TWI_driver.h
 *
 * Created: 19.11.2016 00:17:26
 *  Author: torjehoa
 */ 


#ifndef TWI_DRIVER_H_
#define TWI_DRIVER_H_

void TWI_init(void);

void TWI_write(uint8_t *data, uint8_t len);



#endif /* TWI_DRIVER_H_ */