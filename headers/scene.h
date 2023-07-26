#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include "block.h"

class scene
{
public:
    enum screen
    {
        Title,
        Game,
        Death,
        Control,
        Pause
    };
private:
    HWND parentHwnd;
    ID2D1Factory* pD2DFactory;
    IDWriteFactory* pDWriteFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    block** renderables;
    int sceneSize;
public:
    scene(HWND hwnd);
    ~scene();
    bool resize(RECT size);
    bool render();
    bool changeScene(scene::screen);
    bool clearScene();
private:
};