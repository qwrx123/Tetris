#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "headers/BaseWindow.h"
#include "headers/TetrisWindow.h"
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Winmm.lib")



int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    TetrisWindow win;
    win.test();
    win.test2();
    if (!win.Create(L"Tetris", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}



