#include <avr/io.h>
#include "../macros.h"

void SRAM_init(void) {
	set_bit(MCUCR, SRE); // Enable external memory
	set_bit(SFIOR, XMM2); // Mask the 4 JTAG bits on port C
	
	/* The ALE signal was very sensitive to noise, thus we try to wait two cycles each read/write strobe */
	//set_bit(EMCUCR, SRW11);
	// Two wait cycles RD/WR, one wait cycle before new address
}
