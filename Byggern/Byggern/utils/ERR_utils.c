#include <stdlib.h>
#include <stdio.h>
#define F_CPU 4912000UL
#include <util/delay.h>

void throwError(const char* function, const char* error) {
	error_not_handled = 1;
	set_bit(DDRB,2);
	while(error_not_handled) {
		printf("\n\n [!] [!] Error in %s - program execution halted: %s", function, error);
		toggle_bit(PORTB,2);
		_delay_ms(500);
		toggle_bit(PORTB,2);
		_delay_ms(250);
		toggle_bit(PORTB,2);
		_delay_ms(250);
	}
}