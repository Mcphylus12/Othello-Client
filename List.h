#ifndef LIST_H_
#define LIST_H_
#include "Turn.h"
#define NULL_PTR ((void*)0)
typedef struct tagMoveNode{
    struct tagMoveNode* next;
    Turn* turn;
} MoveNode;

MoveNode* createMoveNode();
MoveNode* destroyMoveNodeandGetNext(MoveNode* mn);


//linked lit nodes for a list of turns
typedef struct tagMoveList{
    MoveNode* head;
} MoveList;

MoveList* createMoveList();
MoveList* createMoveListFromList(MoveList* ml);
void clearList(MoveList* movelist);
void addTurnToMoveList(MoveList* ml, Turn* t);
short isMoveListEmpty(MoveList* ml);
int getMoveListSize(MoveList* ml);
void destroyMoveList(MoveList* ml);
Turn* getTurn(MoveList* ml, int number);





#endif // LIST_H_
