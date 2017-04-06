
#include "MiniMax.h"
#include <stdlib.h>

void destroyMiniMax(MinimaxNode* mn){
    int count;
    destroyBoard(mn->board);
    if(mn->childrenProcessed != 0){
        for(count = 0; count < mn->noChildren; count++){
            destroyMiniMax(mn->children + count);
        }
    }
    free(mn->children);
}


void shallowDestroy(MinimaxNode* mn){
    free(mn);
}
