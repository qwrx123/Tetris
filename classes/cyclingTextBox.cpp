#include "../headers/cyclingTextBox.h"
#include <iostream>

cyclingTextBox::cyclingTextBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
        IDWriteFactory* pDWriteFactory, const wchar_t** myText, int arraySize, int* outCycleNumber
        , block::style myStyle)
    :clickableTextBox(renderTarget, setLocation, screenSize, pDWriteFactory, myText[0], myStyle), 
		pTextLayout(new IDWriteTextLayout*[arraySize]), myText(new const wchar_t*[arraySize]), stringLengths(new int[arraySize]),
		arraySize(arraySize), currentCycle(0)
{
    if (arraySize > 0)
	{
		this->pTextLayout[0] = textBox::pTextLayout;
	}

	for (int i = 0; i < arraySize; i++)
	{
		this->myText[i] = myText[i];
		stringLengths[i] = wcsnlen(myText[i], 1000);
	}

	for (int i = 1; i < arraySize; i++)
	{
		pDWriteFactory->CreateTextLayout(
			myText[i],
			stringLengths[i],
			pTextFormat,
			screenSize.right * coordinates.width,
			screenSize.bottom * coordinates.height,
			&pTextLayout[i]
			);
	}
}

cyclingTextBox::~cyclingTextBox()
{
	for (int i = 0; i < arraySize; i++)
	{
		pTextLayout[i]->Release();
	}
	textBox::pTextLayout = NULL;
	delete[] pTextLayout;

	delete[] myText;
	delete[] stringLengths;
}

void cyclingTextBox::cycleTextBox()
{
	currentCycle = (currentCycle+1)%arraySize;

	textBox::pTextLayout = pTextLayout[currentCycle];
	textBox::boxText = myText[currentCycle];
	textBox::stringLength = stringLengths[currentCycle];
	textSizeFit();
}

void cyclingTextBox::onClick(D2D_POINT_2F clicked)
{
    if (clicked.x > drawnRect.left && clicked.x < drawnRect.right &&
        clicked.y > drawnRect.top && clicked.y < drawnRect.bottom)
    {
        cycleTextBox();
    }
}

bool cyclingTextBox::resize(RECT newScreen)
{
	textBox::resize(newScreen);
	for (int i = 0; i < arraySize; i++)
	{
		pTextLayout[i]->SetMaxWidth(screen.right * coordinates.width);
		pTextLayout[i]->SetMaxHeight(screen.bottom * coordinates.height);
	}
	return true;
}