#ifndef BOARD_H_
#define BOARD_H_

#include "List.h"
typedef unsigned long long uint64_t;

extern const short EMPTY;
extern const short WHITE;
extern const short BLACK;

struct MoveList;

typedef struct tagBoard {
    uint64_t white;
    uint64_t black;
    MoveList* openMoves;
} Board;

//constructors
Board* createBoard();
Board* createBoardFromData(uint64_t white, uint64_t black, MoveList* ml);
Board* createBoardFromBoard(Board* board);

//oo functions
short isMovePossible(Board* b, short pactivePlayer);
unsigned short getTile(Board* b, short i, short j);
void setTile(Board* b, short color, short i, short j);
unsigned short getTilel(uint64_t board, short i, short j);
void setTilel(uint64_t* board, short i, short j);
int allAdjacentTiles(Board* b);
short fillOpenMoves(Board* b, short pactivePlayer);
void flipCaptured(Board* b, Turn* newPiece, short pactivePlayer);
int mostPieces(Board* b);
void destroyBoard(Board* board);
int countPieces(Board* b, short color);
int compareBoards(Board* b1, Board* b2);
#endif // BOARD_H_
