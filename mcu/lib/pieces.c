#include "pieces.h"
#include "chess.h"

int checkPromotion(struct piece* piece){
    if (piece->type.dir == 1){
        if (piece->r == 7){
            piece->type.num = 4;
            return 1;
        }
    } else if (piece->type.dir == -1){
        if (piece->r == 0){
            piece->type.num = 4;
            return 1;
        }
    }
    return 0;
}

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

struct piece initBishop(int r, int c, int dir){
    struct piece bishop;
    bishop.type.num = 1;
    bishop.type.dir = dir;
    emptyNextMoves(&bishop);
    bishop.r = r;
    bishop.c = c;
    return bishop;
}

struct piece initKnight(int r, int c, int dir){
    struct piece knight;
    knight.type.num = 2;
    knight.type.dir = dir;
    emptyNextMoves(&knight);
    knight.nextMoves[0][0] = 2 * dir + r;
    knight.nextMoves[0][1] = c + 1;
    knight.nextMoves[1][0] = 2 * dir + r;
    knight.nextMoves[1][1] = c - 1;
    knight.r = r;
    knight.c = c;
    return knight;
}

struct piece initRook(int r, int c, int dir){
    struct piece rook;
    rook.type.num = 3;
    rook.type.dir = dir;
    emptyNextMoves(&rook);
    rook.r = r;
    rook.c = c;
    return rook;
}

struct piece initQueen(int r, int c, int dir){
    struct piece queen;
    queen.type.num = 4;
    queen.type.dir = dir;
    emptyNextMoves(&queen);
    queen.r = r;
    queen.c = c;
    return queen;
}

struct piece initKing(int r, int c, int dir){
    struct piece king;
    king.type.num = 5;
    king.type.dir = dir;
    emptyNextMoves(&king);
    king.r = r;
    king.c = c;
    return king;
}

void initPieces(struct piece* pieces){
    //white
    *(pieces + WR1) = initRook(0, 0, 1);
    *(pieces + WKn1) = initKnight(0, 1, 1);
    *(pieces + WB1) = initBishop(0, 2, 1);
    *(pieces + WQ) = initQueen(0, 3, 1);
    *(pieces + WKi) = initKing(0, 4, 1);
    *(pieces + WB2) = initBishop(0, 5, 1);
    *(pieces + WKn2) = initKnight(0, 6, 1);
    *(pieces + WR2) = initRook(0, 7, 1);
    for (int i = 0; i < 8; i++){
        *(pieces + 8 + i) = initPawn(1, i, 1);
    }
    //black
    *(pieces + BR1) = initRook(7, 0, -1);
    *(pieces + BKn1) = initKnight(7, 1, -1);
    *(pieces + BB1) = initBishop(7, 2, -1);
    *(pieces + BQ) = initQueen(7, 3, -1);
    *(pieces + BKi) = initKing(7, 4, -1);
    *(pieces + BB2) = initBishop(7, 5, -1);
    *(pieces + BKn2) = initKnight(7, 6, -1);
    *(pieces + BR2) = initRook(7, 7, -1);
    for (int i = 0; i < 8; i++){
        *(pieces + 24 + i) = initPawn(6, i, -1);
    }
}

