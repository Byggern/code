#include <avr/io.h>
#include <stdio.h>
#include "../macros.h"
#include "../utils/SRAM_utils.h"
#include <avr/pgmspace.h>

const char extmem_init_msg[] PROGMEM = "Initializing external memory...\n";
const char extmem_init_done_msg[] PROGMEM = "External memory initialized.\n";
const char extmem_init_fail_msg[] PROGMEM = "[!] External memory initialization failed!\n";

void EXTMEM_init(void) {
	printf_P(extmem_init_msg);
	set_bit(MCUCR, SRE); // Enable external memory
	set_bit(SFIOR, XMM2); // Mask the 4 JTAG bits on port C
	if (SRAM_test() == 0) {
		printf_P(extmem_init_done_msg);
	} else {
		printf_P(extmem_init_fail_msg);
	}
}
