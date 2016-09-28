#include <stdlib.h>
#include <stdio.h>
#define F_CPU 4912000UL
#include <util/delay.h>

void throwError(const char* error) {
	error_not_handled = 1;
	printf("\n\n [!] [!] Error - program execution halted: %s", error);
	while(error_not_handled) {}
}