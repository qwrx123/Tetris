#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/scene.h"
#include "../headers/block.h"

scene::scene(HWND hwnd)
{
    parentHwnd = hwnd;

    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

    if (SUCCEEDED(hr))
    {
        RECT rc;
        GetClientRect(parentHwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(), 
            D2D1::HwndRenderTargetProperties(parentHwnd, size),
            &pRenderTarget);

        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
    }
}

scene::~scene()
{
    pD2DFactory->Release();
    pRenderTarget->Release();
}

void scene::clearScene()
{
	RECT rc;
	GetClientRect(parentHwnd, &rc);

	pRenderTarget->FillRectangle(
	D2D1::RectF(
		rc.left * 1.0f,
		rc.top * 1.0f,
		rc.right * 1.0f,
		rc.bottom * 1.0f),
	pWhiteBrush
	);
}
bool scene::render()
{
    pRenderTarget->BeginDraw();

	clearScene();

    pRenderTarget->EndDraw();
    return true;
}

bool scene::resize(RECT size)
{
    D2D1_SIZE_U newSize = D2D1::SizeU(size.right, size.bottom);

    return SUCCEEDED(pRenderTarget->Resize(newSize));
}