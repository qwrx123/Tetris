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
#include "game.h"

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

    struct mySettings {
        int version;
        wchar_t soundVolumeT[5];
        int soundVolume;
        wchar_t effectVolumeT[5];
        int effectVolume;
        wchar_t startLevelT[10];
        int startLevel;
        int songChoice;
        wchar_t name[20];
    };
private:
    mySettings settings;
    int settingVersion;
    game::gameMetrics currentMetrics;
    game::leaderboard currentLeaderboard[5];
    HWND parentHwnd;
    ID2D1Factory* pD2DFactory;
    IDWriteFactory* pDWriteFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    block** renderables;
    int playerNameSize = 20;
    int renderSize;
    clickableTextBox** clickables;
    int clickableSize;
    scene::screen whenChange;
    bool willDelete;
    game *tetrisGame;
    bool gamePaused;
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
    void onKey(wchar_t inputChar);
    void keyDown(wchar_t inputChar);
    void keyUp(wchar_t inputChar);
    void checkScene();
private:
    void calculateLeaderboard();
    void saveLeaderboard();
    void deleteLeaderboard();
    void initializeLeaderboard();
    void initializeSettings();
    void saveSettings();
};
