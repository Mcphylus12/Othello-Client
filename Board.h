#ifndef BOARD_H_
#define BOARD_H_

#include "List.h"
extern const short EMPTY;
extern const short WHITE;
extern const short BLACK;

typedef struct tagBoard {
    unsigned short board[8][8];
    MoveList* openMoves;
} Board;

//constructors
Board* createBoard();
Board* createBoardFromData(unsigned short board[8][8], MoveList* ml);
Board* createBoardFromBoard(Board* board);

//oo functions
unsigned short getTile(Board* b, short i, short j);
int allAdjacentTiles(Board* b);
short fillOpenMoves(Board* b, int activePlayer);
void flipCaptured(Board* b, Turn* newPiece, int activePlayer);
int mostPieces(Board* b);
void destroyBoard(Board* board);
#endif // BOARD_H_
