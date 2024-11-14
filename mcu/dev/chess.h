
#ifndef CHESS_H
#define CHESS_h

struct pieceType{
    int type;
    int dir;
};

struct piece{
    struct pieceType piece;
    int nextMoves[28][2];
    int r;
    int c;
};
#endif