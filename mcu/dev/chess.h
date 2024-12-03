
#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>
#include "pieces.h"
#include "board.h"

struct boardChange{
    struct piece* piece;
    int up;
};

void initGame(struct piece* pieces, bool* board);
void initBoardChange(struct boardChange* move);
void checkChange(bool* row, int r, bool* board, struct piece* pieces, struct boardChange* update, struct boardChange* lastUpdate);
void updateLightArray(struct piece* piece, bool* lights);
void clearLights(bool* lights);
int checkCheck(struct piece* pieces, int dir);
void lightLights(bool* lights);


#endif