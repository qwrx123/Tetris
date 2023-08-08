#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "block.h"

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

class textBox : public block
{
public:
protected:
static const WCHAR msc_fontName[8];
IDWriteTextFormat* pTextFormat;
IDWriteTextLayout *pTextLayout;
const wchar_t* boxText;
int stringLength;
D2D1_POINT_2F origin;
public:
    textBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t myText[], block::style myStyle = block::boarder);
    ~textBox();
	virtual bool render();
    virtual bool resize(RECT newScreen);
protected:
    bool textSizeFit();
};