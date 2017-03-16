#include "Turn.h"

Turn* createTurn(short x, short y, uint64_t board, short color){
    Turn* result;
    result = (Turn*)malloc(sizeof(Turn));
    result->x = x;
    result->y = y;
    result->board = board;
    result->color = color;
    return result;
}

Turn* createTurnFromTurn(Turn* t){
    Turn* result;
    result = (Turn*)malloc(sizeof(Turn));
    result->x = t->x;
    result->y = t->y;
    result->board = t->board;
    result->color = t->color;
    return result;
}

void destroyTurn(Turn* t){
    free(t);
}
