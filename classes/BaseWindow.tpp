#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Winmm.lib")


template <class DERIVED_TYPE> 
BaseWindow<DERIVED_TYPE>::BaseWindow() 
{
    m_hwnd = NULL;
}

template <class DERIVED_TYPE> 
LRESULT CALLBACK BaseWindow<DERIVED_TYPE>::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    DERIVED_TYPE *pThis = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hwnd = hwnd;
    }
    else
    {
        pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    if (pThis)
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

template <class DERIVED_TYPE> 
BOOL BaseWindow<DERIVED_TYPE>::Create(
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
    WNDCLASS wc = {0};

    wc.lpfnWndProc   = DERIVED_TYPE::WindowProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = ClassName();

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
        dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
        nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

    return (m_hwnd ? TRUE : FALSE);
}

template <class DERIVED_TYPE> 
HWND BaseWindow<DERIVED_TYPE>::Window() const 
{
    return this->m_hwnd;
}

template <class DERIVED_TYPE> 
PCWSTR  BaseWindow<DERIVED_TYPE>::ClassName() const {return 0;}

template <class DERIVED_TYPE> 
LRESULT BaseWindow<DERIVED_TYPE>::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {return 0;}