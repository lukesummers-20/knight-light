//Luke Summers lsummers@g.hmc.edu
//source file for board logic functions

#include "board.h"

//function to initialize the boolean array representing the board to 
//the starting state of a chess game
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

