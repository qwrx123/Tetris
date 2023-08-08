#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include "clickableTextBox.h"
#include "scene.h"

class switchingTextBox : public clickableTextBox
{
public:
private:
scene::screen* changeScene;
scene::screen changeTo;
public:
switchingTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t myText[], scene::screen* outScreen, scene::screen changeTo
        , block::style myStyle = block::boarder);
virtual void onClick(D2D1_POINT_2F clicked);
private:
};