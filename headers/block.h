#pragma once

#include <d2d1.h>
#include <d2d1helper.h>

#pragma comment(lib, "d2d1.lib")

class block
{
public:
	enum colors
	{
		white,
		black,
		gray,
		green,
		blue,
		red,
		yellow,
		cyan,
		purple,
		orange,
		count
	};
	enum style
	{
		boarder,
		boarderless,
		onlyBoarder
	};
	struct location
	{
		float top;
		float left;
		float width;
		float height;
	};
	
protected:
	ID2D1HwndRenderTarget* pRenderTarget;
	block::colors currentColor;
	ID2D1SolidColorBrush* paintColor[colors::count];
	block::location coordinates;
	block::style myStyle;
	RECT screen;
	D2D1_RECT_F drawnRect;
public:
	block(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, block::style myStyle = block::style::boarder);
	~block();
	bool changeColor(block::colors);
	virtual bool resize(RECT newScreen);
	virtual bool render();
protected: 
	void resetRect();
	bool createColor(block::colors createColor);
};