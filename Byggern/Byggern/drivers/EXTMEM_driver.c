#include <avr/io.h>
#include <stdio.h>
#include "../macros.h"
#include "../utils/SRAM_utils.h"

void EXTMEM_init(void) {
	printf("Initializing external memory...\n");
	set_bit(MCUCR, SRE); // Enable external memory
	set_bit(SFIOR, XMM2); // Mask the 4 JTAG bits on port C
	if (SRAM_test() == 0) {
		printf("External memory initialized.\n");
	} else {
		printf("[!] External memory initialization failed!\n");
	}
}
