//Luke Summers lsummers@g.hmc.edu
//main file for interfacing with chessboard and fpga

#include <stdio.h>
#include "../lib/STM32L432KC.h"
#include "../lib/chess.h"
#include "../lib/lights.h"

int main(void) {
  //enable timer 16 clock
  RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;

  initTIM(TIM16);

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

  //flash the board lights on startup
  lightOneLed(LED_STARTUP);
  for(int i = 0; i < 64; i++){
    lightOneLed(LED_GREEN);
  }

  //variables for chess logic

  //board occupation status
  bool board[64];
  //squares white pieces are attacking
  bool whiteAttacked[64];
  //squares black pieces are attacking
  bool blackAttacked[64];
  //squares to light up
  bool lights[64];
  //new row to check
  bool row[8];
  //pieces array
  struct piece pieces[32];
  //hold updates as they come
  struct boardChange update, lastUpdate;

  initGame(&pieces, &board);

  bool lastUp = 0;
  //variable for what is recieved over SPI
  char boardText[8];

  int check, mate;

  clearLights(&lights);
  lightLights(&lights, LED_GREEN);

  while(1){
    //wait for done from FPGA
    while(!digitalRead(PA6));

    for (int i = 0; i < 8; i++) {
      digitalWrite(PA11, 1);
      boardText[i] = spiSendReceive(0);
      digitalWrite(PA11, 0);
    }

    //enable FPGA to capture new board state
    digitalWrite(PA10, 1);
    delay_millis(TIM16, 10);
    spiSendReceive(0);
    digitalWrite(PA10, 0);

    for (int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        row[j] = (boardText[i] & (1 << (7 - j))) ? 0 : 1;
      }
      checkChange(&row, i, &board, &pieces, &update, &lastUpdate, lastUp);
      if (lastUp) {
        if (update.up == 0){
          //case for piece being picked up

          //clear the lights
          clearLights(&lights);
          lightLights(&lights, LED_GREEN);
          lastUp = 0;
          //promote pawn if necessary
          if ((update.piece->type.num == 0) & 
              (((update.piece->r == 7) & (update.piece->type.dir == 1)) | ((update.piece->r == 0) & (update.piece->type.dir == -1))))
          {
            update.piece->type.num = 4;
          }
          //light entire board green to signify calculations occuring
          lightOneLed(LED_STARTUP);
          for(int i = 0; i < 64; i++){
            lightOneLed(LED_GREEN);
          }
          clearAttacked(&whiteAttacked);
          clearAttacked(&blackAttacked);
          //update the next moves for all the pieces
          for(int i = 0; i < 16; i++){
            if (i != WKi){
              calcNextMoves(i, &board, &blackAttacked, &whiteAttacked, &pieces);
            }
          }
          for(int i = 16; i < 32; i++){
            if (i != BKi){
              calcNextMoves(i, &board, &whiteAttacked, &blackAttacked, &pieces);
            }
          }
          //need to calc next moves for kings after the rest of pieces so attacked squares and protected pieces are known
          calcNextMoves(BKi, &board, &whiteAttacked, &blackAttacked, &pieces);
          calcNextMoves(WKi, &board, &blackAttacked, &whiteAttacked, &pieces);
          calcNextMoves(BKi, &board, &whiteAttacked, &blackAttacked, &pieces);
          check = checkCheck(&pieces, update.piece->type.dir);
          //light board for check/mate if necessary, if not then light no moves
          if (check){
            mate = checkMate(&pieces, update.piece->type.dir);
            if(mate){
              fillLights(&lights);
              lightLights(&lights, LED_RED);
              while(1){}
            } else {
              lightCheck(&pieces, &lights, update.piece->type.dir);
              lightLights(&lights, LED_RED);
            }
          } else {
            lightLights(&lights, LED_GREEN);
          }
        }
      } else {
        if (update.up == 1){
          //case for piece being picked up
          lastUp = 1;
          clearLights(&lights);
          updateLightArray(update.piece, &lights);
          lightLights(&lights, LED_GREEN);
        }
      }
    }
  }
}
