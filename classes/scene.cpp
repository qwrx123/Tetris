#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/scene.h"
#include "../headers/block.h"
#include "../headers/textBox.h"
#include "../headers/switchingTextBox.h"
#include "../headers/cyclingTextBox.h"
#include "../headers/scalingTextButton.h"
#include "../headers/warningTextButton.h"
#include "../headers/selectingTextButton.h"

scene::scene(HWND hwnd)
	:renderSize(0), renderables(NULL), whenChange(scene::screen::invalid), willDelete(false)
{
    parentHwnd = hwnd;

	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory)
	);
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

    if (SUCCEEDED(hr))
    {
        RECT rc;
        GetClientRect(parentHwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(), 
            D2D1::HwndRenderTargetProperties(parentHwnd, size),
            &pRenderTarget);

		changeScene(scene::screen::Title);
    }
}

scene::~scene()
{
    pD2DFactory->Release();
    pRenderTarget->Release();
	clearScene();
}

bool scene::render()
{
    pRenderTarget->BeginDraw();

	for (int i = 0; i < renderSize; i++) {
		renderables[i]->render();
	}
    pRenderTarget->EndDraw();
	checkScene();
    return true;
}

bool scene::resize(RECT size)
{
    D2D1_SIZE_U newSize = D2D1::SizeU(size.right, size.bottom);
	for (int i = 0; i < renderSize; i++) {
		renderables[i]->resize(size);
	}
    return SUCCEEDED(pRenderTarget->Resize(newSize));
}

