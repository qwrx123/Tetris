#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/scene.h"

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

        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &temppBlackBrush);
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &temppWhiteBrush);
    }
}

scene::~scene()
{
    pD2DFactory->Release();
    pRenderTarget->Release();
}

bool scene::render() {
    pRenderTarget->BeginDraw();
    RECT rc;
    GetClientRect(parentHwnd, &rc);
        
    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

	pRenderTarget->FillRectangle(
		D2D1::RectF(
			rc.left * 1.0f,
			rc.top * 1.0f,
			rc.right * 1.0f,
			rc.bottom * 1.0f),
		temppWhiteBrush
	);

    pRenderTarget->DrawRectangle(
	D2D1::RectF(
		rc.right * 0.25f,
		rc.bottom * 0.4f,
		rc.right * 0.75f,
		rc.bottom * 0.75f),
	temppBlackBrush,
	2.0,
	NULL);

    pRenderTarget->EndDraw();
    return true;
}

bool scene::resize(D2D1_SIZE_U size) {
    return SUCCEEDED(pRenderTarget->Resize(size));
}