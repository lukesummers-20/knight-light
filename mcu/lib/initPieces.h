

#ifndef INIT_PIECES_H
#define INIT_PIECES_H

#include "chess.h"

void emptyNextMoves(struct piece* piece);
struct piece initPawn(int r, int c, int dir);
struct piece initBishop(int r, int c);
struct piece initKnight(int r, int c, int dir);
struct piece initRook(int r, int c);
struct piece initQueen(int r, int c);
struct piece initKing(int r, int c);
void initPieces(struct piece* pieces);

#endif