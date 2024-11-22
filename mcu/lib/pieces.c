#include "pieces.h"

void emptyNextMoves(struct piece* piece){
    for (int i = 0; i < 28; i++){
        piece->nextMoves[i][0] = -1;
        piece->nextMoves[i][1] = -1;
    }
}

struct piece initPawn(int r, int c, int dir){
    struct piece pawn;
    pawn.type.num = 0;
    pawn.type.dir = dir;
    emptyNextMoves(&pawn);
    pawn.nextMoves[0][0] = r + dir;
    pawn.nextMoves[0][1] = c;
    pawn.r = r;
    pawn.c = c;
    return pawn;
}

struct piece initBishop(int r, int c){
    struct piece bishop;
    bishop.type.num = 1;
    emptyNextMoves(&bishop);
    bishop.r = r;
    bishop.c = c;
    return bishop;
}

struct piece initKnight(int r, int c, int dir){
    struct piece knight;
    knight.type.num = 2;
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
    rook.type.num = 3;
    emptyNextMoves(&rook);
    rook.r = r;
    rook.c = c;
    return rook;
}

struct piece initQueen(int r, int c){
    struct piece queen;
    queen.type.num = 4;
    emptyNextMoves(&queen);
    queen.r = r;
    queen.c = c;
    return queen;
}

struct piece initKing(int r, int c){
    struct piece king;
    king.type.num = 5;
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

void updatePiece(struct piece* pieces, int piece, int r, int c){
    pieces[piece].c = c;
    pieces[piece].r = r;
    emptyNextMoves(pieces + piece);

}

struct piece getPiece(int r, int c, struct piece* pieces){
    struct piece p;
    for (int i = 0; i < 32; i++){
        p = *(pieces + i);
        if ((p.r == r) & (p.c == c)) {
            return p;
        }
    }
    return p;
}

void calcNextMoves(struct piece* piece, bool* board, struct piece* pieces){
    emptyNextMoves(piece);
    switch(piece->type.num){
        case 0: calcPawnMoves(piece, board, pieces);
        case 1: calcBishopMoves(piece, board, pieces);
        case 2: calcKnightMoves(piece);
        case 3: calcRookMoves(piece);
        case 4: calcQueenMoves(piece);
        case 5: calcKingMoves(piece);
    }
}
//add dir to all piece init
//make sure to check captures for al calc moves(all need pieces passed in)
//replace magic nums in inits with defs

void calcPawnMoves(struct piece* piece, bool* board, struct piece* pieces){
    piece->nextMoves[0][0] = piece->r + piece->type.dir;
    piece->nextMoves[0][1] = piece->c;
    int index = 1;
    struct piece p;
    if (*(board + (8 * (piece->r + piece->type.dir) + piece->c + 1))){
        p = getPiece(piece->r + piece->type.dir, piece->c + 1, pieces);
        if (piece->type.dir != p.type.dir){
            piece->nextMoves[index][0] = piece->r + piece->type.dir;
            piece->nextMoves[index][1] = piece->c + 1;
            index = index + 1;
        }
    }
    if (board + (8 * (piece->r + piece->type.dir) + piece->c - 1)){
        p = getPiece(piece->r + piece->type.dir, piece->c - 1, pieces);
        if (piece->type.dir != p.type.dir){
            piece->nextMoves[index][0] = piece->r + piece->type.dir;
            piece->nextMoves[index][1] = piece->c - 1;
        }
    }
}

void calcBishopMoves(struct piece* piece, bool* board, struct piece* pieces){
    int i, j;
    struct piece p;
    i = piece->r + 1;
    j = piece->c + 1;
    int index = 0;
    while((i < 8) & (j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        i = i + 1;
        j = j + 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j)){
        p = getPiece(i , j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r + 1;
    j = piece->c - 1;
    while((i < 8) & (j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        i = i + 1;
        j = j - 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j)){
        p = getPiece(i , j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r - 1;
    j = piece->c - 1;
    while((i > -1) & (j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        i = i - 1;
        j = j - 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j)){
        p = getPiece(i , j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r - 1;
    j = piece->c + 1;
    while((i > -1) & (j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        i = i - 1;
        j = j - 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j)){
        p = getPiece(i , j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
}

void calcKnightMoves(struct piece* piece){

}

void calcRookMoves(struct piece* piece){

}

void calcQueenMoves(struct piece* piece){

}

void calcKingMoves(struct piece* piece){

}