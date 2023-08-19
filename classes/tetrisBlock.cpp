#include "../headers/tetrisBlock.h"

tetrisBlock::tetrisBlock(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, block::style myStyle)
    :block(renderTarget, setLocation, screenSize, myStyle)
{

}

void tetrisBlock::resize(block::location newLocation, RECT screenSize)
{
    coordinates = newLocation;
    block::resize(screenSize);
}

bool tetrisBlock::changeColor(int translateColor)
{
    switch(translateColor)
    {
        case 0:
        block::changeColor(block::colors::white);
        break;
        case 1:
        block::changeColor(block::colors::cyan);
        break;
        case 2:
        block::changeColor(block::colors::blue);
        break;
        case 3:
        block::changeColor(block::colors::orange);
        break;
        case 4:
        block::changeColor(block::colors::yellow);
        break;
        case 5:
        block::changeColor(block::colors::green);
        break;
        case 6:
        block::changeColor(block::colors::purple);
        break;
        case 7:
        block::changeColor(block::colors::red);
        break;
        case 15:
        block::changeColor(block::colors::gray);
        break;
    }
    return true;
}