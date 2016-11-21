#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "../Byggern/drivers/UART_driver.h"
#include "../Byggern/drivers/CAN_driver.h"
#include "drivers/PWM_driver.h"
#include "drivers/ADC_driver.h"
#include "drivers/SOL_driver.h"
#include "utils/GAME2_util.h"
#define UART0_BAUDRATE 9600

int main(void) {
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("\n Communication up! \n");
	_delay_ms(500);
	
	CAN_init(1,0);
	PWM1_start();
	ADC_init();
	SOL_init();
	GAME2_init();
	
	while(1) {
		GAME2_check_messages();
		GAME2_check_sensors();
		GAME2_update_regulator();
	}
}

