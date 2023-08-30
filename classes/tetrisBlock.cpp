#include "../headers/tetrisBlock.h"

tetrisBlock::tetrisBlock(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, block::style myStyle)
    :block(renderTarget, setLocation, screenSize, myStyle), boardPiece(false)
{

}

void tetrisBlock::resize(block::location newLocation, RECT screenSize)
{
    coordinates = newLocation;
    block::resize(screenSize);
}

void tetrisBlock::setPieceType(pieceType currentType, bool isBoard)
{
    boardPiece = isBoard;
    this->currentType = currentType;
    changeColor((int)currentType);
}

void tetrisBlock::setPieceType(tetrisBlock& otherBlock)
{
    setPieceType(otherBlock.getType(), otherBlock.testBoard());
}
tetrisBlock::pieceType tetrisBlock::getType()
{
    return currentType;
}

bool tetrisBlock::testBoard()
{
    return boardPiece;
}

void tetrisBlock::setBoard()
{
    boardPiece = true;
}

bool tetrisBlock::testGhost()
{
    return currentType == tetrisBlock::pieceType::GhostBlock;
}

bool tetrisBlock::testTile()
{
    return currentType == tetrisBlock::pieceType::Tile;
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
        case 8:
        block::changeColor(block::colors::gray);
        break;
    }
    return true;
}