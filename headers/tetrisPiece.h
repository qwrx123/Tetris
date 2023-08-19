#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "tetrisBlock.h"

class tetrisPiece
{
public:
private:
tetrisBlock pieces[4];
block::location coordinates;
int pieceType;
public:
tetrisPiece(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize);
void resize(RECT screenSize);
void changePiece(int type, RECT screenSize);
void render();
private:
};