#include "Computer.h"
#include <stdlib.h>

Computer* createComputer(Board* b, short player){
    Computer* result;
    result = (Computer*)malloc(sizeof(Computer));
    result->player = player;
    result->board = createBoardFromBoard(b);
    return result;
}

void updateTree(Computer* computer, Turn* t, short player){
    short otherTurn;
    computer->board->board[t->x][t->y] = player;
    flipCaptured(computer->board, t, player);
    if(player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    fillOpenMoves(computer->board, otherTurn);
}

Turn* makeMove(Computer* c){
        Turn* max;
        MoveNode* itr;
        Board* b;
        Turn* t;
        float maxh, heuristic;
        short otherTurn;
        maxh = -1;
        if(c->player == BLACK){
            otherTurn = WHITE;
        } else {
            otherTurn = BLACK;
        }
        max = c->board->openMoves->head->turn;
        itr = c->board->openMoves->head;
        while(itr->next != NULL_PTR){
            b = createBoardFromBoard(c->board);
            t = itr->turn;
            b->board[t->x][t->y] = c->player;
            flipCaptured(b, t, c->player);
            fillOpenMoves(b, otherTurn);
            //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
            heuristic = processNode(3, otherTurn, b, c);
            if(heuristic > maxh){
                maxh = heuristic; max = t;
            }
            destroyBoard(b);
        }
        return max;

}

float processNode(int levelsLeft, int player, Board* board, Computer* callback){
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
            b = createBoardFromBoard(board);
            t = itr->turn;
            b->board[t->x][t->y] = callback->player;
            flipCaptured(b, t, callback->player);
            fillOpenMoves(b, otherTurn);
            //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
            heuristic = processNode(levelsLeft-1, otherTurn, b, callback);
            if(heuristic > maxh){
                maxh = heuristic;
            }
            if(heuristic < minh){
                minh = heuristic;
            }
            destroyBoard(b);
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
    result = ((float)ourMoveCount) - ((float)oppmovecount) / ((float)divisor);
    return result;

}

void destroyComputer(Computer* c){
    destroyBoard(c->board);
    free(c);
}
