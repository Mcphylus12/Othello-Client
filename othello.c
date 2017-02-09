#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"

#include "Board.h"
#include "Computer.h"
/**
*  TODO
*   fillopenmove goodmove boolean never getting set to true
*
*
*/
typedef struct tagRect{
    float x;
    float y;
    float width;
    float height;
} Rect;

Rect boardBounds;
Rect screenBounds;

Computer* compOpp;
short playerColor;
short activePlayer;
Board* b;

short contains(Rect* rect, int x, int y){
    if(x < rect->x ||
        x > rect->x + rect->width ||
        y < rect->y ||
        y > rect->y + rect->height){
            return 0;
        } else {
            return 1;
        }
}

void switchPlayer(){
    activePlayer = activePlayer == 1 ? 2 : 1;
    if(activePlayer== BLACK){
        activePlayer = WHITE;
    } else {
        activePlayer = BLACK;
    }
}

void renderScene(void) {
    int i, j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenBounds.width, 0, screenBounds.height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
    glColor3f(0, 0, 0);
	glBegin(GL_LINES);
        for (i = 1 ; i <= 7; i++){
            glVertex3f(boardBounds.x + boardBounds.width/8*i, boardBounds.y, 0.0f); glVertex3f(boardBounds.x + boardBounds.width/8*i, boardBounds.y + boardBounds.height, 0.0f);
            //g.drawLine(boardBounds.x + boardBounds.width/8*i, boardBounds.y, boardBounds.x + boardBounds.width/8*i, boardBounds.y + boardBounds.height);

            glVertex3f(boardBounds.x, boardBounds.y + boardBounds.height/8*i, 0.0f); glVertex3f(boardBounds.x + boardBounds.width, boardBounds.y + boardBounds.height/8*i, 0.0f);
            //g.drawLine(boardBounds.x, boardBounds.y + boardBounds.height/8*i, boardBounds.x + boardBounds.width, boardBounds.y + boardBounds.height/8*i);
        }
	glEnd();

	for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            if(b->board[i][j] != EMPTY){
                if(b->board[i][j] == BLACK){
                    glColor3f(0, 0, 0);
                } else {
                    glColor3f(1, 1, 1);
                }
                glBegin(GL_LINES);
                    glVertex3f(boardBounds.x + boardBounds.width/8*i, boardBounds.y + boardBounds.height/8*j, 0.0f);
                    glVertex3f(boardBounds.x + boardBounds.width/8*(i+1), boardBounds.y + boardBounds.height/8*(j+1), 0.0f);
                glEnd();
            }
        }
	}

    glutSwapBuffers();

}

void processKeyboard(int button, int state, int x, int y){
    printf("Clicked\n");
    int boardx, boardy;
    short cellX, cellY, goodMove;
    MoveNode* itr;
    Turn* itrTurn;
    Turn* compTurn;
    goodMove = 0;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        printf("Click Correct\n");
        if(contains(&boardBounds, x, y)){
            printf("Click in Board\n");
            if(activePlayer == playerColor){
                printf("Click was made by player\n");
                boardx = x - boardBounds.y;
                boardy = y - boardBounds.y;
                cellX = (int) boardx / (boardBounds.width /8);
                cellY = (int) boardy / (boardBounds.height /8);
                printf("click was made on cell %i %i\n", cellX, cellY);
                if(isMoveListEmpty(b->openMoves)){
                    printf("Move List was empty\n");
                } else {
                    printf("moveList was populated");
                    itr = b->openMoves->head;
                    while(itr != NULL_PTR){
                        itrTurn = itr->turn;
                        if(itrTurn->x == cellX && itrTurn->y == cellY){
                            printf("move was found to be good");
                            goodMove = 1;
                            break;
                        }
                        itr = itr->next;
                    }
                    if(goodMove){
                        printf("move was good confirmed");
                        b->board[itrTurn->x][itrTurn->y] = activePlayer;
                        flipCaptured(b, itrTurn, activePlayer);
                        updateTree(compOpp, itrTurn, activePlayer);
                        compTurn = makeMove(compOpp);

                        switchPlayer();
                        b->board[compTurn->x][compTurn->y] = activePlayer;
                        flipCaptured(b, compTurn, activePlayer);
                        switchPlayer();
                        fillOpenMoves(b, activePlayer);
                        switchPlayer();
                        updateTree(compOpp, compTurn, activePlayer);
                        switchPlayer();
                        destroyTurn(compTurn);
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{

    boardBounds.x = 00;
    boardBounds.y = 00;
    boardBounds.width = 1000;
    boardBounds.height = 600;
    screenBounds.x = 100;
    screenBounds.y = 100;
    screenBounds.width = 1000;
    screenBounds.height = 600;
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(screenBounds.x, screenBounds.y);
	glutInitWindowSize(screenBounds.width, screenBounds.height);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	// register callbacks
	glutDisplayFunc(renderScene);

	glutMouseFunc(processKeyboard);

	// enter GLUT event processing cycle
	glClearColor(0.133, 0.545, 0.133, 1.0);
    glLineWidth(2.0f);

    b = createBoard();
    playerColor = BLACK;
    activePlayer = BLACK;
    compOpp = createComputer(b, WHITE);
    fillOpenMoves(b, activePlayer);

	glutMainLoop();

	return 0;

}
