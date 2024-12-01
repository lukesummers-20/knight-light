#include <stdio.h>
#include <stm32l432xx.h>
#include "../lib/STM32L432KC.h"

const int BOARD_SIZE = 64;
const int ROW_SIZE = 8;
const int COL_SIZE = 8;

#define LED_SDI PB0
#define LED_CLK PC14

void ledStartup() {
  uint32_t data = 0x00000000;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15,2);

    data = data >> 1;


  }

  
}

void ledRed() {
  uint32_t data = 0xFF0000FF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledGreen() {
  uint32_t data = 0xF000F0FF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledOrange() {
  uint32_t data = 0x00F0F0FF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledPurple() {
  uint32_t data = 0xF0F0F0FF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledYellow() {
  uint32_t data = 0x00FF00FF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledBlue() {
  uint32_t data = 0x0000FFFF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledEnd() {
  uint32_t data = 0xFFFFFFFF;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void ledClear() {
  uint32_t data = 0x00000007;
  uint32_t msk = 0x1;

  for(int i = 0; i < 32; i++) {
    digitalWrite(LED_CLK,0);
    digitalWrite(LED_SDI,data&msk);
    delay_millis(TIM15, 2);
    digitalWrite(LED_CLK,1);
    delay_millis(TIM15, 2);

    data = data >> 1;


  }
}

void boardClear() {
  ledStartup();
  for(int j = 0; j < BOARD_SIZE; j++) {
    ledClear();
  }
  ledClear();
}

void boardLightMoves(int spots[]) {
  int flippedMoves[64];
  for(int r = 0; r < ROW_SIZE; r++) {
    int oddRow = r%2;
    for(int c = 0; c < COL_SIZE; c++) {
      if(oddRow){
        flippedMoves[r*8+c] = spots[7+r*8-c];
      } else {
        flippedMoves[r*8+c] = spots[r*8+c];
      }
    }
  }
  ledStartup();
  for(int j = 0; j < BOARD_SIZE; j++) {
    if(flippedMoves[j] == 0) {
      ledClear();
    } else {
      ledOrange();
    }
  }
  ledClear();
}

int main(void) {
  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);
  
  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(LED_SDI, GPIO_OUTPUT); //sdi
  pinMode(LED_CLK, GPIO_OUTPUT); //clk
  


  
  
  
  digitalWrite(LED_CLK,0);
  digitalWrite(LED_SDI,0);

  
  
  int availableMoves[64] = {1,1,1,1,1,1,1,1,
                            0,0,0,0,0,0,1,1,
                            0,0,0,0,0,1,0,1,
                            0,0,0,0,1,0,0,1,
                            0,0,0,1,0,0,0,1,
                            0,0,1,0,0,0,0,1,
                            0,1,0,0,0,0,0,1,
                            1,0,0,0,0,0,0,1};
  //boardClear();

  //boardLightMoves(availableMoves);

  pinMode(PA11, GPIO_OUTPUT);
  digitalWrite(PA11, 0); 
  //SPI STUFF

  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 0, 0);


  // Load and done pins
  pinMode(PA10, GPIO_OUTPUT);  // LOAD
  pinMode(PA6, GPIO_INPUT);   // DONE
  

  // Artificial chip select signal to allow 8-bit CE-based SPI decoding on the logic analyzers.
  

  //digitalWrite(PA10, 1);
  //delay_millis(TIM15, 1000);
  ////spiSendReceive(0);
  //digitalWrite(PA10, 0);

  char boardText[8];
  // Wait for DONE signal to be asserted by FPGA signifying that the data is ready to be read out.
  while(!digitalRead(PA6));

  for(int i = 0; i < 8; i++) {
    digitalWrite(PA11, 1); // Arificial CE high
    boardText[i] = spiSendReceive(0);  
    digitalWrite(PA11, 0); // Arificial CE low
  }



  digitalWrite(PA10, 1);
  delay_millis(TIM15, 10);
  spiSendReceive(0);
  digitalWrite(PA10, 0);


  //int done = 3;
  //done = digitalRead(PA6);
  //digitalWrite(PA9, 1);
  //done = digitalRead(PA6);
  //delay_millis(TIM15, 10);
  //digitalWrite(PA9, 0);
  //done = digitalRead(PA6);
  //delay_millis(TIM15, 10);


  while(1) {
  while(!digitalRead(PA6));

  for(int i = 0; i < 8; i++) {
    digitalWrite(PA11, 1); // Arificial CE high
    boardText[i] = spiSendReceive(0);  
    digitalWrite(PA11, 0); // Arificial CE low
  }



  digitalWrite(PA10, 1);
  delay_millis(TIM15, 10);
  spiSendReceive(0);
  digitalWrite(PA10, 0);
  }
}