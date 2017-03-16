#ifndef TURN_H_
#define TURN_H_

typedef unsigned long long uint64_t;

typedef struct tagTurn{
    uint64_t board;
    short x, y, color;
} Turn;

Turn* createTurn(short x, short y, uint64_t board, short color);
Turn* createTurnFromTurn(Turn* t);
void destroyTurn(Turn* t);

#endif // TURN_H_
