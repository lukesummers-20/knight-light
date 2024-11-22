

#ifndef BOARD_H
#define BOARD_H

#include "chess.h"

void initBoard(bool* board);
void updateBoard(bool* board, int r, int c, int newR, int newC);

#endif