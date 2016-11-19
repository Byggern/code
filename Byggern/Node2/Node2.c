//-C"C:/Program Files (x86)/Arduino/hardware/tools/avr/etc/avrdude.conf" -v -v -patmega2560 -cwiring -P\\.\COM17 -b115200 -D -Uflash:w:"$(ProjectDir)Debug\$(ItemFileName).hex":i

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/interrupt.h>

#include "../Byggern/drivers/CAN_driver.h"
#include "../Byggern/drivers/MCP2515_driver.h"
#include "../Byggern/drivers/SPI_driver.h"
#include "../Byggern/drivers/UART_driver.h"
#include "../Byggern/drivers/HID_driver.h"
#include "drivers/PWM_driver.h"
#include "drivers/ADC_driver.h"
#include "drivers/SOL_driver.h"
#include "drivers/MOT_driver.h"
#define UART0_BAUDRATE 9600

unsigned char * loop_string = "From 2";
const char recvmsg[] PROGMEM = "Message received: %s\n";

int main(void)
{
	
	UART0_init(F_CPU, UART0_BAUDRATE);
	printf("\n Communication up! \n");
	_delay_ms(500);
	
	CAN_init(1,0);
	PWM1_start();
	ADC_init();
	SOL_init();
	MOT_init();
	
	sei();
	CAN_MESSAGE loop_message = {
		.id = 0,
		.length = strlen((const char*)loop_string),
		.data = loop_string
	};
	uint16_t adc_last = 0;
	uint16_t adc_curr = 0;
	uint16_t adc_min = 1024;
	uint16_t adc_max = 0x0;
	
	int16_t slider=0;
	while(1)
	{
		
		for (uint8_t i = 0; i < 20; i++)
		{
			
			
			//CAN_send_message(0, 0, &loop_message);
			if ( message_received){
				JOY_VALS * joystick_vals = CAN_receive_buf.data;
				//uint16_t button = CAN_receive_buf.data[4];
				//uint16_t slider = CAN_receive_buf.data[5];
				slider = CAN_receive_buf.data[5];
				
				slider = slider - 128;
				printf("Slider: %d \n", slider);
				cli();
				message_received = false;
				PWM_set_duty((joystick_vals->x_axis/2) + 50);
				sei();
				
				//printf("X val: %d, Y val: %d\n", joystick_vals->x_axis, joystick_vals->y_axis);
				
			}
			
			adc_curr = ADC_state();
			
			if (adc_curr != adc_last) {
				//printf("adc read: %x  min: %x \n",adc_curr,adc_min);
				i++;
			} else {
				//pass
			}
			//adc_min = adc_min < adc_curr ? adc_min : adc_curr;
			//adc_max = adc_max > adc_curr ? adc_max : adc_curr;
			
			//adc_last = adc_curr;
			
			//printf("Encoder: %d \n", MOT_read_encoder());
			uint8_t motor_speed = abs(slider/2);
			
			printf("Motor speed: %d\n", motor_speed);
			if ( slider > 0 ) {
				MOT_set_direction(MOTOR_RIGHT);
				MOT_set_speed(motor_speed + 50);
			} else {
				MOT_set_direction(MOTOR_LEFT);
				MOT_set_speed(motor_speed + 50);
			}
			
		}
		
		printf("%c\n", 11);
	}
}

