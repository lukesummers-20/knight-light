//Luke Summers lsummers@g.hmc.edu
//header file for chess logic functions

#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>
#include <stdint.h>
#include "pieces.h"
#include "board.h"

struct boardChange{
    //piece that was moved
    struct piece* piece;
    //1 if piece picked up, 0 if put down
    int up;
};

void initGame(struct piece* pieces, bool* board);
void checkChange(bool* row, int r, bool* board, struct piece* pieces, struct boardChange* update, struct boardChange* lastUpdate, int lastUp);
void updateLightArray(struct piece* piece, bool* lights);
void clearLights(bool* lights);
int checkCheck(struct piece* pieces, int dir);
void lightLights(bool* lights, uint32_t color);
void lightCheck(struct piece* pieces, bool* lights, int dir);
void clearAttacked(bool* attacked);
int checkMate(struct piece* pieces, int dir);
void fillLights(bool* lights);


#endif