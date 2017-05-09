#ifndef COMPUTER_H_
#define COMPUTER_H_
#include "Board.h"
#include "Turn.h"
#include "MiniMax.h"

typedef struct tagComputer{
    int positional, countt, longstable, shortstable;
    short player;
    MinimaxNode* root;
} Computer;



Computer* createComputer(Board* b, short compplayer, short activeboardplayer, int positional, int countt, int longstable, int shortstable);
void updateTree(Computer* computer, Board* newb, short player);
Turn* makeMove(Computer* c);
void fillNode(MinimaxNode* mn, Computer* callback);
float processNode(int levelsLeft, MinimaxNode* node, Computer* callback, float alpha, float beta, short deadMoves);
float getHeuristic(Computer* c, Board* b);
void destroyComputer(Computer* c);

#endif // COMPUTER_H_
