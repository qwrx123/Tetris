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
#include "../headers/inputTextBox.h"

scene::scene(HWND hwnd)
	:renderSize(0), renderables(NULL), whenChange(scene::screen::invalid), willDelete(false), tetrisGame(nullptr), gamePaused(false)
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

		for (int i = 0; i < 5; i++) 
		{
			wcsncpy(currentLeaderboard[i].name, L"Player", 20);
			currentLeaderboard[i].score = 0;
			wcsncpy(currentLeaderboard[i].scoreText, L"0", 10);
		}
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
	if (tetrisGame && !gamePaused)
	{
		if(tetrisGame->testDeath())
		{
			whenChange = scene::screen::Death;
		}
		tetrisGame->gameLoop();
		tetrisGame->render();
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
	if (tetrisGame)
	{
		tetrisGame->resize(size);
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
			renderSize = 23;
			renderables = new block*[renderSize];

			clickableSize = 8;
			clickables = new clickableTextBox*[clickableSize];

			renderables[0] = new block(pRenderTarget, block::location{0.0, 0.0, 1.0, 1.0}, rc, block::style::boarderless);
			renderables[1] = new textBox(pRenderTarget, block::location{0.05, 0.3, 0.4, 0.15}, rc, pDWriteFactory, L"Tetris");
			//leaderboard
			renderables[2] = new textBox(pRenderTarget, block::location{0.275, 0.35, 0.3, 0.1}, rc, pDWriteFactory, L"Top Scores");
			for (int i = 0; i < 5; i++) {
				renderables[2*i + 3] = new textBox(pRenderTarget, block::location{(float)(0.4 + 0.05*i), 0.25, 0.25, 0.05}, rc, pDWriteFactory, currentLeaderboard[i].name, block::style::boarderless);
				renderables[2*i + 4] = new textBox(pRenderTarget, block::location{(float)(0.4 + 0.05*i), 0.5, 0.25, 0.05}, rc, pDWriteFactory, currentLeaderboard[i].scoreText, block::style::boarderless);
			}
			renderables[13] = new block(pRenderTarget, block::location{0.4, 0.25, 0.5, 0.25}, rc, block::style::onlyBoarder);
			//endLeaderboard
			clickables[0] = new switchingTextBox(pRenderTarget, block::location{0.7, 0.4, 0.2, 0.1}, rc, pDWriteFactory, L"Play"
				, &whenChange, scene::screen::Game);
			renderables[14] = (block*)clickables[0];
			//player name button
			clickables[1] = new inputTextBox(pRenderTarget, block::location{0.85, 0.25, 0.5, 0.1}, rc, pDWriteFactory, playerName, playerNameSize);
			renderables[15] = clickables[1];
			//levelSelect
			clickables[2] = new selectingTextButton(pRenderTarget, block::location{0.2, 0.05, 0.15, 0.15}, rc, pDWriteFactory, L"Level Select", 3, 19, &difficulty);
			renderables[16] = clickables[2];
			//endLevelSelect
			//deleteLeaderboard
			clickables[3] = new warningTextBox(pRenderTarget, block::location{0.2, 0.8, 0.15, 0.15}, rc, pDWriteFactory, L"Delete\nLeaderboard", &willDelete);
			renderables[17] = clickables[3];
			//endDeleteLeaderboard
			//volumeAdjustment
			clickables[4] = new scalingTextButton(pRenderTarget, block::location{0.4, 0.05, 0.15, 0.2}, rc, pDWriteFactory, L"Sound Volume", &volumeScale, 25);
			renderables[18] = clickables[4];
			//effectAdjustment
			clickables[5] = new scalingTextButton(pRenderTarget, block::location{0.6, 0.05, 0.15, 0.2}, rc, pDWriteFactory, L"Effect Volume", &effectScale, 25);
			renderables[19] = clickables[5];
			//changeSong
			renderables[20] = new textBox(pRenderTarget, block::location{0.5, 0.8, 0.15, 0.1}, rc, pDWriteFactory, L"Change Song");
			const wchar_t* boxList[3] = {L"Number 1", L"Number 2", L"Number 3"};
			clickables[6] = new cyclingTextBox(pRenderTarget, block::location{0.6, 0.8, 0.15, 0.05}, rc, pDWriteFactory, boxList, ARRAYSIZE(boxList), &songNumber);
			renderables[21] = clickables[6];
			//controlButton
			clickables[7] = new switchingTextBox(pRenderTarget, block::location{0.7, 0.8, 0.15, 0.1}, rc, pDWriteFactory, L"Controls",
				&whenChange, scene::screen::Control);
			renderables[22] = clickables[7];
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
			renderables[4] = new block(pRenderTarget, block::location{0.4, 0.25, 0.5, 0.35}, rc, block::style::onlyBoarder);

			clickables[0] = new switchingTextBox(pRenderTarget, block::location{0.8, 0.4, 0.2, 0.1}, rc, pDWriteFactory, L"Back", &whenChange, scene::screen::Title);
			renderables[5] = clickables[0];
			resize(rc);
			break;
		}
		case scene::screen::Game:
		{
			renderSize = 3;
			renderables = new block*[renderSize];
			clickableSize = 0;
			clickables = new  clickableTextBox*[1];

			renderables[0] = new block(pRenderTarget, block::location{0.0, 0.0, 1.0, 1.0}, rc, block::style::boarderless);
			renderables[1] = new textBox(pRenderTarget, block::location{0.05, 0.3, 0.4, 0.15}, rc, pDWriteFactory, L"Tetris");
			renderables[2] = new block(pRenderTarget, block::location{0.21, 0.38, 0.24, 0.74}, rc);
			if (!tetrisGame)
			{
				tetrisGame = new game(pRenderTarget, pDWriteFactory, rc, playerName, difficulty, songNumber, volumeScale, effectScale);
			}
			gamePaused = false;
			resize(rc);
			break;
		}
		case scene::screen::Pause:
		{
			tetrisGame->populateMetrics(currentMetrics);
			renderSize = 12;
			renderables = new block*[renderSize];
			clickableSize = 1;
			clickables = new  clickableTextBox*[clickableSize];
			renderables[0] = new block(pRenderTarget, block::location{0.0, 0.0, 1.0, 1.0}, rc, block::style::boarderless);
			renderables[1] = new textBox(pRenderTarget, block::location{0.05, 0.3, 0.4, 0.15}, rc, pDWriteFactory, L"Tetris");
			renderables[2] = new textBox(pRenderTarget, block::location{0.275, 0.35, 0.3, 0.1}, rc, pDWriteFactory, L"Paused");
			renderables[3] = new block(pRenderTarget, block::location{0.4, 0.35, 0.3, 0.35}, rc);
			renderables[4] = new textBox(pRenderTarget, block::location{0.4, 0.35, 0.3, 0.05}, rc, pDWriteFactory, currentMetrics.name, block::style::boarderless);
			renderables[5] = new textBox(pRenderTarget, block::location{0.45, 0.35, 0.3, 0.05}, rc, pDWriteFactory, L"Score", block::style::boarderless);
			renderables[6] = new textBox(pRenderTarget, block::location{0.5, 0.35, 0.3, 0.05}, rc, pDWriteFactory, currentMetrics.scoreText, block::style::boarderless);
			renderables[7] = new textBox(pRenderTarget, block::location{0.55, 0.35, 0.3, 0.05}, rc, pDWriteFactory, L"Level", block::style::boarderless);
			renderables[8] = new textBox(pRenderTarget, block::location{0.6, 0.35, 0.3, 0.05}, rc, pDWriteFactory, currentMetrics.levelText, block::style::boarderless);
			renderables[9] = new textBox(pRenderTarget, block::location{0.65, 0.35, 0.3, 0.05}, rc, pDWriteFactory, L"Lines", block::style::boarderless);
			renderables[10] = new textBox(pRenderTarget, block::location{0.7, 0.35, 0.3, 0.05}, rc, pDWriteFactory, currentMetrics.linesText, block::style::boarderless);
			clickables[0] = new switchingTextBox(pRenderTarget, block::location{0.8, 0.4, 0.2, 0.1}, rc, pDWriteFactory, L"Back", &whenChange, scene::screen::Game);
			renderables[11] = clickables[0];
			resize(rc);
			break;
		}
		case scene::screen::Death:
		{
			tetrisGame->populateMetrics(currentMetrics);
			calculateLeaderboard();
			delete tetrisGame;
			tetrisGame = nullptr;
			renderSize = 12;
			renderables = new block*[renderSize];
			clickableSize = 1;
			clickables = new  clickableTextBox*[clickableSize];
			renderables[0] = new block(pRenderTarget, block::location{0.0, 0.0, 1.0, 1.0}, rc, block::style::boarderless);
			renderables[1] = new textBox(pRenderTarget, block::location{0.05, 0.3, 0.4, 0.15}, rc, pDWriteFactory, L"Tetris");
			renderables[2] = new textBox(pRenderTarget, block::location{0.275, 0.35, 0.3, 0.1}, rc, pDWriteFactory, L"You Died");
			renderables[3] = new block(pRenderTarget, block::location{0.4, 0.35, 0.3, 0.35}, rc);
			renderables[4] = new textBox(pRenderTarget, block::location{0.4, 0.35, 0.3, 0.05}, rc, pDWriteFactory, currentMetrics.name, block::style::boarderless);
			renderables[5] = new textBox(pRenderTarget, block::location{0.45, 0.35, 0.3, 0.05}, rc, pDWriteFactory, L"Score", block::style::boarderless);
			renderables[6] = new textBox(pRenderTarget, block::location{0.5, 0.35, 0.3, 0.05}, rc, pDWriteFactory, currentMetrics.scoreText, block::style::boarderless);
			renderables[7] = new textBox(pRenderTarget, block::location{0.55, 0.35, 0.3, 0.05}, rc, pDWriteFactory, L"Level", block::style::boarderless);
			renderables[8] = new textBox(pRenderTarget, block::location{0.6, 0.35, 0.3, 0.05}, rc, pDWriteFactory, currentMetrics.levelText, block::style::boarderless);
			renderables[9] = new textBox(pRenderTarget, block::location{0.65, 0.35, 0.3, 0.05}, rc, pDWriteFactory, L"Lines", block::style::boarderless);
			renderables[10] = new textBox(pRenderTarget, block::location{0.7, 0.35, 0.3, 0.05}, rc, pDWriteFactory, currentMetrics.linesText, block::style::boarderless);
			clickables[0] = new switchingTextBox(pRenderTarget, block::location{0.8, 0.4, 0.2, 0.1}, rc, pDWriteFactory, L"Back", &whenChange, scene::screen::Title);
			renderables[11] = clickables[0];
			resize(rc);
			break;
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

void scene::onKey(wchar_t inputChar)
{
	if ((wchar_t)inputChar == L'P' || (wchar_t)inputChar == L'p')
	{
		gamePaused = true;
		whenChange = scene::screen::Pause;
	}
	if (!clickables)
	{
		return;
	}

	for (int i = 0; i < clickableSize; i++)
	{
		clickables[i]->onKey(inputChar);
	}
}

void scene::keyUp(wchar_t inputChar)
{
	if (tetrisGame && !gamePaused)
	{
		tetrisGame->onKey(inputChar);
	}
}

void scene::keyDown(wchar_t inputChar)
{
	if (tetrisGame && !gamePaused)
	{
		tetrisGame->keyDown(inputChar);
	}
}

void scene::calculateLeaderboard() {
	int pos;
	bool isBigger = false;
	for (int i = 4; i >= 0; i--) {
		if (currentMetrics.score > currentLeaderboard[i].score) {
     			isBigger = true;
			pos = i;
		}
	}
		if (isBigger == true) {
		for (int i = 3; i >= pos; i--) {
			currentLeaderboard[i + 1].score = currentLeaderboard[i].score;
			wcsncpy(currentLeaderboard[i+1].name, currentLeaderboard[i].name, 20);
			wcsncpy(currentLeaderboard[i+1].scoreText, currentLeaderboard[i].scoreText, 10);
		}
		currentLeaderboard[pos].score = currentMetrics.score;
		wcsncpy(currentLeaderboard[pos].scoreText, currentMetrics.scoreText, 10);
		wcsncpy(currentLeaderboard[pos].name, currentMetrics.name, 20);
	}
	//saveLeaderboard();
}