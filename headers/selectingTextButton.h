#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "clickableTextBox.h"
#include "helpers.h"

class selectingTextButton : public clickableTextBox
{
public:
private:
textBox currentNumber;
clickableTextBox leftSelect;
clickableTextBox rightSelect;
int currentInt;
int maxInt;
int numDigets;
int* outNumber;
wchar_t* currentIntString;
public:
selectingTextButton(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
    IDWriteFactory* pDWriteFactory, const wchar_t myText[], int startingInt, int maxInt, int* trackInt, block::style myStyle = block::style::boarder);
~selectingTextButton();
virtual bool render();
virtual bool resize(RECT newScreen);
virtual void onClick(D2D1_POINT_2F clicked);
virtual void onMove(D2D1_POINT_2F moved);
private:
};