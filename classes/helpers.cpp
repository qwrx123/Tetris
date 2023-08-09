#ifndef UNICODE
#define UNICODE
#endif 

#include "../headers/helpers.h"
#include <algorithm>

namespace helper
{
    void intToText(wchar_t* theText, int textSize, int someNumber)
	{
        for (int i = 0; i <= textSize; i++)
		{
            theText[i] = L'\0';
        }
        if (someNumber == 0)
		{
            theText[0] = L'0';
            return;
        }
        wchar_t number = 0;
        for (int i = textSize-1; i > -1; i--)
		{
            theText[i] = (someNumber % 10) + 0x30;
            someNumber /= 10;
			if (someNumber == 0)
			{
				break;
			}
        }
		int shift = 0;
        while (theText[shift] == L'\0') {
			shift++;
        }
        for (int i = shift; i < textSize; i++) {
            std::swap(theText[i-shift], theText[i]);
    	}
	}

    void textToInt(wchar_t* theText, int textSize, int* someNumber) {
        int tempNum = 0;
		for(int i = 0; i < textSize; i++)
		{
			if (theText[i] == L'\0')
			{
				break;
			}
			tempNum *= 10;
			tempNum += theText[i]-L'0';
		}
        *someNumber = tempNum;
    }

    void intToTextPercent(wchar_t* theText, int textSize, int someNumber)
	{
		helper::intToText(theText, textSize-1, someNumber);
		theText[textSize] = L'\0';
		for (int i = 0; i < textSize; i++)
		{
			if (theText[i] == L'\0')
			{
				theText[i] = L'%';
				break;
			}
		}
    }
}