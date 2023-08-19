#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include "block.h"

class tetrisBlock : public block
{
public:
private:
public:
tetrisBlock(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, block::style myStyle = block::style::boarder);
void resize(block::location newLocation, RECT screenSize);
bool changeColor(int translateColor);
private:
};