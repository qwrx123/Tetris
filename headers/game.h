#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "block.h"
#include "textBox.h"
#include "tetrisBlock.h"
#include "helpers.h"
#include "time.h"
#include "tetrisPiece.h"

#include <random>

class game
{
public:
struct currentBlocks {
	int x;
	int y;
	int type;
	int turn;
};
// the drop down blocks you see
struct ghostBlocks {
	int x = 0;
	int y = 0;
	int type = 15;
};
private:
currentBlocks currentBlocks[4];
ghostBlocks ghostBlocks[4];
int screenBlocks[10][20];
tetrisBlock* renderScreenBlocks[10][20];
static const block::location gameSquareBoundary;
Time countTime;
float count;
float setCount;
char nextBlock[14];
int score = 0;
int level = 0;
int lines = 0;
wchar_t scoreText[10];
wchar_t levelText[10];
wchar_t linesText[10];
textBox scoreLabel;
textBox scoreDisplay;
textBox levelLabel;
textBox levelDisplay;
textBox linesLabel;
textBox linesDisplay;
textBox holdLabel;
block holdDisplay;
tetrisPiece holdPiece;
textBox nextLabel;
block nextDisplay;
tetrisPiece nextPieces[3];
static const float timing[];
bool keys[0xffff];
// current block in hold
char holding = 0;
char canHold = true;
RECT currentScreenSize;
bool died;
public:
game(ID2D1HwndRenderTarget* renderTarget, IDWriteFactory* pDWriteFactory, RECT screenSize);
~game();
void render();
void gameLoop();
void keyDown(wchar_t inputChar);
void onKey(wchar_t inputChar);
void resize(RECT newScreen);
bool testDeath();
private:
void sendBlocks();
bool dropBlocks();
void findGhost();
void moveBlocks(int direction);
void switchHold();
void checkLines();
bool turnBlocks(int xShift, int yShift);
void calculateNextBlocks(int a, int b);
void cycleNextBlocks();
};