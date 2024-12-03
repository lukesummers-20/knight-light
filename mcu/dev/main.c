#include <stdio.h>
#include "../lib/STM32L432KC.h"
#include "../lib/chess.h"
#include "../lib/lights.h"

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/ 

int main(void) {
  RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;

  initTIM(TIM15);

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);
  
  initSPI(1, 0, 0);

  pinMode(LED_SDI, GPIO_OUTPUT);
  pinMode(LED_CLK, GPIO_OUTPUT);
  pinMode(PA11, GPIO_OUTPUT);
  pinMode(PA10, GPIO_OUTPUT);
  pinMode(PA6, GPIO_INPUT);
  digitalWrite(LED_SDI, 0);
  digitalWrite(LED_CLK, 0);
  digitalWrite(PA11, 0);
  digitalWrite(PA10, 0);

  lightOneLed(LED_STARTUP);
  for(int i = 0; i < 64; i++){
    lightOneLed(LED_GREEN);
  }

  bool board[64];
  bool lights[64];
  bool row[8];
  struct piece pieces[32];
  struct boardChange update, lastUpdate;

  initGame(&pieces, &board);

  bool lastUp = 0;
  char boardText[8];

  int check;

  clearLights(&lights);
  lightLights(&lights);


  while(1){
    while(!digitalRead(PA6));

    for (int i = 0; i < 8; i++) {
      digitalWrite(PA11, 1);
      boardText[i] = spiSendReceive(0);
      digitalWrite(PA11, 0);
    }

    digitalWrite(PA10, 1);
    delay_millis(TIM15, 10);
    spiSendReceive(0);
    digitalWrite(PA10, 0);

    for (int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        row[j] = (boardText[i] & (1 << (7 - j))) ? 0 : 1;
        //!((bool)(((uint8_t)boardText[i]) >> j) & ((uint8_t) 0x1));
      }
      checkChange(&row, i, &board, &pieces, &update, &lastUpdate);
      if (lastUp) {
        if (update.up == 1){
          update.piece->r = -1;
          update.piece->c = -1;
          update.piece = lastUpdate.piece;
        } else if (update.up == 0){
          clearLights(&lights);
          lightLights(&lights);
          lastUp = 0;
          if ((update.piece->type.num == 0) & 
              (((update.piece->r == 7) & (update.piece->type.dir == 1)) | ((update.piece->r == 0) & (update.piece->type.dir == -1))))
          {
            update.piece->type.num = 4;
          }
          calcNextMoves(update.piece, &board, &pieces);
          check = checkCheck(&pieces, update.piece->type.dir);
          if (check){
          //light check
          }
        }
      } else {
        if (update.up == 1){
          lastUp = 1;
          clearLights(&lights);
          updateLightArray(update.piece, &lights);
          lightLights(&lights);
        }
      }
    }
  }
}

/*************************** End of file ****************************/
