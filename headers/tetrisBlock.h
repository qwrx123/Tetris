#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include "block.h"

class tetrisBlock : public block
{
public:
enum pieceType
{
    Tile,
    IBlock,
    JBlock,
    LBlock,
    OBlock,
    SBlock,
    TBlock,
    ZBlock,
    GhostBlock
};
private:
pieceType currentType;
bool boardPiece;
public:
tetrisBlock(ID2D1HwndRenderTarget* renderTarget, block::location setLocation, RECT screenSize, block::style myStyle = block::style::boarder);
void resize(block::location newLocation, RECT screenSize);
void setPieceType(pieceType currentType, bool isBoard = false);
void setPieceType(tetrisBlock& otherBlock);
bool testBoard();
void setBoard();
bool testGhost();
bool testTile();
tetrisBlock::pieceType getType();
private:
bool changeColor(int translateColor);
};