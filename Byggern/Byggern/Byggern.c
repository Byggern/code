#include <string.h>
#define F_CPU 4912000UL
#include <avr/io.h>
#include <util/delay.h>

#include "macros.h"
#include "drivers/UART_driver.h"
#include "drivers/EXTMEM_driver.h"
	#include "utils/SRAM_utils.h"
#include "drivers/ADC_driver.h"

#define UART0_BAUDRATE 9600
/* default output is to COM1. */

int main(void)
{
	/* System inits */
	UART0_init(F_CPU, UART0_BAUDRATE);
	EXTMEM_init();
	ADC_init();
	printf("Program Start");
	
	set_bit(DDRB,2);  // Output on heartbeat led pin

    while(1)
    {
		//SRAM_test();
		
		int8_t adc_read = ADC_read_blocking(CHANNEL_0);
		printf("ADC_CH0 reads: %d \n", adc_read);
		
		/* Wait and Toggle heartbeat LED */
		_delay_ms(150);
		toggle_bit(PORTB,2);
    }
}