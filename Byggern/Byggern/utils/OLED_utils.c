
#define F_CPU 4912000UL
#include <util/delay.h>
#include "../drivers/OLED_driver.h"

void OLED_lr_bar(uint8_t x){
  OLED_write_line_state(x,0,x+1,64,1);
  OLED_write_line_state(x-1,0,x,64,0);
  OLED_draw();

}

void OLED_lr_bar_clear(){
  for( uint8_t x= 1; x < 127;x++){
    OLED_lr_bar(x);
  }
}
