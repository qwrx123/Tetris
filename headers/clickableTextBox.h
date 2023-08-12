#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "textBox.h"

class clickableTextBox : public textBox
{
public:
protected:
bool clicked;
public:
clickableTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t myText[], block::style myStyle = block::boarder);
virtual void onCapture(D2D1_POINT_2F capturePoint);
virtual void onClick(D2D1_POINT_2F clicked);
virtual void onMove(D2D1_POINT_2F moved);
virtual void onKey(wchar_t inputChar);
virtual bool parseClick();
private:
};