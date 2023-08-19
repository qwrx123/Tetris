#include "../headers/tetrisPiece.h"

tetrisPiece::tetrisPiece(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize)
    :pieces{tetrisBlock(renderTarget, {0.0, 0.0, 0.0, 0.0}, screenSize),
    tetrisBlock(renderTarget, {0.0, 0.0, 0.0, 0.0}, screenSize),
    tetrisBlock(renderTarget, {0.0, 0.0, 0.0, 0.0}, screenSize),
    tetrisBlock(renderTarget, {0.0, 0.0, 0.0, 0.0}, screenSize)},
    pieceType(0), coordinates(setLocation)
{

}

void tetrisPiece::resize(RECT screenSize)
{
    float blockSize = (screenSize.right * coordinates.width)/4.0f < (screenSize.bottom * coordinates.height)/2.0f ? (screenSize.right * coordinates.width)/4.0f : (screenSize.bottom * coordinates.height)/2.0f;
    D2D1_POINT_2F middle = {(screenSize.right * coordinates.width)/2.0f + screenSize.right * coordinates.left, (screenSize.bottom * coordinates.height)/2.0f + screenSize.bottom * coordinates.top};
    switch(pieceType)
    {
        case 1:
        {
            pieces[0].resize({(middle.y-blockSize/2)/screenSize.bottom, (middle.x-blockSize*2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[1].resize({(middle.y-blockSize/2)/screenSize.bottom, (middle.x-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[2].resize({(middle.y-blockSize/2)/screenSize.bottom, (middle.x)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[3].resize({(middle.y-blockSize/2)/screenSize.bottom, (middle.x+blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            break;
        }
        case 2:
        {
            pieces[0].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x-blockSize/2-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[1].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[2].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[3].resize({(middle.y)/screenSize.bottom, (middle.x+blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            break;
        }
        case 3:
        {
            pieces[0].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x+blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[1].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[2].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[3].resize({(middle.y)/screenSize.bottom, (middle.x+blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            break;
        }
        case 4:
        {
            pieces[0].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[1].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[2].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[3].resize({(middle.y)/screenSize.bottom, (middle.x)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            break;
        }
        case 5:
        {
            pieces[0].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[1].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[2].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x-blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[3].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x+blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            break;
        }
        case 6:
        {
            pieces[0].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x-blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[1].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[2].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[3].resize({(middle.y)/screenSize.bottom, (middle.x+blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            break;
        }
        case 7:
        {
            pieces[0].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x-blockSize/2-blockSize)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[1].resize({(middle.y-blockSize)/screenSize.bottom, (middle.x-blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[2].resize({(middle.y)/screenSize.bottom, (middle.x-blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            pieces[3].resize({(middle.y)/screenSize.bottom, (middle.x+blockSize/2)/screenSize.right, blockSize/screenSize.right, blockSize/screenSize.bottom}, screenSize);
            break;
        }
    }
}

void tetrisPiece::changePiece(int type, RECT screenSize)
{
    pieceType = type;
    pieces[0].changeColor(pieceType);
    pieces[1].changeColor(pieceType);
    pieces[2].changeColor(pieceType);
    pieces[3].changeColor(pieceType);
    resize(screenSize);
}

void tetrisPiece::render()
{
    if (pieceType == 0)
    {
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        pieces[i].render();
    }
}