//Luke Summers lsummers@g.hmc.edu
//header file for chess piece constants and functions

#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>

struct pieceType{
    //0 is pawn, 1 is bishop, 2 is knight, 3 is rook, 4 is queen, 5 is queen
    int num;
    //1 for white and -1 for black
    int dir;
};

struct piece{
    //type of piece
    struct pieceType type;
    //array to hold next moves
    int nextMoves[28][2];
    //current row
    int r;
    //current column
    int c;
    //1 if piece is protected, 0 if not
    bool prot;
};

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
void initPawn(struct piece* piece, int r, int c, int dir);
void initBishop(struct piece* piece, int r, int c, int dir);
void initKnight(struct piece* piece, int r, int c, int dir);
void initRook(struct piece* piece, int r, int c, int dir);
void initQueen(struct piece* piece, int r, int c, int dir);
void initKing(struct piece* piece, int r, int c, int dir);
void initPieces(struct piece* pieces);
struct piece* getPiece(int r, int c, struct piece* pieces);
void calcNextMoves(int num, bool* board, bool* attacked, bool* attacking, struct piece* pieces);
void calcPawnMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces);
void calcBishopMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces);
void calcKnightMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces);
void calcRookMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces);
void calcQueenMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces);
void calcKingMoves(struct piece* piece, bool* board, bool* attacking, bool* attacked, struct piece* pieces);

#endif