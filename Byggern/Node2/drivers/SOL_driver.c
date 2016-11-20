/*
 * SOL_driver.c
 *
 * Created: 18.11.2016 20:05:55
 *  Author: torjehoa
 */ 
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

void SOL_init(void){
	// PH3 as output for Solenoid output
	DDRF  |= (1 << DDF1);
	PORTF |= (1 << PF1);
	
	// Waveform Generation mode: Fast PWM
	TCCR5A  = ((0 << WGM51) | (0 << WGM50)  | (0 << COM5C1)| (0 << COM5C0));
	TCCR5B  = ((0 << WGM53) | (0 << WGM52) | (0b000 << CS50) );
	TCCR5C = 0;
	
	// Set output compare value
	OCR5A = 60000;//60*1000 with 8 prescaler is 30 ms
	
	// Enable output compar A interrupt
	TIMSK5 |= (1 << OCIE5A);
	sei();
}

void SOL_fire(void){
	// Reset timer value
	TCNT5 = 0;
	// Clear solenoid pin
	PORTF &= ~(1 << PF1);	
	// Start timer
	TCCR5B  |= (0b010 << CS50) ;

	
}

ISR(TIMER5_COMPA_vect){
	// Turn timer off
	TCCR5B  &= ~(0b111 << CS50);
	// Clear interrupt flag
	TIFR5 &= ~(1 << OCF5A);
	//Set solenoid pin
	PORTF |= (1 << PF1);
}
