//Luke Summers lsummers@g.hmc.edu
//source code for light function

#include "../lib/STM32L432KC.h"
#include "lights.h"

//bit banging the LED data to light one of the LEDs
void lightOneLed(uint32_t color){
  uint32_t mask = 0x1;
  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,color & mask);
    delay_millis(TIM16, 1);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM16,1);
    color = color >> 1;
  }
}

