//Luke Summers lsummers@g.hmc.edu
//Source code for piece functions

#include "pieces.h"
#include "chess.h"
#include <stdio.h>

//empty the next moves array for a piece
void emptyNextMoves(struct piece* piece){
    for (int i = 0; i < 28; i++){
        piece->nextMoves[i][0] = -1;
        piece->nextMoves[i][1] = -1;
    }
}

//initialize a pawn piece
void initPawn(struct piece* piece, int r, int c, int dir){
    piece->type.num = 0;
    piece->type.dir = dir;
    emptyNextMoves(piece);
    piece->nextMoves[0][0] = r + dir;
    piece->nextMoves[0][1] = c;
    piece->nextMoves[1][0] = r + (2 * dir);
    piece->nextMoves[1][1] = c;
    piece->r = r;
    piece->c = c;
    piece->prot = 0;
}

//initialize a bishop piece
void initBishop(struct piece* piece, int r, int c, int dir){
    piece->type.num = 1;
    piece->type.dir = dir;
    emptyNextMoves(piece);
    piece->r = r;
    piece->c = c;
    piece->prot = 0;
}

//initialize a knight piece
void initKnight(struct piece* piece, int r, int c, int dir){
    piece->type.num = 2;
    piece->type.dir = dir;
    emptyNextMoves(piece);
    piece->nextMoves[0][0] = 2 * dir + r;
    piece->nextMoves[0][1] = c + 1;
    piece->nextMoves[1][0] = 2 * dir + r;
    piece->nextMoves[1][1] = c - 1;
    piece->r = r;
    piece->c = c;
    piece->prot = 0;
}

//initialize a rook piece
void initRook(struct piece* piece, int r, int c, int dir){
    piece->type.num = 3;
    piece->type.dir = dir;
    emptyNextMoves(piece);
    piece->r = r;
    piece->c = c;
    piece->prot = 0;
}

//initialize a queen piece
void initQueen(struct piece* piece, int r, int c, int dir){
    piece->type.num = 4;
    piece->type.dir = dir;
    emptyNextMoves(piece);
    piece->r = r;
    piece->c = c;
    piece->prot = 0;
}

//initialize a king piece
void initKing(struct piece* piece, int r, int c, int dir){
    piece->type.num = 5;
    piece->type.dir = dir;
    emptyNextMoves(piece);
    piece->r = r;
    piece->c = c;
    piece->prot = 0;
}

//initialize the pieces array
void initPieces(struct piece* pieces){
    //white
    initRook(pieces + WR1, 0, 0, 1);
    initKnight(pieces + WKn1, 0, 1, 1);
    initBishop(pieces + WB1, 0, 2, 1);
    initQueen(pieces + WQ, 0, 3, 1);
    initKing(pieces + WKi, 0, 4, 1);
    initBishop(pieces + WB2, 0, 5, 1);
    initKnight(pieces + WKn2, 0, 6, 1);
    initRook(pieces + WR2, 0, 7, 1);
    for (int i = 0; i < 8; i++){
        initPawn(pieces + 8 + i, 1, i, 1);
    }
    //black
    initRook(pieces + BR1, 7, 0, -1);
    initKnight(pieces + BKn1, 7, 1, -1);
    initBishop(pieces + BB1, 7, 2, -1);
    initQueen(pieces + BQ, 7, 3, -1);
    initKing(pieces + BKi, 7, 4, -1);
    initBishop(pieces + BB2, 7, 5, -1);
    initKnight(pieces + BKn2, 7, 6, -1);
    initRook(pieces + BR2, 7, 7, -1);
    for (int i = 0; i < 8; i++){
        initPawn(pieces + 24 + i, 6, i, -1);
    }
}

//go through pieces array and find the piece at r, c
struct piece* getPiece(int r, int c, struct piece* pieces){
    for (int i = 0; i < 32; i++){
        if (((*(pieces + i)).r == r) & ((*(pieces + i)).c == c)) {
            return pieces + i;
        }
    }
    return pieces;
}

