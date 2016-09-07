#include <avr/io.h>
#include "../macros.h"

void SRAM_init(void) {
	set_bit(MCUCR, SRE);
}
