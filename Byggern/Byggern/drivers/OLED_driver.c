#include <stdint.h>
#include <stdio.h>

#include <avr/pgmspace.h>
#include "../macros.h"
#include "avr/io.h"
#include "OLED_driver.h"
#include "../utils/SRAM_utils.h"


volatile uint8_t * OLED_command_address = (uint8_t*)0x1000;
volatile uint8_t * OLED_data_address = (uint8_t*)0x1200;

void OLED_command( uint8_t command){
  *OLED_command_address=command;
}

const unsigned char re_init_error[] PROGMEM = "you already have initialized OLED!\n";

uint8_t * OLED_vram=NULL;
void OLED_init() {
  OLED_reset();
  
  if ( !OLED_vram){
    OLED_vram = SRAM_allocate(1024);
  }else{
    //printf("%s", pgm_read( re_init_error));
  }
}

void OLED_reset(){

    OLED_command(0xae);//Turn off display
    OLED_command(0xa1);//Map Segment 0-128 in instead of 128-256
    OLED_command(0xda);//common pads hardware: alternative
    OLED_command(0x12);
    OLED_command(0xc8);//common output scan direction:com63~com0
    OLED_command(0xa8); //multiplex ration mode:63
    OLED_command(0x3f);
    OLED_command(0xd5);//display divide ratio/osc. freq. mode
    OLED_command(0x80);
    OLED_command(0x81);//contrast control
    OLED_command(0x50);
    OLED_command(0xd9);//set pre-charge period
    OLED_command(0x21);
    OLED_command(0x20);//Set Memory Addressing Mode
    OLED_command(0x00);//MEMadressing horizontal
    OLED_command(0xdb);//VCOM deselect level mode
    OLED_command(0x30);
    OLED_command(0xad);//master configuration
    OLED_command(0x00);
    OLED_command(0xa4); //out follows RAM content
    OLED_command(0xa6); //set normal display
    OLED_command(0xaf);// display on
}

void OLED_draw(){
  for ( int i = 0 ; i < 512;i++){
    OLED_data_address[i] = OLED_vram[i]%256;
  }
  for ( int i = 0 ; i < 512;i++){
    OLED_data_address[i] = OLED_vram[i+512]%256;
  }
}