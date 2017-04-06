int TREEDEPTH;
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GL/glut.h"
#include "SOIL/SOIL.h"
#include "Board.h"
#include "Computer.h"
#include <windows.h>

#define PI 3.14159265
#define GL_CLAMP_TO_EDGE 0x812F

typedef struct tagRect{
    float x;
    float y;
    float width;
    float height;
} Rect;
GLuint newGame;
GLuint quit;
Rect boardBounds;
Rect screenBounds;
Rect newButton;
Rect quiButton;

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

void startnewgame(){
    b = createBoard();
    playerColor = BLACK;
    activePlayer = BLACK;
    fillOpenMoves(b, activePlayer);
    compOpp = createComputer(b, WHITE, activePlayer);
}

void renderScene(void) {
    int i, j, count;
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
                glTranslatef(boardBounds.x + boardBounds.width/8*(i + 0.5), boardBounds.y + boardBounds.height/8*(8 - (j + 0.5)), 0);
                glBegin(GL_POLYGON);
                for(count = 0; count < 20; count ++){
                    glVertex3f(sin(2*PI / 20 * count)*boardBounds.width/16, cos(2*PI / 20 * count)*boardBounds.height/16, 0.0f);
                }

                glEnd();
                glLoadIdentity();
            }
        }
	}
	glColor3f(1.0, 0.0, 0.0);
	itr = b->openMoves->head;
    while(itr != NULL_PTR){
        glTranslatef(boardBounds.x + boardBounds.width/8*(itr->turn->x + 0.5), boardBounds.y + boardBounds.height/8*(8 - (itr->turn->y + 0.5)), 0);
        glBegin(GL_POLYGON);
        for(count = 0; count < 20; count ++){
            glVertex3f(sin(2*PI / 20 * count)*boardBounds.width/64, cos(2*PI / 20 * count)*boardBounds.height/64, 0.0f);
        }
        glEnd();
        glLoadIdentity();
        itr = itr->next;
    }
    glColor4f(0.0, 0.0, 0.0, 0.1);
    glBindTexture(GL_TEXTURE_2D, newGame);
    glBegin(GL_QUADS);
            glTexCoord2f(1, 1);glVertex3f(newButton.x, screenBounds.height - newButton.y, 0.0f);
            glTexCoord2f(0, 1);glVertex3f(newButton.x + newButton.width, screenBounds.height - newButton.y, 0.0f);
            glTexCoord2f(0, 0);glVertex3f(newButton.x + newButton.width, screenBounds.height - (newButton.y + newButton.height), 0.0f);
            glTexCoord2f(1, 0);glVertex3f(newButton.x, screenBounds.height - (newButton.y + newButton.height), 0.0f);
          glEnd();
          glBindTexture(GL_TEXTURE_2D, quit);
      glBegin(GL_QUADS);
            glTexCoord2f(0, 0);glVertex3f(quiButton.x, screenBounds.height - quiButton.y, 0.0f);
            glTexCoord2f(0, 1);glVertex3f(quiButton.x + quiButton.width, screenBounds.height - quiButton.y, 0.0f);
            glTexCoord2f(1, 1);glVertex3f(quiButton.x + newButton.width, screenBounds.height - (quiButton.y + quiButton.height), 0.0f);
            glTexCoord2f(1, 0);glVertex3f(quiButton.x, screenBounds.height - (quiButton.y + quiButton.height), 0.0f);
          glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glutSwapBuffers();

}

