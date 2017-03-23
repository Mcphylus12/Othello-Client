#include "Computer.h"
#include <stdlib.h>
#include <stdio.h>

const int posTable[8][8] = {
   { 100, -25, 10, 5, 5, 10, -25,  100,},
   {-25, -25,  1, 1, 1,  1, -25, -25,},
   { 10,   1,  5, 2, 2,  5,   1,  10,},
   {  5,   1,  2, 1, 1,  2,   1,   5,},
   {  5,   1,  2, 1, 1,  2,   1,   5,},
   { 10,   1,  5, 2, 2,  5,   1,  10,},
   {-25, -25,  1, 1, 1,  1, -25, -25,},
   { 100, -25, 10, 5, 5, 10, -25,  100,}
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
    maxh = -1000;
    alpha = -1000;
    beta = 1000;
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
        heuristic = processNode(5, otherTurn, b, c, alpha, beta);
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
    float best, heuristic;
    short otherTurn;
    if(callback->player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    itr = board->openMoves->head;

    if(isMoveListEmpty(board->openMoves)){
        return getHeuristic(callback, board);
    }
    if(levelsLeft == 0){
        return getHeuristic(callback, board);
    } else {
       if(player == callback->player){
            best = alpha;
                while(itr->next != NULL_PTR){
               // printf("alpha: %f, beta: %f", alpha, beta);

                b = createBoardFromBoard(board);
                t = itr->turn;
                flipCaptured(b, t, callback->player);
                fillOpenMoves(b, otherTurn);
                //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
                heuristic = processNode(levelsLeft-1, otherTurn, b, callback, best, beta);
                if(heuristic > best) best = heuristic;
                if(beta <= best) break;

                itr = itr->next;
                destroyBoard(b);

            }

       } else {
            best = beta;
                       while(itr->next != NULL_PTR){
               // printf("alpha: %f, beta: %f", alpha, beta);

                b = createBoardFromBoard(board);
                t = itr->turn;
                flipCaptured(b, t, callback->player);
                fillOpenMoves(b, otherTurn);
                //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
                heuristic = processNode(levelsLeft-1, callback->player, b, callback, alpha, best);
                if(heuristic < best) best = heuristic;
                if(best <= alpha) break;

                itr = itr->next;
                destroyBoard(b);
            }
       }
       return best;
    }
}
/*
 while(itr->next != NULL_PTR){
                printf("running  at level %i\n", levelsLeft);
            if( 1){
                   // printf("alpha: %f, beta: %f", alpha, beta);
                b = createBoardFromBoard(board);
                t = itr->turn;
                flipCaptured(b, t, callback->player);
                fillOpenMoves(b, otherTurn);
                //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
                heuristic = processNode(levelsLeft-1, otherTurn, b, callback, alpha, beta);
                if(heuristic > maxh){
                    maxh = heuristic;

                }
                if(heuristic > alpha) alpha = heuristic;
                if(heuristic < minh){
                    minh = heuristic;

                }
                if(heuristic < beta) beta = heuristic;
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
*/

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
    short otherTurn;
    int friendly_stones, enemy_stones, enemy_moves, friendly_moves, early_game, count_goodness, i, j, positional_goodness;
    if(c->player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    friendly_stones = countPieces(b, c->player);
    printf("friendly stones %d\n", friendly_stones);
    enemy_stones = countPieces(b, otherTurn);
    if(friendly_stones + enemy_stones > 40){
        early_game = 0;
    } else {
        early_game = 1;
    }
    if( early_game ){
        // give-away in the early game
        fillOpenMoves(b, c->player);
        friendly_moves = getMoveListSize(b->openMoves);
        fillOpenMoves(b, otherTurn);
        enemy_moves = getMoveListSize(b->openMoves);
        count_goodness = 0.1*( enemy_stones - friendly_stones );// + 1*(friendly_moves - enemy_moves);
    }else{
        // take-back later in the game
        count_goodness = 1*( friendly_stones - enemy_stones );
    }
    positional_goodness = 0;
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            if(getTile(b, i, j) == c->player) positional_goodness += posTable[i][j];
            if(getTile(b, i, j) == otherTurn) positional_goodness -= posTable[i][j];
        }
    }
    printf("count_goodness is %d and positional_goodness is %d \n", count_goodness, positional_goodness);
    return count_goodness + positional_goodness;

}

void destroyComputer(Computer* c){
    destroyBoard(c->board);
    free(c);
}
