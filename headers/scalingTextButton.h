#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "clickableTextBox.h"

class scalingTextButton : public clickableTextBox
{
public:
private:
wchar_t percentText[5];
block::location coordinates;
RECT screen;
D2D1_RECT_F drawnRect;
int *outScalar;
int inScalar;
textBox upperBlock;
block lowerBlock;
D2D1_POINT_2F linePoints[2];
D2D1_POINT_2F circlePoint;
D2D1_ELLIPSE scrollingEllipse;
bool captured;
public:
scalingTextButton(ID2D1HwndRenderTarget *renderTarget, block::location setLocation, RECT screenSize
    , IDWriteFactory *pDWriteFactory, wchar_t myText[], int* outScalar, int scale,  block::style myStyle = block::boarder);
bool render();
bool scalingTextButton::resize(RECT newScreen);
virtual void onCapture(D2D1_POINT_2F capturePoint);
virtual void onClick(D2D1_POINT_2F clicked);
virtual void onMove(D2D1_POINT_2F moved);
private:
void resetRect();
};