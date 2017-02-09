#include "Turn.h"

Turn* createTurn(){
    Turn* result;
    result = (Turn*)malloc(sizeof(Turn));
    return result;
}

Turn* createTurnFromXY(short x, short y){
    Turn* result;
    result = createTurn();
    result->x = x;
    result->y = y;
    return result;
}

Turn* createTurnFromTurn(Turn* t){
    Turn* result;
    result = createTurn();
    result->x = t->x;
    result->y = t->x;
    result->TR = t->TR;
    result->T = t->T;
    result->TL = t->TL;
    result->L = t->L;
    result->BL = t->BL;
    result->B = t->B;
    result->BR = t->BR;
    result->R = t->R;
    return result;
}

void destroyTurn(Turn* t){
    free(t);
}
