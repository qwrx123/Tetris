#pragma once
#ifndef UNICODE
#define UNICODE
#endif 
#include "BaseWindow.h"
#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Winmm.lib")

class TetrisWindow : public BaseWindow<TetrisWindow>
{
public:
    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool test2();
private:
	
};

