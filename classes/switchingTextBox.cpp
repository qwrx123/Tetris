#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/switchingTextBox.h"


switchingTextBox::switchingTextBox(ID2D1HwndRenderTarget *renderTarget, block::location setLocation, RECT screenSize
    , IDWriteFactory *pDWriteFactory, const wchar_t myText[], scene::screen *outScreen, scene::screen changeTo, block::style myStyle)
    :clickableTextBox(renderTarget, setLocation, screenSize, pDWriteFactory, myText, myStyle), changeScene(outScreen), changeTo(changeTo)
{

}

void switchingTextBox::onClick(D2D1_POINT_2F clicked)
{
    if (clicked.x > drawnRect.left && clicked.x < drawnRect.right &&
        clicked.y > drawnRect.top && clicked.y < drawnRect.bottom)
    {
        *changeScene = changeTo;
    }
}