/*
 * MOT_driver.h
 *
 * Created: 18.11.2016 22:00:59
 *  Author: torjehoa
 */ 


#ifndef MOT_DRIVER_H_
#define MOT_DRIVER_H_

#include <avr/io.h>


#define MJ1 PORTH
#define MJ1_DIR DDRH

#define MJ2 PINK
#define MJ2_DIR DDRK

#define MOT_DIR PH1
#define ENC_SEL PH3
#define MOT_EN PH4
#define ENC_OE_INV PH5
#define ENC_RST_INV PH6

typedef enum {
	MOTOR_RIGHT = 0,
	MOTOR_LEFT = 1
	} MOTOR_DIR;

void MOT_init(void);

int16_t MOT_read_encoder(void);

void MOT_set_direction(MOTOR_DIR dir);

void MOT_set_speed(uint8_t speed);

void MOT_enable(void);

void MOT_disable(void);

#endif /* MOT_DRIVER_H_ */