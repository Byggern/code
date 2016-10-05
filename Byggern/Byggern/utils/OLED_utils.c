
#define F_CPU 4912000UL
#include <util/delay.h>
#include "../drivers/OLED_driver.h"

void OLED_lr_bar(uint8_t x){
  OLED_write_line_state(x,0,x,64,1);
  OLED_write_line_state(x-1,0,64,0)

}

void OLED_lr_bar_clear(){
  for( int x= 1; x < 128;x++){
    OLED_lr_bar(x);
  }
}
