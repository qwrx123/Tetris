#include "../headers/block.h"


block::block(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize) 
	: pRenderTarget(renderTarget), currentColor(block::colors::white), screen(screenSize), coordinates(setLocation)
{
	for (int i = 0; i < block::colors::count; i++)
	{
		paintColor[i] = nullptr;
	}
	this->createColor(block::colors::black);
	this->createColor(block::colors::white);

	resetRect();
}

block::~block()
{
	for (int i = 0; i < block::colors::count; i++) {
		if (paintColor[i])
		{
			paintColor[i]->Release();
		}
	}
}
bool block::changeColor(block::colors newColor) {
	bool succeeded = true;
	if (!paintColor[newColor])
	{
		succeeded = createColor(newColor);
	}
	currentColor = newColor;
	return succeeded;
}
bool block::resize(RECT newScreen) {
	screen = newScreen;
	resetRect();
	return true;
}

void block::resetRect() {
	drawnRect.left = screen.right * coordinates.left;
	drawnRect.top = screen.bottom * coordinates.top;
	drawnRect.right = drawnRect.left + screen.right * coordinates.width;
	drawnRect.bottom = drawnRect.top + screen.bottom * coordinates.height;
}

bool block::render() {

	pRenderTarget->FillRectangle(
		drawnRect,
		paintColor[currentColor]
	);

    pRenderTarget->DrawRectangle(
	drawnRect,
	paintColor[colors::black],
	2.0,
	NULL);

	return true;
}

bool block::createColor(block::colors createColor)
{
	if (paintColor[createColor])
	{
		return false;
	}
	
	switch (createColor)
	{
		case block::colors::black:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &paintColor[createColor]);
			break;
		}
		case block::colors::white:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &paintColor[createColor]);
			break;
		}
		case block::colors::gray:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &paintColor[createColor]);
			break;
		}
		case block::colors::green:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &paintColor[createColor]);
			break;
		}
		case block::colors::red:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &paintColor[createColor]);
			break;
		}
		case block::colors::blue:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &paintColor[createColor]);
			break;
		}
		case block::colors::yellow:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &paintColor[createColor]);
			break;
		}
		case block::colors::cyan:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cyan), &paintColor[createColor]);
			break;
		}
		case block::colors::purple:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple), &paintColor[createColor]);
			break;
		}
		case block::colors::orange:
		{
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkOrange), &paintColor[createColor]);
			break;
		}
		default:
		{
			break;
		}

	}
	return true;
}