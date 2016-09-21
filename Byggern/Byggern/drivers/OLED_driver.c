#include <stdint.h>
#include <stdio.h>

#include "../macros.h"
#include "avr/io.h"
#include "OLED_driver.h"

volatile uint8_t * OLED_command_address = 0x1000;
volatile uint8_t * OLED_data_address = 0x1200;


