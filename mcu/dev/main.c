#include "chess.h"

int main(){
    //setup
    bool board[64];
    bool lights[64];
    struct piece pieces[32];
    initGame(&pieces, &board);

    bool row[8];
    int r;
    int lastUp = 0;
    int newRow = 0;
    int check;

    struct boardChange update, lastUpdate;

    while(1){
        if (newRow){
            checkChange(&row, r, &board, &pieces, &update, &lastUpdate);
            if (lastUp){
                if (update.up == 1){
                    //piece in update is captured
                    update.piece->r = 99;
                    update.piece->c = 99;
                    update.piece = lastUpdate.piece;//saving piece doing capture
                } else if (update.up == 0){
                    clearLights(&lights);//stop lights
                    lastUp = 0;
                    *(board + (8 * update.piece->r) + update.piece->c) = 1;
                    //check promotion
                    if ((update.piece->type.num == 0) & 
                    (((update.piece->r == 7) & (update.piece->type.dir == 1)) | ((update.piece->r == 0) & (update.piece->type.dir == -1)))){
                        update.piece->type.num = 4;
                    }
                    calcNextMoves(update.piece, &board, &pieces);
                    //check check/mate
                    check = checkCheck(&pieces, update.piece->type.dir);
                    //if (check/mate) lightCheck
                    if (check){
                        //light check
                    }
                }
            } else {
                if (update.up == 1){
                    lastUp = 1;
                    *(board + (8 * update.piece->r) + update.piece->c) = 0;
                    updateLightArray(&update.piece, &lights);//lightMoves(piece)
                    lightLights(&lights);
                }
            }
            lastUpdate = update;
        }
    }
    return 1;
}