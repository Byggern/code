/*
 * InitialRS232.c
 *
 * Created: 31.08.2016 11:53:37
 *  Author: magho
 */ 


#include <avr/io.h>

#include <util/delay.h>

#define F_CPU 4.9152E6

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define toggle_bit(reg, bit) (reg ^= (1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

int main(void)
{
	set_bit(DDRA,0);
	set_bit(DDRA,1);
	set_bit(PORTA,0);
	clear_bit(PORTA,1);	
    while(1)
    {
		_delay_ms(1000);
		toggle_bit(PORTA,0);
		toggle_bit(PORTA,1);
    }
}