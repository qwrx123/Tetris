#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/scene.h"
#include "../headers/block.h"
#include "../headers/textBox.h"

scene::scene(HWND hwnd)
	:sceneSize(0), renderables(NULL)
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

	for (int i = 0; i < sceneSize; i++) {
		renderables[i]->render();
	}
    pRenderTarget->EndDraw();
    return true;
}

bool scene::resize(RECT size)
{
    D2D1_SIZE_U newSize = D2D1::SizeU(size.right, size.bottom);
	for (int i = 0; i < sceneSize; i++) {
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
			renderables = new block*[32];
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
			renderables[14] = new textBox(pRenderTarget, block::location{0.7, 0.4, 0.2, 0.1}, rc, pDWriteFactory, L"Play");
			renderables[15] = new textBox(pRenderTarget, block::location{0.85, 0.25, 0.5, 0.1}, rc, pDWriteFactory, L"Player");
			//levelSelect
			renderables[16] = new textBox(pRenderTarget, block::location{0.2, 0.05, 0.15, 0.05}, rc, pDWriteFactory, L"Level Select");
			renderables[17] = new textBox(pRenderTarget, block::location{0.25, 0.05, 0.15, 0.05}, rc, pDWriteFactory, L"0");
			renderables[18] = new textBox(pRenderTarget, block::location{0.3, 0.05, 0.075, 0.05}, rc, pDWriteFactory, L"<-");
			renderables[19] = new textBox(pRenderTarget, block::location{0.3, 0.125, 0.075, 0.05}, rc, pDWriteFactory, L"->");
			//endLevelSelect
			//deleteLeaderboard
			renderables[20] = new textBox(pRenderTarget, block::location{0.2, 0.8, 0.15, 0.075}, rc, pDWriteFactory, L"Delete", block::style::boarderless);
			renderables[21] = new textBox(pRenderTarget, block::location{0.275, 0.8, 0.15, 0.075}, rc, pDWriteFactory, L"Leaderboard", block::style::boarderless);
			renderables[22] = new block(pRenderTarget, block::location{0.2, 0.8, 0.15, 0.15}, rc, block::style::onlyBoarder);
			//endDeleteLeaderboard
			//volumeAdjustment
			renderables[23] = new textBox(pRenderTarget, block::location{0.4, 0.05, 0.15, 0.1}, rc, pDWriteFactory, L"Sound Volume");
			renderables[24] = new textBox(pRenderTarget, block::location{0.5, 0.05, 0.15, 0.05}, rc, pDWriteFactory, L"100%");
			renderables[25] = new block(pRenderTarget, block::location{0.55, 0.05, 0.15, 0.05}, rc);
			//effectAdjustment
			renderables[26] = new textBox(pRenderTarget, block::location{0.6, 0.05, 0.15, 0.1}, rc, pDWriteFactory, L"Effect Volume");
			renderables[27] = new textBox(pRenderTarget, block::location{0.7, 0.05, 0.15, 0.05}, rc, pDWriteFactory, L"100%");
			renderables[28] = new block(pRenderTarget, block::location{0.75, 0.05, 0.15, 0.05}, rc);
			//changeSong
			renderables[29] = new textBox(pRenderTarget, block::location{0.5, 0.8, 0.15, 0.1}, rc, pDWriteFactory, L"Change Song");
			renderables[30] = new textBox(pRenderTarget, block::location{0.6, 0.8, 0.15, 0.05}, rc, pDWriteFactory, L"100%");
			//controlButton
			renderables[31] = new textBox(pRenderTarget, block::location{0.7, 0.8, 0.15, 0.1}, rc, pDWriteFactory, L"Controls");

			sceneSize = 32;
			break;
		}
		default:
		break;
	}
	return true;
}

bool scene::clearScene()
{
	for (int i = 0; i < sceneSize; i++) {
		delete renderables[i];
	}
	delete[] renderables;
	renderables = NULL;
	return true;
}