#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/scalingTextButton.h"
#include "../headers/helpers.h"

scalingTextButton::scalingTextButton(ID2D1HwndRenderTarget *renderTarget, block::location setLocation, RECT screenSize
    , IDWriteFactory *pDWriteFactory, wchar_t myText[], int* outScalar, int scale, block::style myStyle)
    :clickableTextBox(renderTarget, {setLocation.top, setLocation.left, setLocation.width, setLocation.height/2}
    , screenSize, pDWriteFactory, myText, myStyle), 
    upperBlock(renderTarget, {setLocation.top + setLocation.height/2, setLocation.left, setLocation.width, setLocation.height/4}, screenSize
        , pDWriteFactory, L"100%"), captured(false),
    lowerBlock(renderTarget, coordinates, screenSize)
    ,outScalar(outScalar), inScalar(scale), coordinates({setLocation.top + (setLocation.height/4)*3, setLocation.left, setLocation.width, setLocation.height/4})
{
    helper::intToTextPercent(percentText, 4, scale);
    upperBlock.changeText(percentText);
    this->createColor(block::colors::gray);
}

void scalingTextButton::resetRect() {
	drawnRect.left = screen.right * coordinates.left;
	drawnRect.top = screen.bottom * coordinates.top;
	drawnRect.right = drawnRect.left + screen.right * coordinates.width;
	drawnRect.bottom = drawnRect.top + screen.bottom * coordinates.height;
}

bool scalingTextButton::render()
{
    clickableTextBox::render();
    upperBlock.render();
    lowerBlock.render();

    pRenderTarget->DrawLine(linePoints[0], linePoints[1], block::paintColor[block::colors::black], 2.0);

    pRenderTarget->FillEllipse(scrollingEllipse, block::paintColor[block::colors::gray]);
    pRenderTarget->DrawEllipse(scrollingEllipse, block::paintColor[block::colors::black], 2.0);
    return true;
}

bool scalingTextButton::resize(RECT newScreen)
{
    clickableTextBox::resize(newScreen);
	this->screen = newScreen;
	resetRect();
    upperBlock.resize(newScreen);
    lowerBlock.resize(newScreen);

    float halfBoxHeight = (drawnRect.bottom - drawnRect.top)/2;
    float margin = halfBoxHeight - screen.bottom*0.005;

    scrollingEllipse.radiusX = margin;
    scrollingEllipse.radiusY = margin;

    linePoints[0] = {drawnRect.left + halfBoxHeight, drawnRect.top + halfBoxHeight};
    linePoints[1] = {drawnRect.right - halfBoxHeight, linePoints[0].y};

    float lineLength = linePoints[1].x - linePoints[0].x;
    scrollingEllipse.point = {drawnRect.left + halfBoxHeight + lineLength * ((float)inScalar/100.0f), linePoints[0].y};

    return true;
}

void scalingTextButton::onCapture(D2D1_POINT_2F capturePoint)
{
    if (capturePoint.x > drawnRect.left && capturePoint.x < drawnRect.right &&
        capturePoint.y > drawnRect.top && capturePoint.y < drawnRect.bottom)
    {
        captured = true;
        this->onMove(capturePoint);
    }
}

void scalingTextButton::onClick(D2D1_POINT_2F clicked)
{
    captured = false;
}

void scalingTextButton::onMove(D2D1_POINT_2F moved)
{
    if (!captured)
    {
        return;
    }
    if (moved.x <= linePoints[0].x)
    {
        inScalar = 0;
    }
    else if (moved.x >= linePoints[1].x)
    {
        inScalar = 100;
    }
    else
    {
        float lineLength = linePoints[1].x - linePoints[0].x;
        float mousePercent = moved.x - linePoints[0].x;
        float tempScale = mousePercent/lineLength;
        tempScale *= 100;
        inScalar = (int)tempScale;
    }

    helper::intToTextPercent(percentText, 4, inScalar);
    upperBlock.changeText(percentText);
    this->resize(screen);
}