void updatePiece(struct piece* piece, int r, int c){
    piece->r = r;
    piece->c = c;
    emptyNextMoves(piece);

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
        case 2: calcKnightMoves(piece, board, pieces);
        case 3: calcRookMoves(piece, board, pieces);
        case 4: calcQueenMoves(piece, board, pieces);
        case 5: calcKingMoves(piece, board, pieces);
    }
}

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
    if (*(board + (8 * i) + j)& ((i < 8) & (j < 8))){
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
    if (*(board + (8 * i) + j) & ((i < 8) & (j > -1))){
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
    if (*(board + (8 * i) + j) & ((i > -1) & (j > -1))){
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
        j = j + 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j) & ((i > -1) & (j < 8))){
        p = getPiece(i , j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
}

void calcKnightMoves(struct piece* piece, bool* board, struct piece* pieces){
    struct piece p;
    int index = 0;
    if (piece->r == 0){
        if (piece->c == 0){
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 1) {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 6) {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else if (piece->c == 7){
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        }
    } else if (piece->r == 1){
        if (piece->c == 0){
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 1) {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 6) {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else if (piece->c == 7){
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        }
    } else if (piece->r == 6) {
        if (piece->c == 0){
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 1) {
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 6) {
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else if (piece->c == 7){
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else {
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        }
    } else if (piece->r == 7){
        if (piece->c == 0){
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 1) {
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 6) {
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else if (piece->c == 7){
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else {
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        }
    } else {
        if (piece->c == 0){
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 1) {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
        } else if (piece->c == 6) {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else if (piece->c == 7){
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        } else {
            //up 2 right
            if (*(board + (8 * piece->r + 2) + piece->c + 1)){
                p = getPiece(piece->r + 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up 2 left
            if (*(board + (8 * piece->r + 2) + piece->c - 1)){
                p = getPiece(piece->r + 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up right 2
            if (*(board + (8 * piece->r + 1) + piece->c + 2)){
                p = getPiece(piece->r + 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //up left 2
            if (*(board + (8 * piece->r + 1) + piece->c - 2)){
                p = getPiece(piece->r + 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
            //down 2 right
            if (*(board + (8 * piece->r - 2) + piece->c + 1)){
                p = getPiece(piece->r - 2, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down 2 left
            if (*(board + (8 * piece->r - 2) + piece->c - 1)){
                p = getPiece(piece->r - 2, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 2;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 2;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down right 2
            if (*(board + (8 * piece->r - 1) + piece->c + 2)){
                p = getPiece(piece->r - 1, piece->c + 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 2;
                index = index + 1;
            }
            //down left 2
            if (*(board + (8 * piece->r - 1) + piece->c - 2)){
                p = getPiece(piece->r - 1, piece->c - 2, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 2;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 2;
                index = index + 1;
            }
        }
    }
}

void calcRookMoves(struct piece* piece, bool* board, struct piece* pieces){
    int i, j;
    struct piece p;
    int index = 0;
    i = piece->r + 1;
    j = piece->c;
    while ((i < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        i = i + 1;
        index = index + 1;
    }
    if ((i < 8) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r - 1;
    j = piece->c;
    while ((i > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        i = i - 1;
        index = index + 1;
    }
    if ((i > -1) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r;
    j = piece->c + 1;
    while ((j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        j = j + 1;
        index = index + 1;
    }
    if ((j < 8) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r;
    j = piece->c - 1;
    while ((j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        j = j - 1;
        index = index + 1;
    }
    if ((j > -1) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
}

void calcQueenMoves(struct piece* piece, bool* board, struct piece* pieces){
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
    if (*(board + (8 * i) + j)& ((i < 8) & (j < 8))){
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
    if (*(board + (8 * i) + j) & ((i < 8) & (j > -1))){
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
    if (*(board + (8 * i) + j) & ((i > -1) & (j > -1))){
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
        j = j + 1;
        index = index + 1;
    }
    if (*(board + (8 * i) + j) & ((i > -1) & (j < 8))){
        p = getPiece(i , j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r + 1;
    j = piece->c;
    while ((i < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        i = i + 1;
        index = index + 1;
    }
    if ((i < 8) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r - 1;
    j = piece->c;
    while ((i > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        i = i - 1;
        index = index + 1;
    }
    if ((i > -1) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r;
    j = piece->c + 1;
    while ((j < 8) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        j = j + 1;
        index = index + 1;
    }
    if ((j < 8) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
    i = piece->r;
    j = piece->c - 1;
    while ((j > -1) & (!(*(board + (8 * i) + j)))){
        piece->nextMoves[index][0] = i;
        piece->nextMoves[index][1] = j;
        j = j - 1;
        index = index + 1;
    }
    if ((j > -1) & (*(board + (8 * i) + j))){
        p = getPiece(i, j, pieces);
        if (p.type.dir != piece->type.dir){
            piece->nextMoves[index][0] = i;
            piece->nextMoves[index][1] = j;
            index = index + 1;
        }
    }
}

void calcKingMoves(struct piece* piece, bool* board, struct piece* pieces){
    struct piece p;
    int index = 0;
    if ((piece->r > 0) & (piece->r < 7)){
        if ((piece-> c > 0) & (piece->c < 7)){
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up
            if (*(board + (8 * piece->r + 1) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //down
            if (*(board + (8 * piece->r - 1) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //up right
            if (*(board + (8 * piece->r + 1) + piece->c + 1)){
                p = getPiece(piece->r + 1, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up left
            if (*(board + (8 * piece->r + 1) + piece->c - 1)){
                p = getPiece(piece->r + 1, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down right
            if (*(board + (8 * piece->r - 1) + piece->c + 1)){
                p = getPiece(piece->r - 1, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down left
            if (*(board + (8 * piece->r - 1) + piece->c - 1)){
                p = getPiece(piece->r - 1, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
        } else if (piece->c > 0){
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up
            if (*(board + (8 * piece->r + 1) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //down
            if (*(board + (8 * piece->r - 1) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //up left
            if (*(board + (8 * piece->r + 1) + piece->c - 1)){
                p = getPiece(piece->r + 1, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down left
            if (*(board + (8 * piece->r - 1) + piece->c - 1)){
                p = getPiece(piece->r - 1, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
        } else {
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up
            if (*(board + (8 * piece->r + 1) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //down
            if (*(board + (8 * piece->r - 1) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //up right
            if (*(board + (8 * piece->r + 1) + piece->c + 1)){
                p = getPiece(piece->r + 1, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down right
            if (*(board + (8 * piece->r - 1) + piece->c + 1)){
                p = getPiece(piece->r - 1, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
        }
    } else if (piece->r > 0){
        if ((piece-> c > 0) & (piece->c < 7)){
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down
            if (*(board + (8 * piece->r - 1) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //down right
            if (*(board + (8 * piece->r - 1) + piece->c + 1)){
                p = getPiece(piece->r - 1, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down left
            if (*(board + (8 * piece->r - 1) + piece->c - 1)){
                p = getPiece(piece->r - 1, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
        } else if (piece->c > 0){
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //down
            if (*(board + (8 * piece->r - 1) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //down left
            if (*(board + (8 * piece->r - 1) + piece->c - 1)){
                p = getPiece(piece->r - 1, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
        } else {
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //down
            if (*(board + (8 * piece->r - 1) + piece->c)){
                p = getPiece(piece->r - 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //down right
            if (*(board + (8 * piece->r - 1) + piece->c + 1)){
                p = getPiece(piece->r - 1, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r - 1;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r - 1;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
        }
    } else {
        if ((piece-> c > 0) & (piece->c < 7)){
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up
            if (*(board + (8 * piece->r + 1) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //up right
            if (*(board + (8 * piece->r + 1) + piece->c + 1)){
                p = getPiece(piece->r + 1, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up left
            if (*(board + (8 * piece->r + 1) + piece->c - 1)){
                p = getPiece(piece->r + 1, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
        } else if (piece->c > 0){
            //left
            if (*(board + (8 * piece->r) + piece->c - 1)){
                p = getPiece(piece->r, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
            //up
            if (*(board + (8 * piece->r + 1) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //up left
            if (*(board + (8 * piece->r + 1) + piece->c - 1)){
                p = getPiece(piece->r + 1, piece->c - 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c - 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c - 1;
                index = index + 1;
            }
        } else {
            //right
            if (*(board + (8 * piece->r) + piece->c + 1)){
                p = getPiece(piece->r, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
            //up
            if (*(board + (8 * piece->r + 1) + piece->c)){
                p = getPiece(piece->r + 1, piece->c, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c;
                index = index + 1;
            }
            //up right
            if (*(board + (8 * piece->r + 1) + piece->c + 1)){
                p = getPiece(piece->r + 1, piece->c + 1, pieces);
                if (piece->type.dir != p.type.dir){
                    piece->nextMoves[index][0] = piece->r + 1;
                    piece->nextMoves[index][1] = piece->c + 1;
                    index = index + 1;
                }
            } else {
                piece->nextMoves[index][0] = piece->r + 1;
                piece->nextMoves[index][1] = piece->c + 1;
                index = index + 1;
            }
        }
    }
}