//Luke Summers lsummers@g.hmc.edu
//source file for chess logic functions

#include "chess.h"
#include "lights.h"
#include <stdio.h>

//check if a checkmate has occurred
//checks to see if the opposing king has any moves to make
int checkMate(struct piece* pieces, int dir){
    if (dir == -1) {
      if ((pieces + WKi)->nextMoves[0][0] == -1) {
        return 1;
      }
    } else if (dir == 1) {
      if ((pieces + BKi)->nextMoves[0][0] == -1) {
        return 1;
      }
    }
    return 0;
}

//clears the array for which squares are attacked
void clearAttacked(bool* attacked) {
  for(int i = 0; i < 64; i++) {
    *(attacked + i) = 0;
  }
}

//checks to see if the oppposing king is in check
int checkCheck(struct piece* pieces, int dir){
    struct piece king, p;
    if (dir == -1){
        king = *(pieces + WKi);
        //see if any of the opposing pieces have a move to the king square
        for (int i = 16; i < 32; i++){
            int j = 0;
            p = *(pieces + i);
            while ((j < 28) & (p.nextMoves[j][0] != -1)){
                if ((p.nextMoves[j][0] == king.r) & (p.nextMoves[j][1] == king.c)){
                    return 1;
                }
                j++;
            }
        }
    } else if (dir == 1){
        king = *(pieces + BKi);
        //see if any of the opposing pieces have a move to the king square
        for (int i = 0; i < 16; i++){
            int j = 0;
            p = *(pieces + i);
            while ((j < 28) & (p.nextMoves[j][0] != -1)){
                if ((p.nextMoves[j][0] == king.r) & (p.nextMoves[j][1] == king.c)){
                    return 1;
                }
                j++;
            }
        }
    }
    return 0;
}

//sets the kings square to be lit for check
void lightCheck(struct piece* pieces, bool* lights, int dir){
  clearLights(lights);
  if(dir == 1){
    *(lights + ((pieces + BKi)->r * 8) + (pieces + BKi)->c) = 1;
  } else if (dir == -1){
    *(lights + ((pieces + WKi)->r * 8) + (pieces + WKi)->c) = 1;
  }
}

//initialized the board array and piece array
void initGame(struct piece* pieces, bool* board){
    initPieces(pieces);
    initBoard(board);
}

//check if the given row has a change from the current board state
void checkChange(bool* row, int r, bool* board, struct piece* pieces, struct boardChange* update, struct boardChange* lastUpdate, int lastUp){
    update->up = -1;
    for (int i = 0; i < 8; i++){
        if(*(row + i) != *(board + (8 * r) + i)){
            *lastUpdate = *update;
            update->up = (!(*(row + i)));
            if ((update->up) & (lastUp == 1)){
                //piece is captured
                struct piece* p = getPiece(r, i, pieces);
                p->c = -1;
                p->r = -1;
                *(board + (8 * r) + i) = 0;
            } else if (update->up){
                //piece is picked up
                update->piece = getPiece(r, i, pieces);
                *(board + (8 * r) + i) = 0;
            } else if (lastUp == 1){
                //piece is put down
                update->piece = lastUpdate->piece;
                update->piece->r = r;
                update->piece->c = i;
                *(board + (8 * r) + i) = 1;
            }
        } 
    }
}

//update light array with next moves
void updateLightArray(struct piece* piece, bool* lights){
    int i = 0;
    while ((i < 28) & (piece->nextMoves[i][0] != -1)){
        *(lights + (8 * piece->nextMoves[i][0]) + piece->nextMoves[i][1]) = 1;
        i++;
    }
}

//clear the light array
void clearLights(bool* lights){
    for(int i = 0; i < 64; i++){
        *(lights + i) = 0;
    }
}

//fill the light array
void fillLights(bool* lights){
  for(int i = 0; i < 64; i++){
    *(lights + i) = 1;
  }
}

//light up the lights based on light array
void lightLights(bool* lights, uint32_t color){
    bool snaked[64];
    //light strip is snaked so the odd rows have to be reversed
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        if (i%2){
          snaked[8 * i + j] = *(lights + 8 * i + 7 - j);
        } else {
          snaked[8 * i + j] = *(lights + 8 * i + j);
        }
      }
    }
    lightOneLed(LED_STARTUP);
    for(int i = 63; i > -1; i--){
        if (snaked[i]){
            lightOneLed(color);
        } else {
            lightOneLed(LED_BLUE);
        }
    }
    lightOneLed(LED_OFF);
}
