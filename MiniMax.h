#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "Board.h"

typedef struct tagMinimaxNode{
    struct tagMinimaxNode* children;
    Board* board;
    float heuristic;
    int noChildren;
    char childrenProcessed;
    short player;
} MinimaxNode;

void printTree(MinimaxNode* mn);
void destroyMiniMax(MinimaxNode* mn);
void destroyMiniMaxNoFree(MinimaxNode* mn);
void shallowDestroy(MinimaxNode* mn);
#endif // MINIMAX_H_
