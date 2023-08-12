#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "clickableTextBox.h"

class inputTextBox : public clickableTextBox
{
public:
private:
const static wchar_t defaultName[];
const static int defaultNameSize;
int inputLocation;
wchar_t* currentName;
int maxCharacters;
public:
inputTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
    IDWriteFactory* pDWriteFactory, wchar_t myText[], int bufferSize, block::style myStyle = block::boarder);
	virtual void onClick(D2D1_POINT_2F clicked);
	virtual void onMove(D2D1_POINT_2F moved);
	virtual void onKey(wchar_t inputChar);
private:
};