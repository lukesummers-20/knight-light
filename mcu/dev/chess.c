/*
set up arrays

loop:
wait for row
update with row:
    pickup: lights, save pickup pos
    putdown: calc next moves, update board(include check), stop lights, sound
*/



#include "chess.h"



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
}

struct boardChange checkChange(bool* row, int r, bool* board, struct piece* pieces){
    struct boardChange move = initBoardChange();
    for (int i = 0; i < 8; i++){
        if(*(row + i) != *(board + (8 * r) + i)){
            move.up = (!(*(row + i)));
            move.piece = getPiece(r, i, pieces);
            return move;
        } 
    }
    return move;
}

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

