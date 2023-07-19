#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/TetrisWindow.h"

#include "../headers/BaseWindow.h"

#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Winmm.lib")

LRESULT TetrisWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(m_hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

bool TetrisWindow::test2()
{
return false;
}

PCWSTR  TetrisWindow::ClassName() const { return L"Tetris"; }