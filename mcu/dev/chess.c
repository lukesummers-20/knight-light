/*
set up arrays

loop:
wait for row
update with row:
    pickup: lights, save pickup pos
    putdown: calc next moves, update board(include check), stop lights, sound
*/



#include "chess.h"

int checkCheck(struct piece* pieces, int dir){
    struct piece king, p;
    if (dir == 1){
        king = *(pieces + WKi);
        for (int i = 16; i < 32; i++){
            int j = 0;
            p = *(pieces + i);
            while ((j < 28) & (p.nextMoves[j][0] != -1)){
                if ((p.nextMoves[j][0] == king.r) & (p.nextMoves[j][1] == king.c)){
                    return 1;
                }
                j++;
            }
        }
    } else if (dir == -1){
        king = *(pieces + BKi);
        for (int i = 0; i < 16; i++){
            int j = 0;
            p = *(pieces + i);
            while ((j < 28) & (p.nextMoves[j][0] != -1)){
                if ((p.nextMoves[j][0] == king.r) & (p.nextMoves[j][1] == king.c)){
                    return 1;
                }
                j++;
            }
        }
    }
    return 0;
}

void initGame(struct piece* pieces, bool* board){
    initPieces(pieces);
    initBoard(board);
}

struct boardChange initBoardChange(){
    struct boardChange move;
    move.piece.type.num = 99;
    move.piece.type.dir = 99;
    move.piece.r = 99;
    move.piece.c = 99;
    move.up = 99;
    return move;
}

struct boardChange checkChange(bool* row, int r, bool* board, struct piece* pieces, struct boardChange* lastUpdate){
    struct boardChange move = initBoardChange();
    for (int i = 0; i < 8; i++){
        if(*(row + i) != *(board + (8 * r) + i)){
            move.up = (!(*(row + i)));
            if (move.up){
                //cannot return structs, only ptr to struct
                move.piece = getPiece(r, i, pieces);
            } else {
                move.piece = lastUpdate->piece;
                move.piece.r = r;
                move.piece.c = i;
            }
            return move;
        } 
    }
    return move;
}

void lightMoves(struct piece* piece, bool* lights){
    int i = 0;
    while ((i < 28) & piece->nextMoves[i][0] != -1){
        *(lights + (8 * piece->nextMoves[i][0]) + piece->nextMoves[i][1]) = 1;
    }
}

void clearLights(bool* lights){
    for(int i = 0; i < 64; i++){
        *(lights + i) = 0;
    }
}

