#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"

#include "Board.h"
#include "Computer.h"



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
    if(activePlayer== BLACK){
        activePlayer = WHITE;
    } else {
        activePlayer = BLACK;
    }
}

void renderScene(void) {
    int i, j;
    MoveNode* itr;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenBounds.width, 0, screenBounds.height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
    glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(boardBounds.x, boardBounds.y, 0.0f); glVertex3f(boardBounds.x, boardBounds.y + boardBounds.height, 0.0f);
    glVertex3f(boardBounds.x, boardBounds.y, 0.0f); glVertex3f(boardBounds.x + boardBounds.width, boardBounds.y, 0.0f);
    glVertex3f(boardBounds.x + boardBounds.width, boardBounds.y, 0.0f); glVertex3f(boardBounds.x + boardBounds.width, boardBounds.y + boardBounds.height, 0.0f);
    glVertex3f(boardBounds.x, boardBounds.y + boardBounds.height, 0.0f); glVertex3f(boardBounds.x + boardBounds.width, boardBounds.y + boardBounds.height, 0.0f);


        for (i = 1 ; i <= 7; i++){
            glVertex3f(boardBounds.x + boardBounds.width/8*i, boardBounds.y, 0.0f); glVertex3f(boardBounds.x + boardBounds.width/8*i, boardBounds.y + boardBounds.height, 0.0f);
            //g.drawLine(boardBounds.x + boardBounds.width/8*i, boardBounds.y, boardBounds.x + boardBounds.width/8*i, boardBounds.y + boardBounds.height);

            glVertex3f(boardBounds.x, boardBounds.y + boardBounds.height/8*i, 0.0f); glVertex3f(boardBounds.x + boardBounds.width, boardBounds.y + boardBounds.height/8*i, 0.0f);
            //g.drawLine(boardBounds.x, boardBounds.y + boardBounds.height/8*i, boardBounds.x + boardBounds.width, boardBounds.y + boardBounds.height/8*i);
        }
	glEnd();

	for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            if(getTile(b, i, j) != EMPTY){
                if(getTile(b, i, j) == BLACK){
                    glColor3f(0, 0, 0);
                } else {
                    glColor3f(1, 1, 1);
                }
                glBegin(GL_POLYGON);
                    glVertex3f(boardBounds.x + boardBounds.width/8*(i), boardBounds.y + boardBounds.height/8*(8 - j), 0.0f);
                    glVertex3f(boardBounds.x + boardBounds.width/8*(i), boardBounds.y + boardBounds.height/8*(8 - (j+1)), 0.0f);
                    glVertex3f(boardBounds.x + boardBounds.width/8*(i+1), boardBounds.y + boardBounds.height/8*(8 - (j+1)), 0.0f);
                    glVertex3f(boardBounds.x + boardBounds.width/8*(i+1), boardBounds.y + boardBounds.height/8*(8 - j), 0.0f);
                glEnd();
            }
        }
	}
	glColor3f(1.0, 0.0, 0.0);
	itr = b->openMoves->head;
    while(itr != NULL_PTR){
        glBegin(GL_POLYGON);
            glVertex3f(boardBounds.x + boardBounds.width/8*(itr->turn->x), boardBounds.y + boardBounds.height/8*(8 - itr->turn->y), 0.0f);
            glVertex3f(boardBounds.x + boardBounds.width/8*(itr->turn->x), boardBounds.y + boardBounds.height/8*(8 - (itr->turn->y+1)), 0.0f);
            glVertex3f(boardBounds.x + boardBounds.width/8*(itr->turn->x+1), boardBounds.y + boardBounds.height/8*(8 - (itr->turn->y+1)), 0.0f);
            glVertex3f(boardBounds.x + boardBounds.width/8*(itr->turn->x+1), boardBounds.y + boardBounds.height/8*(8 - (itr->turn->y)), 0.0f);
        glEnd();
        itr = itr->next;
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
        if(contains(&boardBounds, x, y)){
            if(activePlayer == playerColor){
                boardx = x - boardBounds.x;
                boardy = y - boardBounds.y;
                cellX = (int) boardx / (boardBounds.width /8);
                cellY = (int) boardy / (boardBounds.height /8);
                printf("click was made on cell %i %i\n", cellX, cellY);
                if(isMoveListEmpty(b->openMoves)){
                    printf("Move List was empty\n");
                } else {
                    printf("moveList was populated\n");
                    itr = b->openMoves->head;
                    while(itr != NULL_PTR){
                        itrTurn = itr->turn;
                        if(itrTurn->x == cellX && itrTurn->y == cellY){
                            goodMove = 1;
                            break;
                        }
                        itr = itr->next;
                    }
                    if(goodMove){
                        printf("move was good\n");
                        flipCaptured(b, itrTurn, activePlayer);
                        printf("pieces captured\n");
                        updateTree(compOpp, itrTurn, activePlayer);
                        printf("computer data updated\n");
                        compTurn = makeMove(compOpp);
                        printf("computer calculated move at %i, %i\n", compTurn->x, compTurn->y);
                        switchPlayer();
                        flipCaptured(b, compTurn, activePlayer);
                        printf("computer pieces captured\n");
                        switchPlayer();
                        fillOpenMoves(b, activePlayer);
                        printf("moves filled\n");
                        switchPlayer();
                        updateTree(compOpp, compTurn, activePlayer);
                        printf("computer data updated again\n");
                        switchPlayer();
                        destroyTurn(compTurn);
                        printf("turn destroyed\n");
                    }
                }
            }
        }
        printf("click code finished\n");
    }
}

int main(int argc, char **argv)
{

    boardBounds.x = 300;
    boardBounds.y = 00;
    boardBounds.width = 700;
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
