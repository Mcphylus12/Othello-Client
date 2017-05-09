#include "Computer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Board.h"
#define TREEDEPTH 6
#define DEBUG 0

const int posTable[8][8] = {
   { 1000, -25, 20, 10, 10, 20, -25,  1000},
   {-25, -25,  1, -5, -5,  1, -25, -25},
   { 20,   1,  5, 2, 2,  5,   1,  20},
   {  10,   -5,  2, 1, 1,  2,   -5,   10},
   {  10,   -5,  2, 1, 1,  2,   -5,   10},
   { 20,   1,  5, 2, 2,  5,   1,  20},
   {-25, -25,  1, -5, -5,  1, -25, -25},
   { 1000, -25, 20, 10, 10, 20, -25,  1000}
};
Computer* createComputer(Board* b, short compplayer, short activeboardplayer, int positional, int countt, int longstable, int shortstable){
    Computer* result;
    result = (Computer*)malloc(sizeof(Computer));
    result->player = compplayer;
    result->root = malloc(sizeof(MinimaxNode));
    result->root->board = createBoardFromBoard(b);
    result->root->childrenProcessed = 0;
    result->root->heuristic = getHeuristic(result, result->root->board);
    result->root->player = activeboardplayer;
    result->positional = positional;
    result->countt = countt;
    result->longstable = longstable;
    result->shortstable = shortstable;
    processNode(TREEDEPTH, result->root, result, -10000000, 10000000, 0);
    return result;
}

