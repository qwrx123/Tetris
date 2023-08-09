#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

namespace helper
{
    void intToText(wchar_t* theText, int textSize, int someNumber);
    void textToInt(wchar_t* theText, int textSize, int* someNumber);
    void intToTextPercent(wchar_t* theText, int textSize, int someNumber);
}