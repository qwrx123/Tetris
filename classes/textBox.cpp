#include "../headers/textBox.h"

const WCHAR textBox::msc_fontName[8] = L"Verdana";

textBox::textBox(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, 
    IDWriteFactory* pDWriteFactory, const wchar_t myText[], block::style myStyle)
: block(renderTarget, setLocation, screenSize, myStyle), boxText(myText), stringLength(wcsnlen(myText, 100)),
origin(D2D1_POINT_2F{screenSize.right * coordinates.left, screenSize.bottom * coordinates.top})
{
    
    pDWriteFactory->CreateTextFormat(
        msc_fontName,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		1.0,
		L"", //locale
		&pTextFormat
    );

    pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    pDWriteFactory->CreateTextLayout(
        boxText,
        stringLength,
        pTextFormat,
        screenSize.right * coordinates.width,
        screenSize.bottom * coordinates.height,
        &pTextLayout
        );
}

textBox::~textBox()
{
    pTextFormat->Release();
    pTextLayout->Release();
}

bool textBox::render()
{
    this->block::render();
    /*
    pRenderTarget->DrawText(
        boxText,
        stringLength,
        pTextFormat,
        drawnRect,
        paintColor[colors::black]
    );
    */
    pRenderTarget->DrawTextLayout(
        origin,
        pTextLayout,
        paintColor[block::colors::black]
    );
    return true;
}

bool textBox::resize(RECT newScreen)
{
    this->block::resize(newScreen);

    pTextLayout->SetMaxWidth(screen.right * coordinates.width);
    pTextLayout->SetMaxHeight(screen.bottom * coordinates.height);

    origin.x = screen.right * coordinates.left;
    origin.y = screen.bottom * coordinates.top;

    this->textSizeFit();
    return true;
}

bool textBox::textSizeFit()
{
    DWRITE_TEXT_RANGE fullRange = {0, (u_int)stringLength};
    
    DWRITE_TEXT_METRICS metrics;
    pTextLayout->GetMetrics(&metrics);
    float margin = metrics.layoutHeight*0.1;

    DWRITE_OVERHANG_METRICS overhangs;
    pTextLayout->GetOverhangMetrics(&overhangs);

    float fontSize;

    while (overhangs.left > -margin || overhangs.right > -margin ||
        overhangs.top > -margin || overhangs.bottom > -margin)
    {
        pTextLayout->GetFontSize(0, &fontSize);
        pTextLayout->SetFontSize(fontSize - 1, fullRange);
        pTextLayout->GetOverhangMetrics(&overhangs);
    }

    while (overhangs.left < -margin && overhangs.right < -margin &&
        overhangs.top < -margin && overhangs.bottom < -margin)
    {
        pTextLayout->GetFontSize(0, &fontSize);
        pTextLayout->SetFontSize(fontSize + 0.1, fullRange);
        pTextLayout->GetOverhangMetrics(&overhangs);
    }

    return true;
}