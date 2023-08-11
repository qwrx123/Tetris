#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "clickableTextBox.h"
#include "scene.h"

class cyclingTextBox : public clickableTextBox
{
public:
protected:
scene::screen* changeScene;
scene::screen changeTo;
IDWriteTextLayout **pTextLayout;
const wchar_t** myText;
int* stringLengths;
int arraySize;
int currentCycle;
public:
cyclingTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t** myText, int arraySize, int* outCycleNumber
        , block::style myStyle = block::boarder);
~cyclingTextBox();
virtual void onClick(D2D1_POINT_2F clicked);
virtual bool resize(RECT newScreen);
protected:
void cycleTextBox();
};