//calculate the next moves for a pawn piece
void calcPawnMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces){
    int index = 0;
    //square in front of cur pos
    if (!(*(board + (8 * (piece->r + piece->type.dir) + piece->c)))){
      piece->nextMoves[index][0] = piece->r + piece->type.dir;
      piece->nextMoves[index][1] = piece->c;
      index = index + 1;
    }
    //can move two squares if at starting pos
    if ( (((piece->r == 1) & (piece->type.dir == 1)) | ((piece->r == 6) & (piece->type.dir == -1))) & (!(*(board + (8 * (piece->r + (2 * piece->type.dir)) + piece->c))))){
      piece->nextMoves[index][0] = piece->r + (2 * piece->type.dir);
      piece->nextMoves[index][1] = piece->c;
      index = index + 1;
    }
    //can move diagonal if it is a capture
    struct piece* p;
    if (*(board + (8 * (piece->r + piece->type.dir) + piece->c + 1))){
        p = getPiece(piece->r + piece->type.dir, piece->c + 1, pieces);
        if (piece->type.dir != p->type.dir){
            piece->nextMoves[index][0] = piece->r + piece->type.dir;
            piece->nextMoves[index][1] = piece->c + 1;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    } else {
      *(attacked + (8 * (piece->r + piece->type.dir)) + piece->c + 1) = 1;
    }
    if (*(board + (8 * (piece->r + piece->type.dir) + piece->c - 1))){
        p = getPiece(piece->r + piece->type.dir, piece->c - 1, pieces);
        if (piece->type.dir != p->type.dir){
            piece->nextMoves[index][0] = piece->r + piece->type.dir;
            piece->nextMoves[index][1] = piece->c - 1;
        } else {
            p->prot = 1;
        }
    } else {
      *(attacked + (8 * (piece->r + piece->type.dir)) + piece->c - 1) = 1;
    }
}

//calculate the next moves for a bishop piece
void calcBishopMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces){
    int i, j;
    struct piece* p;
    i = piece->r + 1;
    j = piece->c + 1;
    int index = 0;
    //loop to either the edge of the board or an occupied square for each direction
    while((i < 8) & (j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i + 1;
        j = j + 1;
        index = index + 1;
    }
    //if stopped because of occupied square, it can either capture or protect the piece
    if (*(board + (8 * i) + j) & ((i < 8) & (j < 8))){
        p = getPiece(i , j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r + 1;
    j = piece->c - 1;
    while((i < 8) & (j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i + 1;
        j = j - 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j) & ((i < 8) & (j > -1))){
        p = getPiece(i , j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r - 1;
    j = piece->c - 1;
    while((i > -1) & (j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i - 1;
        j = j - 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j) & ((i > -1) & (j > -1))){
        p = getPiece(i , j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r - 1;
    j = piece->c + 1;
    while((i > -1) & (j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i - 1;
        j = j + 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j) & ((i > -1) & (j < 8))){
        p = getPiece(i , j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
}

//calculate next moves for a knight piece
void calcKnightMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces){
    struct piece* p;
    int index = 0;
    if (piece->r == 0){
        if (piece->c == 0){
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
                
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 1) {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 6) {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
        } else if (piece->c == 7){
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
        } else {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
        }
    } else if (piece->r == 1){
        if (piece->c == 0){
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 1) {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 6) {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        } else if (piece->c == 7){
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        } else {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        }
    } else if (piece->r == 6) {
        if (piece->c == 0){
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 1) {
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 6) {
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        } else if (piece->c == 7){
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        } else {
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        }
    } else if (piece->r == 7){
        if (piece->c == 0){
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 1) {
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 6) {
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        } else if (piece->c == 7){
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        } else {
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        }
    } else {
        if (piece->c == 0){
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 1) {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
        } else if (piece->c == 6) {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        } else if (piece->c == 7){
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        } else {
            //up 2 right
            if (*(board + (8 * (piece->r + 2)) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c + 1) = 1;
            }
            //up 2 left
            if (*(board + (8 * (piece->r + 2)) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r + 2)) + piece->c - 1) = 1;
            }
            //up right 2
            if (*(board + (8 * (piece->r + 1)) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c + 2) = 1;
            }
            //up left 2
            if (*(board + (8 * (piece->r + 1)) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r + 1)) + piece->c - 2) = 1;
            }
            //down 2 right
            if (*(board + (8 * (piece->r - 2)) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c + 1) = 1;
            }
            //down 2 left
            if (*(board + (8 * (piece->r - 2)) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
                *(attacked + (8 * (piece->r - 2)) + piece->c - 1) = 1;
            }
            //down right 2
            if (*(board + (8 * (piece->r - 1)) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c + 2) = 1;
            }
            //down left 2
            if (*(board + (8 * (piece->r - 1)) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p->type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                } else {
                    p->prot = 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
                *(attacked + (8 * (piece->r - 1)) + piece->c - 2) = 1;
            }
        }
    }
}

//calculate next moves for a rook piece
void calcRookMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces){
    int i, j;
    struct piece* p;
    int index = 0;
    i = piece->r + 1;
    j = piece->c;
    //loop to edge of board or occupied square
    while ((i < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i + 1;
        index = index + 1;
    }
    //if stopped by occupied square, it can either capture or protect
    if ((i < 8) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r - 1;
    j = piece->c;
    while ((i > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i - 1;
        index = index + 1;
    }
    if ((i > -1) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r;
    j = piece->c + 1;
    while ((j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        j = j + 1;
        index = index + 1;
    }
    if ((j < 8) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r;
    j = piece->c - 1;
    while ((j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        j = j - 1;
        index = index + 1;
    }
    if ((j > -1) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
}

//calculate next moves for a queen piece
//combination of rook and bishop movement
void calcQueenMoves(struct piece* piece, bool* board, bool* attacked, struct piece* pieces){
    int i, j;
    struct piece* p;
    i = piece->r + 1;
    j = piece->c + 1;
    int index = 0;
    while((i < 8) & (j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i + 1;
        j = j + 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j)& ((i < 8) & (j < 8))){
        p = getPiece(i , j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r + 1;
    j = piece->c - 1;
    while((i < 8) & (j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i + 1;
        j = j - 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j) & ((i < 8) & (j > -1))){
        p = getPiece(i , j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r - 1;
    j = piece->c - 1;
    while((i > -1) & (j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i - 1;
        j = j - 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j) & ((i > -1) & (j > -1))){
        p = getPiece(i , j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r - 1;
    j = piece->c + 1;
    while((i > -1) & (j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i - 1;
        j = j + 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j) & ((i > -1) & (j < 8))){
        p = getPiece(i , j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r + 1;
    j = piece->c;
    while ((i < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i + 1;
        index = index + 1;
    }
    if ((i < 8) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r - 1;
    j = piece->c;
    while ((i > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        i = i - 1;
        index = index + 1;
    }
    if ((i > -1) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r;
    j = piece->c + 1;
    while ((j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        j = j + 1;
        index = index + 1;
    }
    if ((j < 8) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
    i = piece->r;
    j = piece->c - 1;
    while ((j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        *(attacked + (8 * i) + j) = 1;
        j = j - 1;
        index = index + 1;
    }
    if ((j > -1) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p->type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        } else {
            p->prot = 1;
        }
    }
}

//calculate next moves for a king piece
void calcKingMoves(struct piece* piece, bool* board, bool* attacking, bool* attacked, struct piece* pieces){
    struct piece* p;
    int index = 0;
    //can go one square in any direction
    if ((piece->r > 0) & (piece->r < 7)){
        if ((piece-> c > 0) & (piece->c < 7)){
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if (!(*(attacked + (8 * piece->r) + piece->c + 1))){
                  piece->nextMoves[index][0] = piece->r;
                  piece->nextMoves[index][1] = piece->c + 1;
                  *(attacking + (8 * (piece->r)) + piece->c + 1) = 1;
                  index = index + 1;
                }
            }
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if (!(*(attacked + (8 * piece->r) + piece->c - 1))){
                  piece->nextMoves[index][0] = piece->r;
                  piece->nextMoves[index][1] = piece->c - 1;
                  *(attacking + (8 * (piece->r)) + piece->c - 1) = 1;
                  index = index + 1;
                }
            }
            //up
            if (*(board + (8 * (piece->r + 1)) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * (piece->r + 1)) + piece->c))){
                  piece->nextMoves[index][0] = piece->r + 1;
                  piece->nextMoves[index][1] = piece->c;
                  *(attacking + (8 * (piece->r + 1)) + piece->c) = 1;
                  index = index + 1;
                }
            }
            //down
            if (*(board + (8 * (piece->r - 1)) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * (piece->r - 1)) + piece->c))){
                  piece->nextMoves[index][0] = piece->r - 1;
                  piece->nextMoves[index][1] = piece->c;
                  *(attacking + (8 * (piece->r - 1)) + piece->c) = 1;
                  index = index + 1;
                }
            }
            //up right
            if (*(board + (8 * (piece->r + 1)) + piece->c + 1)){
                p = getPiece(piece->r + 1, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * (piece->r + 1)) + piece->c + 1))){
                  piece->nextMoves[index][0] = piece->r + 1;
                  piece->nextMoves[index][1] = piece->c + 1;
                  *(attacking + (8 * (piece->r + 1)) + piece->c + 1) = 1;
                  index = index + 1;
                }
            }
            //up left
            if (*(board + (8 * (piece->r + 1)) + piece->c - 1)){
                p = getPiece(piece->r + 1, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * (piece->r + 1)) + piece->c - 1))){
                  piece->nextMoves[index][0] = piece->r + 1;
                  piece->nextMoves[index][1] = piece->c - 1;
                  *(attacking + (8 * (piece->r + 1)) + piece->c - 1) = 1;
                  index = index + 1;
                }
            }
            //down right
            if (*(board + (8 * (piece->r - 1)) + piece->c + 1)){
                p = getPiece(piece->r - 1, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * (piece->r - 1)) + piece->c + 1))){
                  piece->nextMoves[index][0] = piece->r - 1;
                  piece->nextMoves[index][1] = piece->c + 1;
                  *(attacking + (8 * (piece->r - 1)) + piece->c + 1) = 1;
                  index = index + 1;
                }
            }
            //down left
            if (*(board + (8 * (piece->r - 1)) + piece->c - 1)){
                p = getPiece(piece->r - 1, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;  
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * (piece->r - 1)) + piece->c - 1))){
                  piece->nextMoves[index][0] = piece->r - 1;
                  piece->nextMoves[index][1] = piece->c - 1;
                  *(attacking + (8 * (piece->r - 1)) + piece->c - 1) = 1;
                  index = index + 1;
                }
            }
        } else if (piece->c > 0){
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * piece->r) + piece->c - 1))){
                  piece->nextMoves[index][0] = piece->r;
                  piece->nextMoves[index][1] = piece->c - 1;
                  *(attacking + (8 * (piece->r)) + piece->c - 1) = 1;
                  index = index + 1;
                }
            }
            //up
            if (*(board + (8 * (piece->r + 1)) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * (piece->r + 1)) + piece->c))){
                  piece->nextMoves[index][0] = piece->r + 1;
                  piece->nextMoves[index][1] = piece->c;
                  *(attacking + (8 * (piece->r + 1)) + piece->c) = 1;
                  index = index + 1;
                }
            }
            //down
            if (*(board + (8 * (piece->r - 1)) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 * (piece->r - 1)) + piece->c))){
                  piece->nextMoves[index][0] = piece->r - 1;
                  piece->nextMoves[index][1] = piece->c;
                  *(attacking + (8 * (piece->r - 1)) + piece->c) = 1;
                  index = index + 1;
                }
            }
            //up left
            if (*(board + (8 * (piece->r + 1)) + piece->c - 1)){
                p = getPiece(piece->r + 1, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
                if(!(*(attacked + (8 *(piece->r + 1)) + piece->c - 1))){
                  piece->nextMoves[index][0] = piece->r + 1;
                  piece->nextMoves[index][1] = piece->c - 1;
                  *(attacking + (8 * (piece->r + 1)) + piece->c - 1) = 1;
                  index = index + 1;
                }
            }
            //down left
            if (*(board + (8 * (piece->r - 1)) + piece->c - 1)){
                p = getPiece(piece->r - 1, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * (piece->r - 1)) + piece->c - 1) = 1;
                index = index + 1;
              }
            }
        } else {
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
                *(attacking + (8 * piece->r) + piece->c + 1) = 1;
              }
            }
            //up
            if (*(board + (8 * (piece->r + 1)) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
                *(attacking + (8 * (piece->r + 1)) + piece->c) = 1;
              }
            }
            //down
            if (*(board + (8 * (piece->r - 1)) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                *(attacking + (8 * (piece->r - 1)) + piece->c) = 1;
                index = index + 1;
              }
            }
            //up right
            if (*(board + (8 * (piece->r + 1)) + piece->c + 1)){
                p = getPiece(piece->r + 1, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * (piece->r + 1)) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
            //down right
            if (*(board + (8 * (piece->r - 1)) + piece->c + 1)){
                p = getPiece(piece->r - 1, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * (piece->r - 1)) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
        }
    } else if (piece->r > 0){
        if ((piece-> c > 0) & (piece->c < 7)){
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * piece->r) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * piece->r) + piece->c - 1) = 1;
                index = index + 1;
              }
            }
            //down
            if (*(board + (8 * (piece->r - 1)) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                *(attacking + (8 * (piece->r - 1)) + piece->c) = 1;
                index = index + 1;
              }
            }
            //down right
            if (*(board + (8 * (piece->r - 1)) + piece->c + 1)){
                p = getPiece(piece->r - 1, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * (piece->r - 1)) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
            //down left
            if (*(board + (8 * (piece->r - 1)) + piece->c - 1)){
                p = getPiece(piece->r - 1, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * (piece->r - 1)) + piece->c - 1) = 1;
                index = index + 1;
              }
            }
        } else if (piece->c > 0){
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * piece->r) + piece->c - 1) = 1;
                index = index + 1;
              }
            }
            //down
            if (*(board + (8 * (piece->r - 1)) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                *(attacking + (8 * (piece->r - 1)) + piece->c) = 1;
                index = index + 1;
              }
            }
            //down left
            if (*(board + (8 * (piece->r - 1)) + piece->c - 1)){
                p = getPiece(piece->r - 1, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * (piece->r - 1)) + piece->c - 1) = 1;
                index = index + 1;
              }
            }
        } else {
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * piece->r) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
            //down
            if (*(board + (8 * (piece->r - 1)) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                *(attacking + (8 * (piece->r - 1)) + piece->c) = 1;
                index = index + 1;
              }
            }
            //down right
            if (*(board + (8 * (piece->r - 1)) + piece->c + 1)){
                p = getPiece(piece->r - 1, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r - 1;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r - 1)) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * (piece->r - 1)) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
        }
    } else {
        if ((piece-> c > 0) & (piece->c < 7)){
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * piece->r) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * piece->r) + piece->c - 1) = 1;
                index = index + 1;
              }
            }
            //up
            if (*(board + (8 * (piece->r + 1)) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                *(attacking + (8 * (piece->r + 1)) + piece->c) = 1;
                index = index + 1;
              }
            }
            //up right
            if (*(board + (8 * (piece->r + 1)) + piece->c + 1)){
                p = getPiece(piece->r + 1, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * (piece->r + 1)) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
            //up left
            if (*(board + (8 * (piece->r + 1)) + piece->c - 1)){
                p = getPiece(piece->r + 1, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * (piece->r + 1)) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
        } else if (piece->c > 0){
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * piece->r) + piece->c - 1) = 1;
                index = index + 1;
              }
            }
            //up
            if (*(board + (8 * (piece->r + 1)) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                *(attacking + (8 * (piece->r + 1)) + piece->c) = 1;
                index = index + 1;
              }
            }
            //up left
            if (*(board + (8 * (piece->r + 1)) + piece->c - 1)){
                p = getPiece(piece->r + 1, piece->c - 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c - 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c - 1))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 1;
                *(attacking + (8 * (piece->r + 1)) + piece->c - 1) = 1;
                index = index + 1;
              }
            }
        } else {
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * piece->r) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * piece->r) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
            //up
            if (*(board + (8 * (piece->r + 1)) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c;
                      index = index + 1;
                    }
                } else {
                    p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                *(attacking + (8 * (piece->r + 1)) + piece->c) = 1;
                index = index + 1;
              }
            }
            //up right
            if (*(board + (8 * (piece->r + 1)) + piece->c + 1)){
                p = getPiece(piece->r + 1, piece->c + 1, pieces);
                if (piece->type.dir != p->type.dir){
                    if (!p->prot){
                      piece->nextMoves[index][0] = piece->r + 1;
                      piece->nextMoves[index][1] = piece->c + 1;
                      index = index + 1;
                    }
                } else {
                  p->prot = 1;
                }
            } else {
              if(!(*(attacked + (8 * (piece->r + 1)) + piece->c + 1))){
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 1;
                *(attacking + (8 * (piece->r + 1)) + piece->c + 1) = 1;
                index = index + 1;
              }
            }
        }
    }
}

//top level calculate next moves fuction
void calcNextMoves(int num, bool* board, bool* attacked, bool* attacking, struct piece* pieces){
    emptyNextMoves(pieces + num);
    switch((pieces + num)->type.num){
        case 0: calcPawnMoves(pieces + num, board, attacked, pieces);
                break;
        case 1: calcBishopMoves(pieces + num, board, attacked, pieces);
                break;
        case 2: calcKnightMoves(pieces + num, board, attacked, pieces);
                break;
        case 3: calcRookMoves(pieces + num, board, attacked, pieces);
                break;
        case 4: calcQueenMoves(pieces + num, board, attacked, pieces);
                break;
        case 5: calcKingMoves(pieces + num, board, attacking, attacked, pieces);
                break;
        default: break;
    }
}