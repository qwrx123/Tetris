#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "cyclingTextBox.h"



class warningTextBox : public cyclingTextBox
{
public:
private:
int extendCycle;
bool* warned;
const static wchar_t* warningText[2];
public:
    warningTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t* myText, bool* outWarned, block::style myStyle = block::boarder);
	void onClick(D2D1_POINT_2F clicked);
	void onMove(D2D1_POINT_2F moved);
private:
};
