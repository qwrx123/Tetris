#include "../headers/inputTextBox.h"
#include <stdexcept>

const wchar_t inputTextBox::defaultName[] = L"Player";
const int inputTextBox::defaultNameSize = sizeof(inputTextBox::defaultName)/sizeof(wchar_t);

inputTextBox::inputTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
    IDWriteFactory* pDWriteFactory, wchar_t myText[], int bufferSize, block::style myStyle)
    : clickableTextBox(renderTarget, setLocation, screenSize, pDWriteFactory, myText, myStyle), inputLocation(0), 
    currentName(myText), maxCharacters(bufferSize-1)
{
    if (bufferSize < defaultNameSize)
    {
        throw std::invalid_argument("Buffer Size Too Small");
    }


}

void inputTextBox::onClick(D2D1_POINT_2F clicked)
{
    if (clicked.x > drawnRect.left && clicked.x < drawnRect.right &&
        clicked.y > drawnRect.top && clicked.y < drawnRect.bottom)
    {
        clickableTextBox::clicked = true;
        inputLocation = 0;
        for (int i = 0; i < maxCharacters; i++)
        {
            currentName[i] = L'\0';
        }
        textBox::changeText(currentName);
    }
    else 
    {
        clickableTextBox::clicked = false;
        if (currentName[0] == L'\0')
        {
            for (int i = 0; i < defaultNameSize; i++)
            {
                currentName[i] = defaultName[i];
            }
        }
        changeText(currentName);
        onMove(clicked);
    }
}

void inputTextBox::onMove(D2D1_POINT_2F moved)
{
    if (clicked == false)
    {
        clickableTextBox::onMove(moved);
    }
}

void inputTextBox::onKey(wchar_t inputChar)
{
    if (!clicked)
    {
        return;
    }

    if (inputChar == L'\n' || inputChar == L'\r')
    {
        onClick({-1.0, -1.0});
        textBox::changeText(currentName);
        return;
    }

    if (inputChar == L'\b')
    {
        if (inputLocation == 0)
        {
            return;
        }
        inputLocation--;
        currentName[inputLocation] = L'\0';
        textBox::changeText(currentName);
        return;
    }

    if (inputChar < L'!' || inputChar > L'~')
    {
        return;
    }

    currentName[inputLocation] = inputChar;
    inputLocation++;

    if (inputLocation == maxCharacters)
    {
        inputLocation--;
    }
    textBox::changeText(currentName);
}

