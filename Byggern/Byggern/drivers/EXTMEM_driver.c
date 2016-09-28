#include "../utils/ERR-utils.h"
char module[] = "EXTMEM_driver";

#include <avr/io.h>
#include <stdio.h>
#include "../macros.h"
#include "../utils/SRAM_utils.h"

void EXTMEM_init(void) {
	char function[] = "EXTMEM_init";
	
	printf("Initializing external memory...\n");
	set_bit(MCUCR, SRE); // Enable external memory
	set_bit(SFIOR, XMM2); // Mask the 4 JTAG bits on port C
	if (SRAM_test() == 0) {
		printf("External memory initialized.\n");
	} else {
		throwError(module, function, "External memory initialization failed!");
	}
}
