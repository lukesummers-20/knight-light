
#ifndef CHESS_H
#define CHESS_h

#include <stdbool.h>
#include "board.h"
#include "../lib/pieces.h"

struct pieceType{
    int num;
    int dir;
};

struct piece{
    struct pieceType type;
    int nextMoves[28][2];
    int r;
    int c;
};

struct boardChange{
    struct piece piece;
    int up;
};

void initGame(struct piece* pieces, bool* board);

#endif