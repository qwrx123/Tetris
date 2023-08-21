#include "../headers/warningTextButton.h"


const wchar_t* warningTextBox::warningText[2] = {L"h", L"Are You Sure"};

warningTextBox::warningTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t* myText, bool* outWarned, block::style myStyle)
    : cyclingTextBox(renderTarget, setLocation, screenSize, pDWriteFactory, warningText, 2, &extendCycle, 0, myStyle),
        warned(outWarned), extendCycle(0)
{
    textBox::changeText(myText);
    cyclingTextBox::myText[0] = myText;
    cyclingTextBox::stringLengths[0] = wcsnlen(myText, 1000);
    cyclingTextBox::pTextLayout[0] = textBox::pTextLayout;
}

void warningTextBox::onClick(D2D1_POINT_2F clicked)
{
    if (clicked.x > drawnRect.left && clicked.x < drawnRect.right &&
        clicked.y > drawnRect.top && clicked.y < drawnRect.bottom)
    {
        if (currentCycle == 1)
        {
            *warned = true;
        }
    }
    else
    {
        if (currentCycle == 1)
        {
            cyclingTextBox::cycleTextBox();
            cyclingTextBox::onMove(clicked);
        }
    }
    cyclingTextBox::onClick(clicked);
}


void warningTextBox::onMove(D2D1_POINT_2F moved)
{
    if (currentCycle == 0)
    {
        cyclingTextBox::onMove(moved);
    }

}