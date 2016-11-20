/*
* MOT_drover.c
*
* Created: 18.11.2016 22:00:49
*  Author: torjehoa
*/
#define F_CPU 16000000
#include <util/delay.h>

#include "TWI_driver.h"
#include "MOT_driver.h"

void MOT_init(void){
	// Encoder data port to input
	MJ2_DIR = 0x0;
	// Encoder control pins to output
	MJ1_DIR |= (1 << ENC_SEL) | (1 << ENC_OE_INV) | (1 << ENC_RST_INV);
	//Reset
	MJ1 &= ~(1 << ENC_RST_INV);
	_delay_us(20);
	MJ1 |= (1 << ENC_RST_INV);
	
	TWI_init();
	
	// Motor control pins to output
	MJ1_DIR |= (1 << MOT_DIR) | (1 << MOT_EN);
	// Motor enable
	MJ1 |= (1 << MOT_EN);
	
	MOT_set_direction(MOTOR_LEFT);
	MOT_set_speed(0);
}

int16_t MOT_read_encoder(void) {
	int16_t encoder_data;
	//Enable output
	MJ1 &= ~(1 << ENC_OE_INV);
	// SEL to MSB
	MJ1 &= ~(1 << ENC_SEL);
	// Wait 20us
	_delay_us(20);
	// Read MSB
	encoder_data = (MJ2 << 8);
	// SEL to LSB
	MJ1 |= (1 << ENC_SEL);
	// Wait 20us
	_delay_us(20);
	// Read LSB
	encoder_data |= MJ2;
	// Reset encoder
	//MJ1 &= ~(1 << ENC_RST_INV);
	//_delay_us(20);
	//MJ1 |= (1 << ENC_RST_INV);
	
	return encoder_data;
}

void MOT_set_direction(MOTOR_DIR dir){
	if (dir == MOTOR_RIGHT) {
		MJ1 |= (1 << MOT_DIR);
	} else if (dir == MOTOR_LEFT) {
		MJ1 &= ~(1 << MOT_DIR);
	} else {
		printf("Motor direction invalid\n");
	}
}

void MOT_set_speed(uint8_t speed){
	uint8_t motor_msg[3];
	// DAC slave address
	motor_msg[0] = 0b01011110;
	// DAC command byte
	motor_msg[1] = 0b00000000;
	// DAC speed byte
	motor_msg[2] = speed;
	TWI_write(motor_msg, 3);
}