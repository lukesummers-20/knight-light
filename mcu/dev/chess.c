#include "chess.h"
#include "lights.h"
#include <stdio.h>

int checkMate(struct piece* pieces, int dir){
    struct piece king, p;
    int j, k;
    if (dir == -1){
        king = *(pieces + WKi);
        j = 0;
        while ((j < 28) & (king.nextMoves[j][0] != -1)){
            for(int i = 16; i < 32; i++){
                p = *(pieces + i);
                k = 0;
                while((k < 28) & (p.nextMoves[k][0] != -1)){
                    if ((king.nextMoves[j][0] == p.nextMoves[k][0]) & (king.nextMoves[j][1] == p.nextMoves[k][1])){

                    }
                }
            }
            j++;
        }
    } else if (dir == 1){
        king = *(pieces + BKi);
    }
    return 0;
}

int checkCheck(struct piece* pieces, int dir){
    struct piece king, p;
    //dir is dir of piece put down
    if (dir == -1){
        king = *(pieces + WKi);
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

void initGame(struct piece* pieces, bool* board){
    initPieces(pieces);
    initBoard(board);
}

void initBoardChange(struct boardChange* move){
    move->piece->type.num = 99;
    move->piece->type.dir = 99;
    move->piece->r = 99;
    move->piece->c = 99;
    move->up = 99;
}

void checkChange(bool* row, int r, bool* board, struct piece* pieces, struct boardChange* update, struct boardChange* lastUpdate){
    initBoardChange(update);
    for (int i = 0; i < 8; i++){
        if(*(row + i) != *(board + (8 * r) + i)){
            update->up = (!(*(row + i)));
            if (update->up){
                //cannot return structs, only ptr to struct
                update->piece = getPiece(r, i, pieces);
                *(board + (8 * r) + i) = 0;
            } else {
                update->piece = lastUpdate->piece;
                update->piece->r = r;
                update->piece->c = i;
                *(board + (8 * r) + i) = 1;
            }
        } 
    }
}

void updateLightArray(struct piece* piece, bool* lights){
    int i = 0;
    while ((i < 28) & (piece->nextMoves[i][0] != -1)){
        *(lights + (8 * piece->nextMoves[i][0]) + piece->nextMoves[i][1]) = 1;
    }
    for(int i = 0; i < 64; i++){
      if (*(lights + i)){
        printf("1");
      } else {
        printf("0");
      }
      if (i % 8 == 7){
        printf("/n");
      }
    }
}

void clearLights(bool* lights){
    for(int i = 0; i < 64; i++){
        *(lights + i) = 0;
    }
}

void lightLights(bool* lights){
    bool snaked[64];
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        if (i%2){
          snaked[8 * i + j] = *(lights + 7 + 8 * i - j);
        } else {
          snaked[8 * i + j] = *(lights + 8 * i + j);
        }
      }
    }
    lightOneLed(LED_STARTUP);
    for(int i = 0; i < 64; i++){
        if (snaked[i]){
            lightOneLed(LED_GREEN);
        } else {
            lightOneLed(LED_BLUE);
        }
    }
}
