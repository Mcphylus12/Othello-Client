#include "Board.h"
#include <stdlib.h>
#include <stdio.h>

#include "Turn.h"
#include "List.h"


const short EMPTY = 0;
const short WHITE = 1;
const short BLACK = 2;

const uint64_t m1  = 0x5555555555555555; //binary: 0101...
const uint64_t m2  = 0x3333333333333333; //binary: 00110011..
const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
const uint64_t m8  = 0x00ff00ff00ff00ff; //binary:  8 zeros,  8 ones ...
const uint64_t m16 = 0x0000ffff0000ffff; //binary: 16 zeros, 16 ones ...
const uint64_t m32 = 0x00000000ffffffff; //binary: 32 zeros, 32 ones
const uint64_t hff = 0xffffffffffffffff; //binary: all ones
const uint64_t h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,2,3...

Board* createBoard(){
    Board* result;
    result = (Board*)malloc(sizeof(Board));
    result->openMoves = createMoveList();
    result->white = 0;
    result->black = 0;
    setTile(result, BLACK, 3, 3);
    setTile(result, BLACK, 4, 4);
    setTile(result, WHITE, 4, 3);
    setTile(result, WHITE, 3, 4);
    return result;
}

Board* createBoardFromData(uint64_t white, uint64_t black, MoveList* ml){
    Board* result;
    result = (Board*)malloc(sizeof(Board));
    result->white = white;
    result->black = black;
    result->openMoves = createMoveListFromList(ml);
    return result;
}

Board* createBoardFromBoard(Board* board){
    Board* result;
    result = (Board*)malloc(sizeof(Board));

    result->white = board->white;
    result->black = board->black;
    result->openMoves = createMoveListFromList(board->openMoves);
    return result;
}

unsigned short getTile(Board* board, short i, short j){
    if(i < 0 || i > 7 || j < 0 || j > 7){
        return EMPTY;
    } else {
        if(board->white & (1LL<<(8*(j)+(i)))) return WHITE;
        if(board->black & (1LL<<(8*(j)+(i)))) return BLACK;
    }
    return EMPTY;
}

void setTile(Board* b, short color, short i, short j){
    if(color == WHITE){
        b->white = (b->white | (1LL<<(8*(j)+(i))));
    } else if (color == BLACK){
        b->black = (b->black | (1LL<<(8*(j)+(i))));
    }
}

unsigned short getTilel(uint64_t board, short i, short j){
    if(i < 0 || i > 7 || j < 0 || j > 7){
        return 0;
    } else {
        if(board & (1LL<<(8*(j)+(i)))) return 1;
    }
    return 0;
}

void setTilel(uint64_t* board, short i, short j){

        *board = (*board | (1LL<<(8*(j)+(i))));

}

int allAdjacentTiles(Board* b){
    int tileTot;
    short i, j;
    tileTot = 0;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if(getTile(b, i, j) == EMPTY){

                if(getTile(b, i + 1,j + 1) != EMPTY){
                    tileTot++;

                } else if(getTile(b, i,j + 1) != EMPTY){
                    tileTot++;

                } else if(getTile(b, i - 1,j + 1) != EMPTY){
                    tileTot++;

                } else if(getTile(b, i + 1,j) != EMPTY){
                    tileTot++;

                } else if(getTile(b, i - 1,j) != EMPTY){
                    tileTot++;

                } else if(getTile(b, i + 1,j - 1) != EMPTY){
                    tileTot++;

                } else if(getTile(b, i,j - 1) != EMPTY){
                    tileTot++;

                } else if(getTile(b, i - 1,j - 1) != EMPTY){
                    tileTot++;

                }
            }
        }
    }


    return tileTot;
}

//best algorithm for hamming weight (Wikipedia)
int popcount64c(uint64_t x)
{
    x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
    x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits
    x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits
    return (x * h01) >> 56;  //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ...
}

int countPieces(Board* b, short color){
    if(color == WHITE){
        return popcount64c(b->white);
    } else if(color == BLACK){
        return popcount64c(b->black);
    } else return 0;
}

short fillOpenMoves(Board* b, short pactivePlayer){
    short goodMove, k, i, j, counter, otherTurn, xdir, ydir;
    Turn* t;
    uint64_t board, tempBoard;
    if(pactivePlayer == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }

    clearList(b->openMoves);
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
           if(getTile(b, i, j) != EMPTY) continue;
           board = 0;
           for(k = 0 ; k < 9; k++){
                tempBoard = 0;
                ydir = (k%3) -1;
                xdir = (k/3) -1;
                if(!(xdir || ydir)) continue;
                counter = 1;
                if(getTile(b, i + counter*xdir, j + counter*ydir) == otherTurn){
                    setTilel(&tempBoard, i, j);
                    setTilel(&tempBoard, i + counter*xdir, j + counter*ydir);
                        while(1){
                        counter++;
                        if(getTile(b, i + counter*xdir, j + counter*ydir) == otherTurn){
                            setTilel(&tempBoard, i + counter*xdir, j + counter*ydir);
                        } else if(getTile(b, i + counter*xdir, j + counter*ydir) == EMPTY){
                            break;
                        } else if(getTile(b, i + counter*xdir, j + counter*ydir) == pactivePlayer){
                            board = board | tempBoard;
                            break;
                        }
                    }
                }


           }
           if(board){
                t = createTurn(i, j, board, pactivePlayer);
                addTurnToMoveList(b->openMoves, t);
           }

        }
    }
    return isMoveListEmpty(b->openMoves);
}

void flipCaptured(Board* b, Turn* newPiece, short pactivePlayer){
    if(pactivePlayer == WHITE){
        b->white = b->white | newPiece->board;
        b->black = b->black & (~newPiece->board);
    }

    if(pactivePlayer == BLACK){
        b->black = b->black | newPiece->board;
        b->white = b->white & (~newPiece->board);
    }

}

int mostPieces(Board* b){
    int result;
    result = countPieces(b, WHITE) - countPieces(b, BLACK);
    if(result > 0) return WHITE;
    if(result < 0) return BLACK;
    else return EMPTY;
}

void destroyBoard(Board* board){
    destroyMoveList(board->openMoves);
    free(board);
}

int compareBoards(Board* b1, Board* b2){
    if(b1->black == b2->black && b1->white == b2->white) return 0;
    return 1;
}
