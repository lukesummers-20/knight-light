#include "initPieces.h"

void emptyNextMoves(struct piece* piece){
    for (int i = 0; i < 28; i++){
        piece->nextMoves[i][0] = -1;
        piece->nextMoves[i][1] = -1;
    }
}

struct piece initPawn(int r, int c, int dir){
    struct piece pawn;
    pawn.piece.type = 0;
    pawn.piece.dir = dir;
    emptyNextMoves(&pawn);
    pawn.nextMoves[0][0] = r + dir;
    pawn.nextMoves[0][1] = c;
    pawn.r = r;
    pawn.c = c;
    return pawn;
}

struct piece initBishop(int r, int c){
    struct piece bishop;
    bishop.piece.type = 1;
    emptyNextMoves(&bishop);
    bishop.r = r;
    bishop.c = c;
    return bishop;
}

struct piece initKnight(int r, int c, int dir){
    struct piece knight;
    knight.piece.type = 2;
    emptyNextMoves(&knight);
    knight.nextMoves[0][0] = 2 * dir + r;
    knight.nextMoves[0][1] = c + 1;
    knight.nextMoves[1][0] = 2 * dir + r;
    knight.nextMoves[1][1] = c - 1;
    knight.r = r;
    knight.c = c;
    return knight;
}

struct piece initRook(int r, int c){
    struct piece rook;
    rook.piece.type = 3;
    emptyNextMoves(&rook);
    rook.r = r;
    rook.c = c;
    return rook;
}

struct piece initQueen(int r, int c){
    struct piece queen;
    queen.piece.type = 4;
    emptyNextMoves(&queen);
    queen.r = r;
    queen.c = c;
    return queen;
}

struct piece initKing(int r, int c){
    struct piece king;
    king.piece.type = 5;
    emptyNextMoves(&king);
    king.r = r;
    king.c = c;
    return king;
}

void initPieces(struct piece* pieces){
    //white
    *(pieces) = initRook(0, 0);
    *(pieces + 1) = initKnight(0, 1, 1);
    *(pieces + 2) = initBishop(0, 2);
    *(pieces + 3) = initQueen(0, 3);
    *(pieces + 4) = initKing(0, 4);
    *(pieces + 5) = initBishop(0, 5);
    *(pieces + 6) = initKnight(0, 6, 1);
    *(pieces + 7) = initRook(0, 7);
    for (int i = 0; i < 8; i++){
        *(pieces + 8 + i) = initPawn(1, i, 1);
    }
    //black
    *(pieces + 16) = initRook(7, 0);
    *(pieces + 17) = initKnight(7, 1, -1);
    *(pieces + 18) = initBishop(7, 2);
    *(pieces + 19) = initQueen(7, 3);
    *(pieces + 20) = initKing(7, 4);
    *(pieces + 21) = initBishop(7, 5);
    *(pieces + 22) = initKnight(7, 6, -1);
    *(pieces + 23) = initRook(7, 7);
    for (int i = 0; i < 8; i++){
        *(pieces + 24 + i) = initPawn(6, i, -1);
    }
}