#include <stdio.h>
#include <stm32l432xx.h>
#include "../lib/STM32L432KC.h"

void ledStartup() {
  uint32_t data = 0x00000000;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(PB3,0);
    digitalWrite(PB0,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(PB3,1);
    delay_millis(TIM15,2);

    data = data >> 1;


  }

  
}

void ledRed() {
  uint32_t data = 0xFF0000FF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(PB3,0);
    digitalWrite(PB0,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(PB3,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledGreen() {
  uint32_t data = 0x00FF00FF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(PB3,0);
    digitalWrite(PB0,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(PB3,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledBlue() {
  uint32_t data = 0x0000FFFF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(PB3,0);
    digitalWrite(PB0,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(PB3,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledEnd() {
  uint32_t data = 0xFFFFFFFF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(PB3,0);
    digitalWrite(PB0,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(PB3,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledClear() {
  uint32_t data = 0x00000007;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(PB3,0);
    digitalWrite(PB0,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(PB3,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

int main(void) {
  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);
  
  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(PB0, GPIO_OUTPUT); //sdi
  pinMode(PB3, GPIO_OUTPUT); //clk
  
  //digitalWrite(PB3,0);
  //digitalWrite(PB0,0);


  
  
  
  digitalWrite(PB3,0);
  digitalWrite(PB0,0);

  ledStartup();
    
    for(int j = 0; j < 16; j++) {
    ledBlue();
    ledClear();
    ledClear();
    ledBlue();

  }

  while(1) {
  

  }
}