#include "Computer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TREEDEPTH 4

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

Computer* createComputer(Board* b, short compplayer, short activeboardplayer){
    Computer* result;
    result = (Computer*)malloc(sizeof(Computer));
    result->player = compplayer;
    result->root = malloc(sizeof(MinimaxNode));
    result->root->board = createBoardFromBoard(b);
    result->root->childrenProcessed = 0;
    result->root->heuristic = getHeuristic(result, result->root->board);
    result->root->player = activeboardplayer;
    processNode(TREEDEPTH, result->root, result, -1000, 1000, 0);
    return result;
}

void updateTree(Computer* computer, Board* newb, short player){
    int count;
    MinimaxNode* oldroot;
    MinimaxNode* newroot;
    oldroot = computer->root;
    for(count = 0; count < computer->root->noChildren; count++){
        if(compareBoards(computer->root->children[count].board, newb) == 0){
            newroot = (MinimaxNode*)malloc(sizeof(MinimaxNode));
            newroot->board = computer->root->children[count].board;
            newroot->heuristic = computer->root->children[count].heuristic;
            newroot->children = computer->root->children[count].children;
            newroot->childrenProcessed = computer->root->children[count].childrenProcessed;
            newroot->player = computer->root->children[count].player;
            newroot->noChildren = computer->root->children[count].noChildren;
        } else {
            //destroyMiniMax(computer->root->children + count);
        }
    }
    free(oldroot->children);
    free(oldroot->board);
    free(oldroot);
    computer->root = newroot;
}

void fillNode(MinimaxNode* mn, Computer* callback){
    int count;
    MoveNode* itr;
    Turn* t;
    short otherTurn;
    if(mn->player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    itr = mn->board->openMoves->head;
    if(mn->childrenProcessed != 0){
        return;
    } else {
        mn->noChildren = getMoveListSize(mn->board->openMoves);
        mn->children = malloc(mn->noChildren * sizeof(MinimaxNode));
        count = 0;
        while(itr != NULL_PTR){
            mn->children[count].board = createBoardFromBoard(mn->board);
            t = itr->turn;
            mn->children[count].player = otherTurn;
            mn->children[count].childrenProcessed = 0;
            flipCaptured(mn->children[count].board, t, mn->player);
            fillOpenMoves(mn->children[count].board, otherTurn);
            mn->children[count].heuristic = getHeuristic(callback, mn->children[count].board);
            count++;
            itr = itr->next;
        }

        mn->childrenProcessed = 1;

    }
    //printf("node filled \n");
}

int mincmpfunc (const void * a, const void * b)
{
    MinimaxNode* ma = (MinimaxNode*)a;
    MinimaxNode* mb = (MinimaxNode*)b;
   return ( ma->heuristic - mb->heuristic );
}

int maxcmpfunc (const void * a, const void * b)
{
   MinimaxNode* ma = (MinimaxNode*)a;
    MinimaxNode* mb = (MinimaxNode*)b;
   return ( mb->heuristic - ma->heuristic );
}


Turn* makeMove(Computer* c){
    float alpha, beta;
    Turn* max;
    int count;
    float maxh, heuristic;

    maxh = -1000;
    alpha = -1000;
    beta = 1000;

    max = c->root->board->openMoves->head->turn;
    if(c->root->childrenProcessed == 0){
        fillNode(c->root, c);
    } else {
        printf("node already created at depth ROOT\n");
    }
    qsort(c->root->children, c->root->noChildren, sizeof(MinimaxNode), maxcmpfunc);

    for(count = 0; count < c->root->noChildren; count++){



        //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
        heuristic = processNode(TREEDEPTH, &c->root->children[count], c, alpha, beta, 0);
        if(heuristic > maxh){
            maxh = heuristic;
            max = getTurn(c->root->board->openMoves, count);
            alpha = heuristic;
        }


    }
    return max;

}

float processNode(int levelsLeft, MinimaxNode* node, Computer* callback, float alpha, float beta, short deadMoves){


    int count;
    float best, heuristic;
    short otherTurn;
    if(node->player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }

    if(isMoveListEmpty(node->board->openMoves)){
        deadMoves++;
        if(deadMoves == 2){
            return getHeuristic(callback, node->board);
        }
        if(node->childrenProcessed == 0){
            node->noChildren = 1;
            node->children = malloc(sizeof(MinimaxNode));
            node->children[0].board = createBoardFromBoard(node->board);
            node->children[0].player = otherTurn;
            node->children[0].childrenProcessed = 0;
            fillOpenMoves(node->children[0].board, otherTurn);
            node->children[0].heuristic = getHeuristic(callback, node->children[0].board);
        }
        return processNode(levelsLeft, &node->children[0], callback, alpha, beta, deadMoves);
    } else {
        deadMoves = 0;
    }
    if(levelsLeft == 0){
        return getHeuristic(callback, node->board);
    } else {
       if(node->player == callback->player){
            best = alpha;
            if(node->childrenProcessed == 0){
                fillNode(node, callback);

                 printf("node filed at depth %i\n", levelsLeft);
            } else {
                printf("node already created at depth %i\n", levelsLeft);
            }
                qsort(node->children, node->noChildren, sizeof(MinimaxNode), maxcmpfunc);
                for(count = 0; count < node->noChildren; count++){
               // printf("alpha: %f, beta: %f", alpha, beta);

                //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
                heuristic = processNode(levelsLeft-1, &node->children[count], callback, best, beta, deadMoves);
                if(heuristic > best) best = heuristic;
                if(beta <= best) {

                printf("\tpruning");
                    break;
                };


            }

       } else {
            best = beta;
            if(node->childrenProcessed == 0){
                fillNode(node, callback);
                 printf("node filed at depth %i\n", levelsLeft);
            } else {
                printf("node already created at depth %i\n", levelsLeft);
            }
            qsort(node->children, node->noChildren, sizeof(MinimaxNode), mincmpfunc);
                for(count = 0; count < node->noChildren; count++){
               // printf("alpha: %f, beta: %f", alpha, beta);

                //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
                heuristic = processNode(levelsLeft-1, node->children+count, callback, alpha, best, deadMoves);
                if(heuristic < best) best = heuristic;
                if(best <= alpha) {
                        printf("\tpruning");
                    break;

                };

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
    int friendly_stones, enemy_stones, early_game, count_goodness, i, j, positional_goodness;
    if(c->player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    friendly_stones = countPieces(b, c->player);
    //printf("friendly stones %d\n", friendly_stones);
    enemy_stones = countPieces(b, otherTurn);
    if(friendly_stones + enemy_stones > 40){
        early_game = 0;
    } else {
        early_game = 1;
    }
    if( early_game ){
        // give-away in the early game
        //fillOpenMoves(b, c->player);
        //fillOpenMoves(b, otherTurn);
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
    //printf("count_goodness is %d and positional_goodness is %d \n", count_goodness, positional_goodness);
    return count_goodness + positional_goodness;

}

void destroyComputer(Computer* c){
    destroyMiniMax(c->root);
    free(c->root);
    free(c);
}
