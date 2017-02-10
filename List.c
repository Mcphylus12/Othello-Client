#include "List.h"
#include <stdlib.h>

MoveNode* createMoveNode(){
    MoveNode* result;
    result = (MoveNode*)malloc(sizeof(MoveNode));
    return result;
}

MoveNode* destroyMoveNodeandGetNext(MoveNode* mn){
    MoveNode* next;
    next = mn->next;
    destroyTurn(mn->turn);
    free(mn);
    return next;
}

MoveList* createMoveList(){
    MoveList* result;
    result = (MoveList*)malloc(sizeof(MoveList));
    result->head = NULL_PTR;
    return result;
}

MoveList* createMoveListFromList(MoveList* ml){
    MoveNode* itr1;
    MoveList* result;
    result = (MoveList*)malloc(sizeof(MoveList));
    if(ml->head != NULL_PTR){
        result->head = createMoveNode();
        result->head->next = NULL_PTR;
        result->head->turn = createTurnFromTurn(ml->head->turn);
        itr1 = ml->head->next;
        while(itr1 != NULL_PTR){
            addTurnToMoveList(result, createTurnFromTurn(itr1->turn));

            itr1 = itr1->next;
        }
    } else {
        result->head = NULL_PTR;
    }
    return result;
}

void clearList(MoveList* movelist){
    MoveNode* itr;
    itr = movelist->head;
    while(itr != NULL_PTR){

        itr = destroyMoveNodeandGetNext(itr);
    }
    movelist->head = NULL_PTR;

}

void addTurnToMoveList(MoveList* ml, Turn* t){
    MoveNode* itr;
    if(ml->head == NULL_PTR){
        ml->head = createMoveNode();
        ml->head->next = NULL_PTR;
        ml->head->turn = t;
    } else {
        itr = ml->head;
        while(itr->next != NULL_PTR){
            itr = itr->next;
        }
        itr->next = createMoveNode();
        itr = itr->next;
        itr->next = NULL_PTR;
        itr->turn = t;
    }
}

short isMoveListEmpty(MoveList* ml){
    if(ml->head == NULL_PTR){
        return 1;
    } else {
        return 0;
    }
}

int getMoveListSize(MoveList* ml){
    int size;
    MoveNode* itr;
    size = 1;
    if(ml->head == NULL_PTR){
        return 0;
    } else {
        itr = ml->head;
        while(itr->next != NULL_PTR){
            size++;
            itr = itr->next;
        }
        return size;
    }
}

void destroyMoveList(MoveList* ml){
    clearList(ml);
    free(ml);
}

