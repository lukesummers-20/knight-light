

#ifndef PIECES_H
#define PIECES_H

#include "chess.h"

#define WR1 0
#define WKn1 1
#define WB1 2
#define WQ 3
#define WKi 4
#define WB2 5
#define WKn2 6
#define WR2 7
#define WA 8
#define WB 9
#define WC 10
#define WD 11
#define WE 12
#define WF 13
#define WG 14
#define WH 15
#define BR1 16
#define BKn1 17
#define BB1 18
#define BQ 19
#define BKi 20
#define BB2 21
#define BKn2 22
#define BR2 23
#define BA 24
#define BB 25
#define BC 26
#define BD 27
#define BE 28
#define BF 29
#define BG 30
#define BH 31

void emptyNextMoves(struct piece* piece);
struct piece initPawn(int r, int c, int dir);
struct piece initBishop(int r, int c);
struct piece initKnight(int r, int c, int dir);
struct piece initRook(int r, int c);
struct piece initQueen(int r, int c);
struct piece initKing(int r, int c);
void initPieces(struct piece* pieces);
struct piece getPiece(int r, int c, struct piece* pieces);
void calcNextMoves(struct piece* piece, bool* board, struct piece* pieces);

#endif