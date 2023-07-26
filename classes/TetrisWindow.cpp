#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/TetrisWindow.h"

#include "../headers/BaseWindow.h"

#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Winmm.lib")

TetrisWindow::TetrisWindow()
{

}

TetrisWindow::~TetrisWindow()
{
	delete myScene;
}

LRESULT TetrisWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
			myScene->render();
        }
        return 0;
	case WM_SIZE:
		{
			RECT rc;

			GetClientRect(m_hwnd, &rc);

			myScene->resize(rc);

			return 0;
		}
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 300;
			lpMMI->ptMinTrackSize.y = 300;
			return 0;
		}
    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

bool TetrisWindow::forceRender()
{
    return myScene->render();
}

PCWSTR  TetrisWindow::ClassName() const { return L"Tetris"; }

BOOL TetrisWindow::Create(
    PCWSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu
    ) 
{
    bool returnBool = BaseWindow::Create(
        lpWindowName,
        dwStyle,
    	dwExStyle,
    	x,
    	y,
    	nWidth,
    	nHeight,
    	hWndParent,
    	hMenu
    );

	myScene = new scene(m_hwnd);

	return returnBool;
}