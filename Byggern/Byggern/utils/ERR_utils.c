#include <stdlib.h>
#include <stdio.h>
#define F_CPU 4912000UL
#include <util/delay.h>

void throwError(error error) {
	error_not_handled = 1;
	printf("\n\n [!] Function %s stopped program execution with error code %s. Extra data: %s", error.function, error.code, error.data);
	while(error_not_handled) {}
}