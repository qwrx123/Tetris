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
#include "clickableTextBox.h"

class scene
{
public:
    enum screen
    {
        invalid = -1,
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
    int renderSize;
    clickableTextBox** clickables;
    int clickableSize;
    scene::screen whenChange;
    int volumeScale;
    int songNumber;
    bool willDelete;
public:
    scene(HWND hwnd);
    ~scene();
    bool resize(RECT size);
    bool render();
    bool changeScene(scene::screen);
    bool clearScene();
    void onMove(D2D1_POINT_2F moved);
    void onClick(D2D1_POINT_2F clicked);
    void onCapture(D2D1_POINT_2F captrue);
    void checkScene();
private:
};