bool scene::changeScene(scene::screen newScreen)
{
	if (renderables)
	{
		clearScene();
	}
	RECT rc;
    GetClientRect(parentHwnd, &rc);

	switch (newScreen)
	{
		case scene::screen::Title:
		{
			renderSize = 32;
			renderables = new block*[renderSize];

			clickableSize = 7;
			clickables = new clickableTextBox*[clickableSize];

			renderables[0] = new block(pRenderTarget, block::location{0.0, 0.0, 1.0, 1.0}, rc, block::style::boarderless);
			renderables[1] = new textBox(pRenderTarget, block::location{0.05, 0.3, 0.4, 0.15}, rc, pDWriteFactory, L"Tetris");
			//leaderboard
			renderables[2] = new textBox(pRenderTarget, block::location{0.275, 0.35, 0.3, 0.1}, rc, pDWriteFactory, L"Top Scores");
			for (int i = 0; i < 5; i++) {
				renderables[2*i + 3] = new textBox(pRenderTarget, block::location{(float)(0.4 + 0.05*i), 0.25, 0.25, 0.05}, rc, pDWriteFactory, L"Player", block::style::boarderless);
				renderables[2*i + 4] = new textBox(pRenderTarget, block::location{(float)(0.4 + 0.05*i), 0.5, 0.25, 0.05}, rc, pDWriteFactory, L"0", block::style::boarderless);
			}
			renderables[13] = new block(pRenderTarget, block::location{0.4, 0.26, 0.5, 0.25}, rc, block::style::onlyBoarder);
			//endLeaderboard
			clickables[0] = new switchingTextBox(pRenderTarget, block::location{0.7, 0.4, 0.2, 0.1}, rc, pDWriteFactory, L"Play"
				, &whenChange, scene::screen::Game);
			renderables[14] = (block*)clickables[0];
			renderables[15] = new textBox(pRenderTarget, block::location{0.85, 0.25, 0.5, 0.1}, rc, pDWriteFactory, L"Player");
			//levelSelect
			clickables[1] = new selectingTextButton(pRenderTarget, block::location{0.2, 0.05, 0.15, 0.15}, rc, pDWriteFactory, L"Level Select", 3, 19, &difficulty);
			renderables[16] = clickables[1];

			renderables[17] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);
			renderables[18] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);
			renderables[19] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);
			//renderables[16] = new textBox(pRenderTarget, block::location{0.2, 0.05, 0.15, 0.05}, rc, pDWriteFactory, L"Level Select");
			//renderables[17] = new textBox(pRenderTarget, block::location{0.25, 0.05, 0.15, 0.05}, rc, pDWriteFactory, L"0");
			//renderables[18] = new textBox(pRenderTarget, block::location{0.3, 0.05, 0.075, 0.05}, rc, pDWriteFactory, L"<-");
			//renderables[19] = new textBox(pRenderTarget, block::location{0.3, 0.125, 0.075, 0.05}, rc, pDWriteFactory, L"->");
			//endLevelSelect
			//deleteLeaderboard
			clickables[2] = new warningTextBox(pRenderTarget, block::location{0.2, 0.8, 0.15, 0.15}, rc, pDWriteFactory, L"Delete\nLeaderboard", &willDelete);
			renderables[20] = clickables[2];
			//renderables[20] = new textBox(pRenderTarget, block::location{0.2, 0.8, 0.15, 0.15}, rc, pDWriteFactory, L"Delete\nLeaderboard");

			renderables[21] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);
			renderables[22] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);
			//renderables[20] = new textBox(pRenderTarget, block::location{0.2, 0.8, 0.15, 0.075}, rc, pDWriteFactory, L"Delete", block::style::boarderless);
			//renderables[21] = new textBox(pRenderTarget, block::location{0.275, 0.8, 0.15, 0.075}, rc, pDWriteFactory, L"Leaderboard", block::style::boarderless);
			//renderables[22] = new block(pRenderTarget, block::location{0.2, 0.8, 0.15, 0.15}, rc, block::style::onlyBoarder);
			//endDeleteLeaderboard
			//volumeAdjustment
			clickables[3] = new scalingTextButton(pRenderTarget, block::location{0.4, 0.05, 0.15, 0.2}, rc, pDWriteFactory, L"Sound Volume", &volumeScale, 25);
			renderables[23] = clickables[3];
			
			renderables[24] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);
			renderables[25] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);
			/*
			renderables[23] = new textBox(pRenderTarget, block::location{0.4, 0.05, 0.15, 0.1}, rc, pDWriteFactory, L"Sound Volume");
			renderables[24] = new textBox(pRenderTarget, block::location{0.5, 0.05, 0.15, 0.05}, rc, pDWriteFactory, L"100%");
			renderables[25] = new block(pRenderTarget, block::location{0.55, 0.05, 0.15, 0.05}, rc);
			*/
			//effectAdjustment
			clickables[4] = new scalingTextButton(pRenderTarget, block::location{0.6, 0.05, 0.15, 0.2}, rc, pDWriteFactory, L"Effect Volume", &volumeScale, 25);
			renderables[26] = clickables[4];
			
			renderables[27] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);
			renderables[28] = new block(pRenderTarget, block::location{0.0, 0.0, 0.0, 0.0}, rc, block::boarderless);

			//renderables[26] = new textBox(pRenderTarget, block::location{0.6, 0.05, 0.15, 0.1}, rc, pDWriteFactory, L"Effect Volume");
			//renderables[27] = new textBox(pRenderTarget, block::location{0.7, 0.05, 0.15, 0.05}, rc, pDWriteFactory, L"100%");
			//renderables[28] = new block(pRenderTarget, block::location{0.75, 0.05, 0.15, 0.05}, rc);
			//changeSong
			renderables[29] = new textBox(pRenderTarget, block::location{0.5, 0.8, 0.15, 0.1}, rc, pDWriteFactory, L"Change Song");
			const wchar_t* boxList[3] = {L"Number 1", L"Number 2", L"Number 3"};
			clickables[5] = new cyclingTextBox(pRenderTarget, block::location{0.6, 0.8, 0.15, 0.05}, rc, pDWriteFactory, boxList, ARRAYSIZE(boxList), &songNumber);
			renderables[30] = clickables[5];
			//controlButton
			clickables[6] = new switchingTextBox(pRenderTarget, block::location{0.7, 0.8, 0.15, 0.1}, rc, pDWriteFactory, L"Controls",
				&whenChange, scene::screen::Control);
			renderables[31] = clickables[6];

			resize(rc);
			break;
		}
		case scene::screen::Control:
		{
			renderSize = 6;
			renderables = new block*[renderSize];

			clickableSize = 1;
			clickables = new clickableTextBox*[clickableSize];

			renderables[0] = new block(pRenderTarget, block::location{0.0, 0.0, 1.0, 1.0}, rc, block::style::boarderless);
			renderables[1] = new textBox(pRenderTarget, block::location{0.05, 0.3, 0.4, 0.15}, rc, pDWriteFactory, L"Tetris");
			renderables[2] = new textBox(pRenderTarget, block::location{0.275, 0.35, 0.3, 0.1}, rc, pDWriteFactory, L"Controls");
			renderables[3] = new textBox(pRenderTarget, block::location{0.4, 0.25, 0.5, 0.35}, rc, pDWriteFactory, L"Rotate block - W or ↑\nMove Left - A or ←\nMove Right - D or →\nSoft Drop - S or ↓\nHard Drop - Space\nHold Block - F\nPause Game - P", block::boarderless);
			/*
			renderables[4] = new textBox(pRenderTarget, block::location{0.45, 0.25, 0.5, 0.05}, rc, pDWriteFactory, L"Move Left - A or ←", block::boarderless);
			renderables[5] = new textBox(pRenderTarget, block::location{0.5, 0.25, 0.5, 0.05}, rc, pDWriteFactory, L"Move Right - D or →", block::boarderless);
			renderables[6] = new textBox(pRenderTarget, block::location{0.55, 0.25, 0.5, 0.05}, rc, pDWriteFactory, L"Soft Drop - S or ↓", block::boarderless);
			renderables[7] = new textBox(pRenderTarget, block::location{0.6, 0.25, 0.5, 0.05}, rc, pDWriteFactory, L"Hard Drop - Space", block::boarderless);
			renderables[8] = new textBox(pRenderTarget, block::location{0.65, 0.25, 0.5, 0.05}, rc, pDWriteFactory, L"Hold Block - F", block::boarderless);
			renderables[9] = new textBox(pRenderTarget, block::location{0.7, 0.25, 0.5, 0.05}, rc, pDWriteFactory, L"Pause Game - P", block::boarderless);
			*/
			renderables[4] = new block(pRenderTarget, block::location{0.4, 0.25, 0.5, 0.35}, rc, block::style::onlyBoarder);

			clickables[0] = new switchingTextBox(pRenderTarget, block::location{0.8, 0.4, 0.2, 0.1}, rc, pDWriteFactory, L"Back", &whenChange, scene::screen::Title);
			renderables[5] = clickables[0];
			resize(rc);
		}
		default:
		break;
	}
	return true;
}

bool scene::clearScene()
{
	for (int i = 0; i < renderSize; i++) {
		delete renderables[i];
	}
	delete[] renderables;
	renderables = NULL;
	renderSize = 0;
	delete[] clickables;
	clickables = NULL;
	clickableSize = 0;
	return true;
}

void scene::onMove(D2D1_POINT_2F moved)
{
	if (!clickables)
	{
		return;
	}

	for (int i = 0; i < clickableSize; i++)
	{
		clickables[i]->onMove(moved);
	}
}

void scene::onClick(D2D1_POINT_2F clicked)
{
	if (!clickables)
	{
		return;
	}

	for (int i = 0; i < clickableSize; i++)
	{
		clickables[i]->onClick(clicked);
	}
}

void scene::onCapture(D2D1_POINT_2F capture)
{
	if (!clickables)
	{
		return;
	}

	for (int i = 0; i < clickableSize; i++)
	{
		clickables[i]->onCapture(capture);
	}
}

void scene::checkScene()
{
	if (whenChange == scene::screen::invalid)
	{
		return;
	}

	changeScene(whenChange);

	whenChange = scene::screen::invalid;
}