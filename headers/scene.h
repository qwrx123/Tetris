#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <d2d1.h>
#include <d2d1helper.h>

#pragma comment(lib, "d2d1.lib")

#include "block.h"

class scene
{
public:
    scene(HWND hwnd);
    ~scene();
    bool resize(RECT size);
    bool render();
private:
    void clearScene();
private:
    HWND parentHwnd;
    ID2D1Factory* pD2DFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush* pWhiteBrush = NULL;
    
    int sceneSize;
};