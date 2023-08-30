#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "headers/BaseWindow.h"
#include "headers/TetrisWindow.h"
#include "headers/scene.h"
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Winmm.lib")

#include <iostream>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    TetrisWindow win;
	
    if (!win.Create(L"Tetris", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), SW_MAXIMIZE);

    win.forceRender();

    // Run the message loop.

    MSG msg = { };
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) {
				return 0;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
            win.forceRender();
		}
    }
    return 0;
}



