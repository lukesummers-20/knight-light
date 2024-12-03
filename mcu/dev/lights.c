#include "../lib/STM32L432KC.h"
#include "lights.h"

void lightOneLed(uint32_t color){
  uint32_t mask = 0x1;
  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,color & mask);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15,2);
    color = color >> 1;
  }
}

//void ledStartup() {
//  uint32_t data = 0x00000000;
//  uint32_t msk = 0x1;

//  for(int i = 0; i < 32; i++) {
//    digitalWrite(LED_CLK,0);
//    digitalWrite(LED_SDI,data&msk);
//    delay_millis(TIM15, 2);
//    digitalWrite(LED_CLK,1);
//    delay_millis(TIM15,2);

//    data = data >> 1;


//  }

  
//}

//void ledRed() {
//  uint32_t data = 0xFF0000FF;
//  uint32_t msk = 0x1;

//  for(int i = 0; i < 32; i++) {
//    digitalWrite(LED_CLK,0);
//    digitalWrite(LED_SDI,data&msk);
//    delay_millis(TIM15, 2);
//    digitalWrite(LED_CLK,1);
//    delay_millis(TIM15,2);

//    data = data >> 1;


//  }
//}

//void ledGreen() {
//  uint32_t data = 0x00FF00FF;
//  uint32_t msk = 0x1;

//  for(int i = 0; i < 32; i++) {
//    digitalWrite(LED_CLK,0);
//    digitalWrite(LED_SDI,data&msk);
//    delay_millis(TIM15, 2);
//    digitalWrite(LED_CLK,1);
//    delay_millis(TIM15,2);

//    data = data >> 1;


//  }
//}

//void ledBlue() {
//  uint32_t data = 0x0000FFFF;
//  uint32_t msk = 0x1;

//  for(int i = 0; i < 32; i++) {
//    digitalWrite(LED_CLK,0);
//    digitalWrite(LED_SDI,data&msk);
//    delay_millis(TIM15, 2);
//    digitalWrite(LED_CLK,1);
//    delay_millis(TIM15,2);

//    data = data >> 1;


//  }
//}

//void ledEnd() {
//  uint32_t data = 0xFFFFFFFF;
//  uint32_t msk = 0x1;

//  for(int i = 0; i < 32; i++) {
//    digitalWrite(PB3,0);
//    digitalWrite(PB0,data&msk);
//    delay_millis(TIM15, 2);
//    digitalWrite(PB3,1);
//    delay_millis(TIM15, 2);

//    data = data >> 1;


//  }
//}

//void ledClear() {
//  uint32_t data = 0x00000007;
//  uint32_t msk = 0x1;

//  for(int i = 0; i < 32; i++) {
//    digitalWrite(PB3,0);
//    digitalWrite(PB0,data&msk);
//    delay_millis(TIM15, 2);
//    digitalWrite(PB3,1);
//    delay_millis(TIM15, 2);

//    data = data >> 1;


//  }
//}
