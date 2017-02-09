#ifndef TURN_H_
#define TURN_H_

typedef struct tagTurn{
    short x, y, TR, T, TL, L, BL, B, BR, R;
} Turn;

Turn* createTurn();
Turn* createTurnFromXY(short x, short y);
Turn* createTurnFromTurn(Turn* t);
void destroyTurn(Turn* t);

#endif // TURN_H_
