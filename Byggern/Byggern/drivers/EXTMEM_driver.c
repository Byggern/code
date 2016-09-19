#include <avr/io.h>
#include "../macros.h"

void EXTMEM_init(void) {
	set_bit(MCUCR, SRE); // Enable external memory
	set_bit(SFIOR, XMM2); // Mask the 4 JTAG bits on port C
	

}
