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
    //myScene = scene(m_hwnd);
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

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
			myScene->resize(size);

			return 0;
		}
    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

bool TetrisWindow::test2()
{
return false;
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