void processKeyboard(int button, int state, int x, int y){
    printf("Clicked\n");
    int boardx, boardy, result, blackcount, whitecount;
    short cellX, cellY, goodMove;
    MoveNode* itr;
    Turn* itrTurn;
    Turn* compTurn;
    char winString[50];
    goodMove = 0;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(contains(&newButton, x, y)){
            destroyBoard(b);
            destroyComputer(compOpp);
            startnewgame();
        } else if(contains(&quiButton, x, y)){
            destroyBoard(b);
            destroyComputer(compOpp);
            exit(0);
        } else if(contains(&boardBounds, x, y)){
            if(activePlayer == playerColor){
                boardx = x - boardBounds.x;
                boardy = y - boardBounds.y;
                cellX = (int) boardx / (boardBounds.width /8);
                cellY = (int) boardy / (boardBounds.height /8);
                printf("click was made on cell %i %i\n", cellX, cellY);
                if(isMoveListEmpty(b->openMoves)){
                    printf("Move List was empty\n");
                } else {
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
                        switchPlayer();

                        if(fillOpenMoves(b, activePlayer)){
                            renderScene();
                            blackcount = countPieces(b, BLACK);
                            whitecount = countPieces(b, WHITE);
                            if(whitecount > blackcount){
                                sprintf(winString, "White Wins with %i counters to Black's %i\nPlay Again?", whitecount, blackcount);
                            }else if(whitecount < blackcount){
                                sprintf(winString, "Black Wins with %i counters to Whites's %i\nPlay Again?", blackcount, whitecount);
                            } else {
                                sprintf(winString, "A draw with both players have %i counters\nPlay Again?", whitecount);
                            }
                            result = MessageBox(NULL, winString, "Game Over", MB_YESNO);
                            if(result == IDYES){
                                destroyBoard(b);
                                destroyComputer(compOpp);
                                startnewgame();
                                return;
                            } else {
                                destroyBoard(b);
                                destroyComputer(compOpp);
                                exit(0);
                            }
                        }
                        renderScene();
                        updateTree(compOpp, b, activePlayer);
                        printf("computer data updated\n");
                        compTurn = makeMove(compOpp);
                        printf("computer calculated move at %i, %i\n", compTurn->x, compTurn->y);
                        flipCaptured(b, compTurn, activePlayer);
                        switchPlayer();
                        printf("computer pieces captured\n");
                        //fillOpenMoves(b, activePlayer);
                        //commene
                        if(fillOpenMoves(b, activePlayer)){
                            renderScene();
                            blackcount = countPieces(b, BLACK);
                            whitecount = countPieces(b, WHITE);
                            if(whitecount > blackcount){
                                sprintf(winString, "White Wins with %i counters to Black's %i\nPlay Again?", whitecount, blackcount);
                            }else if(whitecount < blackcount){
                                sprintf(winString, "Black Wins with %i counters to Whites's %i\nPlay Again?", blackcount, whitecount);
                            } else {
                                sprintf(winString, "A draw with both players have %i counters\nPlay Again?", whitecount);
                            }
                            result = MessageBox(NULL, winString, "Game Over", MB_YESNO);
                            if(result == IDYES){
                                destroyBoard(b);
                                destroyComputer(compOpp);
                                b = createBoard();
                                playerColor = BLACK;
                                activePlayer = BLACK;
                                compOpp = createComputer(b, WHITE, activePlayer);
                                fillOpenMoves(b, activePlayer);
                                return;
                            } else {
                                destroyBoard(b);
                                destroyComputer(compOpp);
                                exit(0);
                            }
                        }
                        renderScene();
                        printf("moves filled\n");
                        updateTree(compOpp, b, activePlayer);
                        printf("computer data updated again\n");
                        destroyTurn(compTurn);
                        printf("turn destroyed\n");
                    }
                }
            }
        }
        printf("click code finished\n");
    }
}


void resize(int width, int height) {
    // we ignore the params and do:
    glutReshapeWindow( 1000, 600);
};

int main(int argc, char **argv)
{

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    boardBounds.x = 300;
    boardBounds.y = 00;
    boardBounds.width = 700;
    boardBounds.height = 600;
    screenBounds.x = 100;
    screenBounds.y = 100;
    screenBounds.width = 1000;
    screenBounds.height = 600;
    quiButton.x = 0;
    quiButton.width = boardBounds.x;
    quiButton.height = 80;
    quiButton.y = screenBounds.height - quiButton.height - 20;
    newButton.x = 0;
    newButton.width = boardBounds.x;
    newButton.height = 80;
    newButton.y = screenBounds.height - quiButton.height - 20 - newButton.height - 20;;
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(screenBounds.x, screenBounds.y);
	glutInitWindowSize(screenBounds.width, screenBounds.height);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");
    glutReshapeFunc(resize);

    newGame = SOIL_load_OGL_texture
	(
		"newgame.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
    if( 0 == newGame )
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    glBindTexture(GL_TEXTURE_2D, newGame);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    quit = SOIL_load_OGL_texture
	(
		"quit.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
    glBindTexture(GL_TEXTURE_2D, quit);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// register callbacks
	glutDisplayFunc(renderScene);

	glutMouseFunc(processKeyboard);

	// enter GLUT event processing cycle
	glClearColor(0.133, 0.545, 0.133, 1.0);
    glLineWidth(2.0f);

    startnewgame();


	glutMainLoop();

	return 0;

}
