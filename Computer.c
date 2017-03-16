#include "Computer.h"
#include <stdlib.h>
#include <stdio.h>

const int posTable[8][8] = {
{5, 4, 3, 2, 2, 3, 4, 5},
{4, -3, 0, 0, 0, 0, -3, 4},
{3, 0, 0, 0, 0, 0, 0, 3},
{2, 0, 0, 0, 0, 0, 0, 2},
{2, 0, 0, 0, 0, 0, 0, 2},
{3, 0, 0, 0, 0, 0, 0, 3},
{4, -3, 0, 0, 0, 0, -3, 4},
{5, 4, 3, 2, 2, 3, 4, 5}
};

Computer* createComputer(Board* b, short player){
    Computer* result;
    result = (Computer*)malloc(sizeof(Computer));
    result->player = player;
    result->board = createBoardFromBoard(b);
    return result;
}

void updateTree(Computer* computer, Turn* t, short player){
    short otherTurn;
    flipCaptured(computer->board, t, player);
    if(player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    fillOpenMoves(computer->board, otherTurn);
}

Turn* makeMove(Computer* c){
    float alpha, beta;
    Turn* max;
    MoveNode* itr;
    Board* b;
    Turn* t;
    float maxh, heuristic;
    short otherTurn;
    maxh = -1;
    alpha = -1;
    beta = 1;
    if(c->player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    max = c->board->openMoves->head->turn;
    itr = c->board->openMoves->head;
    while(itr->next != NULL_PTR && alpha < beta){
        printf("decision made with  depth %i\n", 3);
        b = createBoardFromBoard(c->board);
        t = itr->turn;
        flipCaptured(b, t, c->player);
        fillOpenMoves(b, otherTurn);
        //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
        heuristic = processNode(4, otherTurn, b, c, alpha, beta);
        if(heuristic > maxh){
            maxh = heuristic; max = t;
            alpha = heuristic;
        }
        destroyBoard(b);
        itr = itr->next;
    }
    return max;

}

float processNode(int levelsLeft, int player, Board* board, Computer* callback, float alpha, float beta){
    MoveNode* itr;
    Board* b;
    Turn* t;
    float maxh, minh, heuristic;
    short otherTurn;
    maxh = -1; minh = 1;
    if(callback->player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    itr = board->openMoves->head;

    if(isMoveListEmpty(board->openMoves)){
        if(mostPieces(board) == callback->player) return 1;
        if(mostPieces(board) == EMPTY) return 0;
        else return -1;
    }
    if(levelsLeft == 0){
        return getHeuristic(callback, board);
    } else {
        while(itr->next != NULL_PTR){
            if( alpha < beta){
                    printf("alpha: %f, beta: %f", alpha, beta);
                b = createBoardFromBoard(board);
                t = itr->turn;
                flipCaptured(b, t, callback->player);
                fillOpenMoves(b, otherTurn);
                //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
                heuristic = processNode(levelsLeft-1, otherTurn, b, callback, alpha, beta);
                if(heuristic > maxh){
                    maxh = heuristic;
                    if(player == callback->player){
                        alpha = heuristic;
                    }
                }
                if(heuristic < minh){
                    minh = heuristic;
                    if(player != callback->player){
                        beta = heuristic;
                    }
                }
                destroyBoard(b);
                itr = itr->next;
            }else {
                printf("pruned tree \n");
                itr = itr->next;
            }
        }
        if(player == callback->player){
            return maxh;
        } else {
            return minh;
        }
    }
}


float getHeuristic(Computer* c, Board* b){
/*    bs.fillOpenMoves(player);
    float ourmovecount = bs.getOpenMoves().size();
    bs.fillOpenMoves(player == 1 ? 2 : 1);
    float oppmovecount = bs.getOpenMoves().size();
    float divisaor = bs.allAdjacentTiles();
    float result = (ourmovecount - oppmovecount)/divisaor;
    //System.out.printf("heuristic formula calculated (%f - %f)/%f counter at %d result was %f", ourmovecount, oppmovecount, divisaor, counter++, result);
    return result;
*/
        int ourMoveCount, oppmovecount, divisor;
        short otherTurn;
        float result;
    if(countPieces(b, EMPTY) < 10){
        ourMoveCount = countPieces(b, c->player);
        if(c->player == BLACK){
            otherTurn = WHITE;
        } else {
            otherTurn = BLACK;
        }
        oppmovecount = countPieces(b, otherTurn);
        return 0;

        //this is end game
    } else {

        fillOpenMoves(b, c->player);
        ourMoveCount = getMoveListSize(b->openMoves);
        if(c->player == BLACK){
            otherTurn = WHITE;
        } else {
            otherTurn = BLACK;
        }
        fillOpenMoves(b, otherTurn);
        oppmovecount = getMoveListSize(b->openMoves);
        divisor = allAdjacentTiles(b);
        result = ((((float)ourMoveCount) - ((float)oppmovecount)) / ((float)divisor));
        return result;
    }

}

void destroyComputer(Computer* c){
    destroyBoard(c->board);
    free(c);
}
