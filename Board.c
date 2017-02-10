#include "Board.h"
#include <stdlib.h>
#include <stdio.h>

#include "Turn.h"
#include "List.h"

const short EMPTY = 0;
const short WHITE = 1;
const short BLACK = 2;

Board* createBoard(){
    short i, j;
    Board* result;
    result = (Board*)malloc(sizeof(Board));
    result->openMoves = createMoveList();
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            result->board[i][j] = EMPTY;
            result->board[3][3] = WHITE; result->board[4][4] = WHITE;
            result->board[4][3] = BLACK; result->board[3][4] = BLACK;
        }
    }
    return result;
}

Board* createBoardFromData(unsigned short board[8][8], MoveList* ml){
    short i, j;
    Board* result;
    result = (Board*)malloc(sizeof(Board));
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            result->board[i][j] = board[i][j];
        }
    }
    result->openMoves = ml;
    return result;
}

Board* createBoardFromBoard(Board* board){
    short i, j;
    Board* result;
    result = (Board*)malloc(sizeof(Board));

    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            result->board[i][j] = board->board[i][j];
        }
    }
    result->openMoves = createMoveListFromList(board->openMoves);
    return result;
}

unsigned short getTile(Board* board, short i, short j){
    if(i < 0 || i > 7 || j < 0 || j > 7){
        return EMPTY;
    } else {
        return board->board[i][j];
    }
    return EMPTY;
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

short fillOpenMoves(Board* b, int activePlayer){
    int k, i, j, otherTurn;
    short goodMove;
    short TR, R, BR, B, BL, L, TL, T;
    Turn* t;
    if(activePlayer == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }

    clearList(b->openMoves);
    for(i = 0; i < 8; i++)
    {

        for(j = 0; j < 8; j++)
        {
            if(getTile(b, i, j) == EMPTY)
            {
                goodMove = 0;
                TR = 0;
                R = 0;
                BR = 0;
                B = 0;
                BL = 0;
                L = 0;
                TL = 0;
                T = 0;

                k = 2;
                if(getTile(b, i, j + 1) == otherTurn){
                    while(1){
                       if(getTile(b, i, j + k) == EMPTY){
                           break;
                       }
                       if(getTile(b, i, j + k) == otherTurn){
                           k++;
                       }
                       if(getTile(b, i, j + k) == activePlayer){
                           T = 1;
                           goodMove = 1;break;

                       }
                    }
                }

                k = 2;
                if(getTile(b, i + 1, j + 1) == otherTurn){
                    while(1){
                       if(getTile(b, i + k, j + k) == EMPTY){
                           break;
                       }
                       if(getTile(b, i + k, j + k) == otherTurn){
                           k++;
                       }
                       if(getTile(b, i + k, j + k) == activePlayer){
                           TR = 1;
                           goodMove = 1;break;

                       }
                    }
                }


                k = 2;
                if(getTile(b, i + 1, j) == otherTurn){
                    while(1){
                       if(getTile(b, i + k, j) == EMPTY){
                           break;
                       }
                       if(getTile(b, i + k, j) == otherTurn){
                           k++;
                       }
                       if(getTile(b, i + k, j) == activePlayer){
                           R = 1;
                           goodMove = 1;break;

                       }
                    }
                }


                k = 2;
                if(getTile(b, i + 1, j - 1) == otherTurn){
                    while(1){
                       if(getTile(b, i + k, j - k) == EMPTY){
                           break;
                       }
                       if(getTile(b, i + k, j - k) == otherTurn){
                           k++;
                       }
                       if(getTile(b, i + k, j - k) == activePlayer){
                           BR = 1;
                           goodMove = 1;break;

                       }
                    }
                }


                k = 2;
                if(getTile(b, i, j - 1) == otherTurn){
                    while(1){
                       if(getTile(b, i, j - k) == EMPTY){
                           break;
                       }
                       if(getTile(b, i, j - k) == otherTurn){
                           k++;
                       }
                       if(getTile(b, i, j - k) == activePlayer){
                           B = 1;
                           goodMove = 1;break;

                       }
                    }
                }


                k = 2;
                if(getTile(b, i - 1, j - 1) == otherTurn){
                    while(1){
                       if(getTile(b, i - k, j - k) == EMPTY){
                           break;
                       }
                       if(getTile(b, i - k, j - k) == otherTurn){
                           k++;
                       }
                       if(getTile(b, i - k, j - k) == activePlayer){
                           BL = 1;
                           goodMove = 1;break;

                       }
                    }
                }


                k = 2;
                if(getTile(b, i - 1, j) == otherTurn){
                    while(1){
                       if(getTile(b, i - k, j) == EMPTY){
                           break;
                       }
                       if(getTile(b, i - k, j) == otherTurn){
                           k++;
                       }
                       if(getTile(b, i - k, j) == activePlayer){
                           L = 1;
                           goodMove = 1;break;

                       }
                    }
                }

                k = 2;
                if(getTile(b, i - 1, j + 1) == otherTurn){
                    while(1){
                       if(getTile(b, i - k, j + k) == EMPTY){
                           break;
                       }
                       if(getTile(b, i - k, j + k) == otherTurn){
                           k++;
                       }
                       if(getTile(b, i - k, j + k) == activePlayer){
                           TL = 1;
                           goodMove = 1;break;

                       }
                    }
                }

                if(goodMove){
                    t = createTurnFromXY(i, j);
                    t->B = B;
                    t->BL = BL;
                    t->BR = BR;
                    t->T = T;
                    t->TL = TL;
                    t->TR = TR;
                    t->R = R;
                    t->L =L;
                    addTurnToMoveList(b->openMoves, t);

                }
            }
        }
    }
    return isMoveListEmpty(b->openMoves);
}

void flipCaptured(Board* b, Turn* newPiece, int activePlayer){
    int otherTurn;
    if(activePlayer == BLACK){
        otherTurn = WHITE;
    } else {
        otherTurn = BLACK;
    }
    if(newPiece->B){
        int k = 1;
        while(getTile(b, newPiece->x, newPiece->y-k) == otherTurn){
            b->board[newPiece->x][newPiece->y-k] = activePlayer;
            k++;
        }
    }
    if(newPiece->BL){
        int k = 1;
        while(getTile(b, newPiece->x-k, newPiece->y-k) == otherTurn){
            b->board[newPiece->x-k][newPiece->y-k] = activePlayer;
            k++;
        }
    }
    if(newPiece->BR){
        int k = 1;
        while(getTile(b, newPiece->x+k, newPiece->y-k) == otherTurn){
            b->board[newPiece->x+k][newPiece->y-k] = activePlayer;
            k++;
        }
    }
    if(newPiece->T){
        int k = 1;
        while(getTile(b, newPiece->x, newPiece->y+k) == otherTurn){
            b->board[newPiece->x][newPiece->y+k] = activePlayer;
            k++;
        }
    }
    if(newPiece->TL){
        int k = 1;
        while(getTile(b, newPiece->x-k, newPiece->y+k) == otherTurn){
            b->board[newPiece->x-k][newPiece->y+k] = activePlayer;
            k++;
        }
    }
    if(newPiece->TR){
        int k = 1;
        while(getTile(b, newPiece->x+k, newPiece->y+k) == otherTurn){
            b->board[newPiece->x+k][newPiece->y+k] = activePlayer;
            k++;
        }
    }
    if(newPiece->R){
        int k = 1;
        while(getTile(b, newPiece->x+k, newPiece->y) == otherTurn){
            b->board[newPiece->x+k][newPiece->y] = activePlayer;
            k++;
        }
    }
    if(newPiece->L){
        int k = 1;
        while(getTile(b, newPiece->x-k, newPiece->y) == otherTurn){
            b->board[newPiece->x-k][newPiece->y] = activePlayer;
            k++;
        }
    }

}

int mostPieces(Board* b){
    int whiteTot, blackTot, i, j;
    whiteTot  = 0, blackTot = 0;
    for(i = 0; i < 7; i++){
        for(j = 0; j < 7; j++){
            if(b->board[i][j] == BLACK){
                blackTot++;
            }
            if(b->board[i][j] == WHITE){
                whiteTot++;
            }
        }
    }
    if(whiteTot > blackTot) return WHITE;
    if(blackTot > whiteTot) return BLACK;
    else return EMPTY;
}

void destroyBoard(Board* board){
    destroyMoveList(board->openMoves);
    free(board);
}
