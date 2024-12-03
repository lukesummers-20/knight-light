#include "board.h"


void initBoard(bool* board){
    for (int i = 0; i < 8; i++){
        if ((i < 2) | (i > 5)){
            for(int j = 0; j < 8; j++){
                *(board + (8 * i) + j) = 1;
            }
        } else {
            for(int j = 0; j < 8; j++) {
                *(board + (8 * i) + j) = 0;
            }
        } 
    }
}

void updateBoard(bool* board, int r, int c, int newR, int newC){
    *(board + (8 * r) + c) = 0;
    *(board + (8 * newR) + newC) = 1;
}