/*
void lightMoves(int piece, int r, int c, int* board, int* lights, int dir){
    int i, j;
    switch(piece) {
        case 0: //pawn
                *(lights + (8 * (r + dir) + c)) = 1;
                if (c > 0){
                    if (c < 7){
                        *(lights + (8 * (r + dir)) + c + 1) = *(board + (8 * r + dir) + c + 1);
                        *(lights + (8 * (r + dir)) + c - 1) = *(board + (8 * r + dir) + c - 1);
                    } else {
                        *(lights + (8 * (r + dir)) + c - 1) = *(board + (8 * r + dir) + c - 1);
                    }
                } else {
                    *(lights + (8 * (r + dir)) + c + 1) = *(board + (8 * (r + dir)) + c + 1);
                }
                break;
        case 1: //bishop
                i = r + 1;
                j = c + 1;
                while ((i < 7) & (j < 7)){
                    *(lights + (8 * i) + j) = 1;
                    i = i + 1;
                    j = j + 1;
                }
                i = r + 1;
                j = c - 1;
                while ((i < 7) & (j > 0)){
                    *(lights + (8 * i) + j) = 1;
                    i = i + 1;
                    j = j - 1;
                }
                i = r - 1;
                j = c + 1;
                while ((i > 0) & (j < 7)){
                    *(lights + (8 * i) + j) = 1;
                    i = i - 1;
                    j = j + 1;
                }
                i = r - 1;
                j = c - 1;
                while ((i > 0) & (j > 0)){
                    *(lights + (8 * i) + j) = 1;
                    i = i - 1;
                    j = j - 1;
                }
                break;
        case 2: //knight
                if (r == 0){
                    if (c == 0){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                    } else if (c == 1){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                    } else if (c == 6){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                    } else if (c == 7){
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                    } else {
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                    }
                } else if (r == 1){
                    if (c == 0){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                    } else if (c == 1){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                    } else if (c == 6){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    } else if (c == 7){
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    } else {
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    }
                } else if (r == 6){
                    if (c == 0){
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                    } else if (c == 1){
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                    } else if (c == 6){
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    } else if (c == 7){
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    } else {
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    }
                } else if (r == 7){
                    if (c == 0){
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                    } else if (c == 1){
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                    } else if (c == 6){
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    } else if (c == 7){
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    } else {
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    }
                } else {
                    if (c == 0){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                    } else if (c == 1){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                    } else if (c == 6){
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    } else if (c == 7){
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    } else {
                        *(lights + (8 * (r + 2)) + c + 1) = 1;
                        *(lights + (8 * (r + 2)) + c - 1) = 1;
                        *(lights + (8 * (r - 2)) + c + 1) = 1;
                        *(lights + (8 * (r - 2)) + c - 1) = 1;
                        *(lights + (8 * (r + 1)) + c + 2) = 1;
                        *(lights + (8 * (r - 1)) + c + 2) = 1;
                        *(lights + (8 * (r + 1)) + c - 2) = 1;
                        *(lights + (8 * (r - 1)) + c - 2) = 1;
                    }
                }
                break;
        case 3: //rook
                i = r + 1;
                j = c;
                while (i < 7){
                    *(lights + (8 * i) + j) = 1;
                    i = i + 1;
                }
                i = r - 1;
                while (i > 0) {
                    *(lights + (8 * i) + j) = 1;
                    i = i - 1;
                }
                i = r;
                j = c + 1;
                while (j < 7) {
                    *(lights + (8 * i) + j) = 1;
                    j = j + 1;
                }
                j = c - 1;
                while (j > 0) {
                    *(lights + (8 * i) + j) = 1;
                    j = j - 1;
                }
                break;
        case 4: //queen
                i = r + 1;
                j = c + 1;
                while ((i < 7) & (j < 7)){
                    *(lights + (8 * i) + j) = 1;
                    i = i + 1;
                    j = j + 1;
                }
                i = r + 1;
                j = c - 1;
                while ((i < 7) & (j > 0)){
                    *(lights + (8 * i) + j) = 1;
                    i = i + 1;
                    j = j - 1;
                }
                i = r - 1;
                j = c + 1;
                while ((i > 0) & (j < 7)){
                    *(lights + (8 * i) + j) = 1;
                    i = i - 1;
                    j = j + 1;
                }
                i = r - 1;
                j = c - 1;
                while ((i > 0) & (j > 0)){
                    *(lights + (8 * i) + j) = 1;
                    i = i - 1;
                    j = j - 1;
                }
                i = r + 1;
                j = c;
                while (i < 7){
                    *(lights + (8 * i) + j) = 1;
                    i = i + 1;
                }
                i = r - 1;
                while (i > 0) {
                    *(lights + (8 * i) + j) = 1;
                    i = i - 1;
                }
                i = r;
                j = c + 1;
                while (j < 7) {
                    *(lights + (8 * i) + j) = 1;
                    j = j + 1;
                }
                j = c - 1;
                while (j > 0) {
                    *(lights + (8 * i) + j) = 1;
                    j = j - 1;
                }
                break;
        case 5: //king
                if (r == 0){
                    if (c == 7){
                        *(lights + 8 * r + c - 1) = 1;
                        *(lights + 8 * (r + 1) + c) = 1;
                        *(lights + 8 * (r + 1) + c - 1) = 1;
                    } else if (c  == 0){
                        *(lights + 8 * r + c + 1) = 1;
                        *(lights + 8 * (r + 1) + c) = 1;
                        *(lights + 8 * (r + 1) + c + 1) = 1;
                    } else {
                        *(lights + 8 * r + c + 1) = 1;
                        *(lights + 8 * r + c - 1) = 1;
                        *(lights + 8 * (r + 1) + c) = 1;
                        *(lights + 8 * (r + 1) + c - 1) = 1;
                        *(lights + 8 * (r + 1) + c + 1) = 1;
                    }
                } else if (r == 7){
                    if (c == 7){
                        *(lights + 8 * r + c - 1) = 1;
                        *(lights + 8 * (r - 1) + c) = 1;
                        *(lights + 8 * (r - 1) + c - 1) = 1;
                    } else if (c  == 0){
                        *(lights + 8 * r + c + 1) = 1;
                        *(lights + 8 * (r - 1) + c) = 1;
                        *(lights + 8 * (r - 1) + c + 1) = 1;
                    } else {
                        *(lights + 8 * r + c + 1) = 1;
                        *(lights + 8 * r + c - 1) = 1;
                        *(lights + 8 * (r - 1) + c) = 1;
                        *(lights + 8 * (r - 1) + c - 1) = 1;
                        *(lights + 8 * (r - 1) + c + 1) = 1;
                    }
                } else {
                    if (c == 7){
                        *(lights + 8 * r + c - 1) = 1;
                        *(lights + 8 * (r + 1) + c) = 1;
                        *(lights + 8 * (r + 1) + c - 1) = 1;
                        *(lights + 8 * (r - 1) + c) = 1;
                        *(lights + 8 * (r - 1) + c - 1) = 1;
                    } else if (c  == 0){
                        *(lights + 8 * r + c + 1) = 1;
                        *(lights + 8 * (r + 1) + c) = 1;
                        *(lights + 8 * (r + 1) + c + 1) = 1;
                        *(lights + 8 * (r - 1) + c) = 1;
                        *(lights + 8 * (r - 1) + c + 1) = 1;
                    } else {
                        *(lights + 8 * r + c + 1) = 1;
                        *(lights + 8 * r + c - 1) = 1;
                        *(lights + 8 * (r + 1) + c) = 1;
                        *(lights + 8 * (r + 1) + c - 1) = 1;
                        *(lights + 8 * (r + 1) + c + 1) = 1;
                        *(lights + 8 * (r - 1) + c) = 1;
                        *(lights + 8 * (r - 1) + c - 1) = 1;
                        *(lights + 8 * (r - 1) + c + 1) = 1;
                    }
                }
                break;
    }
}
*/
