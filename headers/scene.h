#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <d2d1.h>
#include <d2d1helper.h>

#pragma comment(lib, "d2d1.lib")

class scene
{
public:
    scene(HWND hwnd);
    ~scene();
    bool resize(D2D1_SIZE_U size);
    bool render();
private:
    HWND parentHwnd;
    ID2D1Factory* pD2DFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush* temppBlackBrush = NULL;
    ID2D1SolidColorBrush* temppWhiteBrush = NULL;
};