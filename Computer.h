#ifndef COMPUTER_H_
#define COMPUTER_H_
#include "Board.h"
#include "Turn.h"

typedef struct tagComputer{
    Board* board;
    short player;
} Computer;

Computer* createComputer(Board* b, short player);
void updateTree(Computer* computer, Turn* t, short player);
Turn* makeMove(Computer* c);
float processNode(int levelsLeft, int player, Board* board, Computer* callback);
float getHeuristic(Computer* c, Board* b);
void destroyComputer(Computer* c);

#endif // COMPUTER_H_