void updateTree(Computer* computer, Board* newb, short player){
    int count;
    MinimaxNode* oldroot;
    MinimaxNode* newroot;
    oldroot = computer->root;
    fillNode(oldroot, computer);
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
            destroyMiniMax(computer->root->children + count);
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
        if(mn->noChildren == 0){
            mn->noChildren = 1;
            mn->children = malloc(sizeof(MinimaxNode));
            mn->children[0].board = createBoardFromBoard(mn->board);
            mn->children[0].player = otherTurn;
            mn->children[0].childrenProcessed = 0;
            fillOpenMoves(mn->children[0].board, otherTurn);
            mn->children[0].heuristic = getHeuristic(callback, mn->children[0].board);
            mn->childrenProcessed = 1;
        } else {
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
    maxh = -10000000;
    alpha = -10000000;
    beta = 10000000;
    max = c->root->board->openMoves->head->turn;
    if(c->root->childrenProcessed == 0){
        fillNode(c->root, c);
    } else {
//        printf("node already created at depth ROOT\n");
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
#if DEBUG == 1
    printf("%f was best as we want to maximise at level %i\n", maxh, TREEDEPTH +1);
#endif // DEBUG
    return max;

}

float max(float a, float b){if(a > b) return a; else return b;}
float min(float a, float b){if(a < b) return a; else return b;}

float processNode(int levelsLeft, MinimaxNode* node, Computer* callback, float alpha, float beta, short deadMoves){


    int count, count2;
    float best, heuristic;
    short otherTurn;
    if(node->player == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }


    if(levelsLeft == 0){
        best = getHeuristic(callback, node->board);
#if DEBUG == 1
           for(count2 = 0; count2 < TREEDEPTH - levelsLeft; count2++){
                printf("\t");
            }
            printf("%f was heuristic at level %i\n", best, TREEDEPTH);
#endif // DEBUG
        return best;

    } else {
       if(node->player == callback->player){
            best = -1000000;
            if(node->childrenProcessed == 0){
                fillNode(node, callback);

//                 printf("node filed at depth %i\n", levelsLeft);
            } else {
//                printf("node already created at depth %i\n", levelsLeft);
            }
                qsort(node->children, node->noChildren, sizeof(MinimaxNode), maxcmpfunc);
                //printf("processing: ");
                for(count = 0; count < node->noChildren; count++){
               // printf("alpha: %f, beta: %f", alpha, beta);
         //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
                heuristic = processNode(levelsLeft-1, &node->children[count], callback, alpha, beta, deadMoves);
                //printf("%f, ", heuristic);
                best = max(best, heuristic);
                alpha = max(alpha, best);
                if(beta <= alpha) {

//                printf("\tpruning");
                break;
                };


            }
#if DEBUg == 1
            for(count2 = 0; count2 < TREEDEPTH - levelsLeft + 1; count2++){
                printf("\t");
            }
            printf("%f was best as we want to maximise at level %i\n", best, levelsLeft);
#endif // DEBUg
       } else {
            best = 1000000;
            if(node->childrenProcessed == 0){
                fillNode(node, callback);
//                 printf("node filed at depth %i\n", levelsLeft);
            } else {
//                printf("node already created at depth %i\n", levelsLeft);
            }
            qsort(node->children, node->noChildren, sizeof(MinimaxNode), mincmpfunc);
                for(count = 0; count < node->noChildren; count++){
               // printf("alpha: %f, beta: %f", alpha, beta);

                //System.out.println("ROOT:Processing child from root made with Turn:" + t.toString());
                heuristic = processNode(levelsLeft-1, &node->children[count], callback, alpha, beta, deadMoves);
                best = min(best, heuristic);
                beta = min(beta, best);
                if(beta <= alpha) {
//                        printf("\tpruning");
                   break;

                };

            }
#if DEBUG == 1
            for(count2 = 0; count2 < TREEDEPTH - levelsLeft + 1; count2++){
                printf("\t");
            }
            printf("%f was best as we want to minimise at level %i\n", best, levelsLeft);
#endif // DEBUG
       }
       return best;
    }
}
/*
 while(itr->next != NULL_PTR){
//                printf("running  at level %i\n", levelsLeft);
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
//               printf("pruned tree \n");
                itr = itr->next;
            }
        }
        if(player == callback->player){
            return maxh;
        } else {
            return minh;
        }
*/
typedef unsigned long long uint64_t;
void  calculateStability(uint64_t board, uint64_t* resultp){
    uint64_t result;
    int width_start, width_end, height_start, height_end, i, j;
    result = *resultp;
    width_start = 0; height_start = 0; width_end = 7; height_end = 7;
    for(j = 0; j < 4; j++){
         for(i = width_start; i < width_end; i++){
            if(getTilel(board, i, height_start)){
                if(i == 0 || getTilel(result, i-1, height_start)){
                    if(height_start == 0 || getTilel(result, i, height_start-1)){
                        if((height_start == 0 && i == 0) || getTilel(result, i-1, height_start-1)){
                            setTilel(&result, i, height_start);
                        }
                    }
                    if(height_start == 7 || getTilel(result, i, height_start+1)){
                        if((height_start == 7 && i == 0) || getTilel(result, i-1, height_start+1)){
                            setTilel(&result, i, height_start);
                        }
                    }
                }
                if(i == 7 || getTilel(result, i+1, height_start)){
                    if(height_start == 0 || getTilel(result, i, height_start-1)){
                        if((height_start == 0 && i == 7) || getTilel(result, i+1, height_start-1)){
                            setTilel(&result, i, height_start);
                        }
                    }
                    if(height_start == 7 || getTilel(result, i, height_start+1)){
                        if((height_start == 7 && i == 7) || getTilel(result, i+1, height_start+1)){
                            setTilel(&result, i, height_start);
                        }
                    }
                }
            }
         }
        for(i = height_start; i < height_end; i++){
            if(getTilel(board, width_end, i)){
                if(width_end == 0 || getTilel(result, width_end-1, i)){
                    if(i == 0 || getTilel(result, width_end, i-1)){
                        if((i == 0 && width_end == 0) || getTilel(result, width_end-1, i-1)){
                            setTilel(&result, width_end, i);
                        }
                    }
                    if(i == 7 || getTilel(result, width_end, i+1)){
                        if((i == 7 && width_end == 0) || getTilel(result, width_end-1, i+1)){
                            setTilel(&result, width_end, i);
                        }
                    }
                }
                if(width_end == 7 || getTilel(result, width_end+1, i)){
                    if(i == 0 || getTilel(result, width_end, i-1)){
                        if((i == 0 && width_end == 7) || getTilel(result, width_end+1, i-1)){
                            setTilel(&result, width_end, i);
                        }
                    }
                    if(i == 7 || getTilel(result, width_end, i+1)){
                        if((i == 7 && width_end == 7) || getTilel(result, width_end+1, i+1)){
                            setTilel(&result, width_end, i);
                        }
                    }
                }
            }
        }
         for(i = width_start; i < width_end; i++){
            if(getTilel(board, i, height_end)){
                if(i == 0 || getTilel(result, i-1, height_end)){
                    if(height_end == 0 || getTilel(result, i, height_end-1)){
                        if((height_end == 0 && i == 0) || getTilel(result, i-1, height_end-1)){
                            setTilel(&result, i, height_end);
                        }
                    }
                    if(height_end == 7 || getTilel(result, i, height_end+1)){
                        if((height_end == 7 && i == 0) || getTilel(result, i-1, height_end+1)){
                            setTilel(&result, i, height_end);
                        }
                    }
                }
                if(i == 7 || getTilel(result, i+1, height_end)){
                    if(height_end == 0 || getTilel(result, i, height_end-1)){
                        if((height_end == 0 && i == 7) || getTilel(result, i+1, height_end-1)){
                            setTilel(&result, i, height_end);
                        }
                    }
                    if(height_end == 7 || getTilel(result, i, height_end+1)){
                        if((height_end == 7 && i == 7) || getTilel(result, i+1, height_end+1)){
                            setTilel(&result, i, height_end);
                        }
                    }
                }
            }
         }

         for(i = height_start; i < height_end; i++){
            if(getTilel(board, width_start, i)){
                if(width_start == 0 || getTilel(result, width_start-1, i)){
                    if(i == 0 || getTilel(result, width_start, i-1)){
                        if((i == 0 && width_start == 0) || getTilel(result, width_start-1, i-1)){
                            setTilel(&result, width_start, i);
                        }
                    }
                    if(i == 7 || getTilel(result, width_start, i+1)){
                        if((i == 7 && width_start == 0) || getTilel(result, width_start-1, i+1)){
                            setTilel(&result, width_start, i);
                        }
                    }
                }
                if(width_start == 7 || getTilel(result, width_start+1, i)){
                    if(i == 0 || getTilel(result, width_start, i-1)){
                        if((i == 0 && width_start == 7) || getTilel(result, width_start+1, i-1)){
                            setTilel(&result, width_start, i);
                        }
                    }
                    if(i == 7 || getTilel(result, width_start, i+1)){
                        if((i == 7 && width_start == 7) || getTilel(result, width_start+1, i+1)){
                            setTilel(&result, width_start, i);
                        }
                    }
                }
            }
         }
         width_start++, height_start++; width_end--; height_end--;
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
    short otherTurn;
    int friendly_stones, enemy_stones, early_game, count_goodness, i, j, positional_goodness, friendly_moves, enemy_moves, friendly_stable, enemy_stable, white_shortStable, black_shortStable, immstable;
    uint64_t white_stability, black_stability;
    Board* tmp = createBoardFromBoard(b);
    fillOpenMoves(tmp, WHITE);
    white_shortStable = getMoveListSize(b->openMoves);
    fillOpenMoves(tmp, BLACK);
    black_shortStable = getMoveListSize(b->openMoves);
    white_stability = 0;
    black_stability = 0;
    calculateStability(b->white, &white_stability);
    calculateStability(b->black, &black_stability);
    immstable = black_shortStable - white_shortStable;
    if(c->player == BLACK){
        otherTurn = WHITE;
        friendly_stable = popcount64c(black_stability);
        enemy_stable = popcount64c(white_stability);
        immstable = black_shortStable - white_shortStable;
    } else {
        otherTurn = BLACK;
        friendly_stable = popcount64c(white_stability);
        enemy_stable = popcount64c(black_stability);
        immstable = -immstable;
    }

    friendly_stones = countPieces(b, c->player);
    //printf("friendly stones %d\n", friendly_stones);
    enemy_stones = countPieces(b, otherTurn);
    friendly_moves = countMovePossible(b, c->player);
    enemy_moves = countMovePossible(b, otherTurn);
    count_goodness = 0;
    if(friendly_stones + enemy_stones > 40){
        early_game = 0;
    } else {
        early_game = 1;
    }
    if( early_game ){
        // give-away in the early game
        //fillOpenMoves(b, c->player);
        //fillOpenMoves(b, otherTurn);
        count_goodness = ( friendly_moves - enemy_moves );// + 1*(friendly_moves - enemy_moves);

    }else{
        // take-back later in the game
        count_goodness = ( friendly_stones - enemy_stones );
    }
    positional_goodness = 0;
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){

            if(getTile(b, i, j) == c->player){
                //if(i == 0 && j == 0) printf("top left good\n");
                positional_goodness += posTable[i][j];
            }

            if(getTile(b, i, j) == otherTurn){
               // if(i == 0 && j == 0) printf("top left bad\n");
               positional_goodness -= posTable[i][j];
            }
        }
    }
    destroyBoard(tmp);
    //printf("%i, %i, %i, %i, %i\n", c->positional*positional_goodness, c->countt*count_goodness, c->longstable*(friendly_stable - enemy_stable), c->shortstable*immstable,c->positional*positional_goodness + c->countt*count_goodness + c->longstable*(friendly_stable - enemy_stable) + c->shortstable*immstable);
    return c->positional*positional_goodness + c->countt*count_goodness + c->longstable*(friendly_stable - enemy_stable) + c->shortstable*immstable;

}

void destroyComputer(Computer* c){
    destroyMiniMax(c->root);
    free(c->root);
    free(c);
}
