#include "../headers/clickableTextBox.h"

clickableTextBox::clickableTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t myText[], block::style myStyle)
    :textBox(renderTarget, setLocation, screenSize, pDWriteFactory, myText, myStyle), clicked(false)
{

}

void clickableTextBox::onClick(D2D1_POINT_2F clicked)
{
    clickableTextBox::clicked = true;
}

bool clickableTextBox::parseClick()
{
    bool returnClick = clicked;
    clicked = false;
    return returnClick;
}

void clickableTextBox::onMove(D2D1_POINT_2F moved)
{
    if (moved.x > drawnRect.left && moved.x < drawnRect.right &&
        moved.y > drawnRect.top && moved.y < drawnRect.bottom)
    {
        this->block::changeColor(block::colors::gray);
    }
    else
    {
        this->block::changeColor(block::colors::white);
    }
}