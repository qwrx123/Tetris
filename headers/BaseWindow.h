#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Winmm.lib")

template <class DERIVED_TYPE> 
class BaseWindow
{
public:
    BaseWindow();   

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    BOOL Create(
		PCWSTR lpWindowName,
    	DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    );

    HWND Window() const;

protected:

    virtual PCWSTR  ClassName() const;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
};

#include "../classes/BaseWindow.tpp"