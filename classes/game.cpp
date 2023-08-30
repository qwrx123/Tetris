#include "../headers/game.h"
#include "../headers/scene.h"

const block::location game::gameSquareBoundary = {0.21, 0.3, 0.4, 0.74};
const float game::timing[] = {48.0f/60.0f, 43.0f/60.0f, 38.0f/60.0f, 33.0f/60.0f, 28.0f/60.0f, 23.0f/60.0f, 18.0f/60.0f, 13.0f/60.0f, 8.0f/60.0f, 6.0f/60.0f
					, 5.0f/60.0f, 5.0f/60.0f, 5.0f/60.0f, 4.0f/60.0f, 4.0f/60.0f, 4.0f/60.0f, 3.0f/60.0f, 3.0f/60.0f, 3.0f/60.0f};
const game::pieceLocation game::startingPosistions[] = {{{{3,0},{4,0},{5,0},{6,0}}, tetrisBlock::pieceType::IBlock, 1},
														{{{3,0},{4,0},{5,0},{6,0}}, tetrisBlock::pieceType::IBlock, 1},
														{{{3,0},{3,1},{4,1},{5,1}}, tetrisBlock::pieceType::JBlock, 1},
														{{{3,1},{4,1},{5,1},{5,0}}, tetrisBlock::pieceType::LBlock, 1},
														{{{4,0},{5,0},{4,1},{5,1}}, tetrisBlock::pieceType::OBlock, 1},
														{{{3,1},{4,1},{4,0},{5,0}}, tetrisBlock::pieceType::SBlock, 1},
														{{{3,1},{4,1},{5,1},{4,0}}, tetrisBlock::pieceType::TBlock, 1},
														{{{3,0},{4,0},{4,1},{5,1}}, tetrisBlock::pieceType::ZBlock, 1}};

const POINT game::turnOffset[8][5][4] = {{{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{1, 1}, {1, 1}, {1, 1}, {1, 1}}},
											
											{{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{2, -1}, {1, 0}, {0, 1}, {-1, 2}},
											{{1, 2}, {0, 1}, {-1, 0}, {-2, -1}},
											{{-2, 1}, {-1, 0}, {0, -1}, {1, -2}},
											{{-1, -2}, {0, -1}, {1, 0}, {2, 1}}},
											
											{{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{2, 0}, {1, -1}, {0, 0}, {-1, 1}},
											{{0, 2}, {1, 1}, {0, 0}, {-1, -1}},
											{{-2, 0}, {-1, 1}, {0, 0}, {1, -1}},
											{{0, -2}, {-1, -1}, {0, 0}, {1, 1}}},
											
											{{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{1, -1}, {0, 0}, {-1, 1}, {0, 2}},
											{{1, 1}, {0, 0}, {-1, -1}, {-2, 0}},
											{{-1, 1}, {0, 0}, {1, -1}, {0, -2}},
											{{-1, -1}, {0, 0}, {1, 1}, {2, 0}}},
											
											{{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
											{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
											{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
											{{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
											
											{{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{1, -1}, {0, 0}, {1, 1}, {0, 2}},
											{{1, 1}, {0, 0}, {-1, 1}, {-2, 0}},
											{{-1, 1}, {0, 0}, {-1, -1}, {0, -2}},
											{{-1, -1}, {0, 0}, {1, -1}, {2, 0}}},
											
											{{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{1, -1}, {0, 0}, {-1, 1}, {1, 1}},
											{{1, 1}, {0, 0}, {-1, -1}, {-1, 1}},
											{{-1, 1}, {0, 0}, {1, -1}, {-1, -1}},
											{{-1, -1}, {0, 0}, {1, 1}, {1, -1}}},
											
											{{{1, 1}, {1, 1}, {1, 1}, {1, 1}},
											{{2, 0}, {1, 1}, {0, 0}, {-1, 1}},
											{{0, 2}, {-1, 1}, {0, 0}, {-1, -1}},
											{{-2, 0}, {-1, -1}, {0, 0}, {1, -1}},
											{{0, -2}, {1, -1}, {0, 0}, {1, 1}}}};

game::game(ID2D1HwndRenderTarget* renderTarget, IDWriteFactory* pDWriteFactory, RECT screenSize, wchar_t playerName[20], int startingLevel, int songNumber, int musicVolume, int effectVolume, songManager effectGenerator)
    :scoreLabel(renderTarget, {0.55, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"Score"), 
    scoreDisplay(renderTarget, {0.6, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"0"),
    levelLabel(renderTarget, {0.65, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"Level"),
    levelDisplay(renderTarget, {0.7, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"0"),
    linesLabel(renderTarget, {0.75, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"Lines"),
    linesDisplay(renderTarget, {0.8, 0.05, 0.15, 0.05}, screenSize, pDWriteFactory, L"0"),
    holdLabel(renderTarget, {0.25, 0.05, 0.15, 0.1}, screenSize, pDWriteFactory, L"Hold"),
    holdDisplay(renderTarget, {0.35, 0.05, 0.15, 0.1}, screenSize),
    nextLabel(renderTarget, {0.3, 0.8, 0.15, 0.1}, screenSize, pDWriteFactory, L"Next"),
    nextDisplay(renderTarget, {0.4, 0.8, 0.15, 0.38}, screenSize),
    holdPiece(renderTarget, {0.36, 0.06, 0.13, 0.08}, screenSize),
    nextPieces{tetrisPiece(renderTarget, {0.42, 0.81, 0.13, 0.1}, screenSize),
    tetrisPiece(renderTarget, {0.54, 0.81, 0.13, 0.1}, screenSize),
    tetrisPiece(renderTarget, {0.66, 0.81, 0.13, 0.1}, screenSize)},
	died(false), score(0), level(startingLevel), lines(0), effectGenerator(effectGenerator), holding(tetrisBlock::pieceType::Tile)
{
    for (int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            renderScreenBlocks[i][j] = new tetrisBlock(renderTarget, {0.1, 0.1, 0.1, 0.1}, screenSize);
			renderScreenBlocks[i][j]->setPieceType(tetrisBlock::pieceType::Tile);
        }
    }
	for(int i = 0; i < 14; i++)
	{
		nextBlock[i] = tetrisBlock::pieceType::Tile;
	}
    game::resize(screenSize);
    calculateNextBlocks(0, 7);//0 - 14 first time
	calculateNextBlocks(7, 14);//0 - 14 first time
    sendBlocks();
	findGhost();
    setCount = timing[level];
	count = setCount;
	wcsncpy(game::playerName, playerName, 20);
	helper::intToText(scoreText, 9, score);
	helper::intToText(linesText, 9, lines);
	helper::intToText(levelText, 9, level);
	levelDisplay.changeText(levelText);
}

game::~game()
{
	for (int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            delete renderScreenBlocks[i][j];
        }
    }
}

bool game::testDeath()
{
	return died;
}

void game::populateMetrics(game::gameMetrics& populate)
{
	wcsncpy(populate.name, playerName, 20);
	populate.score = game::score;
	wcsncpy(populate.scoreText, game::scoreText, 10);
	populate.level = game::level;
	wcsncpy(populate.levelText, game::levelText, 10);
	populate.lines = game::lines;
	wcsncpy(populate.linesText, game::linesText, 10);
}

void game::gameLoop()
{
    countTime.calculateDeltaTime();
    float deltaTime = countTime.getDeltaTime();
    count -= deltaTime;
	if (died)
	{
		return;
	}
    if (count <= 0.0f) 
    {
		dropBlocks();
		findGhost();
	}

	if (keys[0x41] == true) {
		moveBlocks(-1);
		findGhost();
		keys[0x41] = false;
	}
	if (keys[0x44] == true) {
		moveBlocks(1);
		findGhost();
		keys[0x44] = false;
	}
	if (keys[0x53] == true) {
		dropBlocks();
		score++;
		helper::intToText(scoreText, 9, score);
        scoreDisplay.changeText(scoreText);
		findGhost();
		keys[0x53] = false;
	}
	if (keys[0x46] == true) {
		switchHold();
		findGhost();
		keys[0x46] = false;
	}
	if (keys[0x57] == true) {
		if (turnBlocks(0, 0)) {}
		else if (turnBlocks(0, 1)) {}
		else if (turnBlocks(1, 0)) {}
		else if (turnBlocks(-1, 0)) {}
		else if (turnBlocks(0, -1)) {}
		findGhost();
		keys[0x57] = false;
	}
	if (keys[VK_LEFT] == true) {
		moveBlocks(-1);
		findGhost();
		keys[VK_LEFT] = false;
	}
	if (keys[VK_RIGHT] == true) {
		moveBlocks(1);
		findGhost();
		keys[VK_RIGHT] = false;
	}
	if (keys[VK_DOWN] == true) {
		dropBlocks();
		score++;
		helper::intToText(scoreText, 9, score);
        scoreDisplay.changeText(scoreText);
		findGhost();
		keys[VK_DOWN] = false;
	}
	if (keys[VK_UP] == true) {
		if (turnBlocks(0, 0)) {}
		else if (turnBlocks(0, 1)) {}
		else if (turnBlocks(1, 0)) {}
		else if (turnBlocks(-1, 0)) {}
		else if (turnBlocks(0, -1)) {}
		findGhost();
		keys[VK_UP] = false;
	}
	if (keys[VK_SPACE] == true) {
		while (dropBlocks()) 
        {
			score += 2;

		};
    	helper::intToText(scoreText, 9, score);
        scoreDisplay.changeText(scoreText);
		findGhost();
		keys[VK_SPACE] = false;
	}
}

void game::render()
{
    for (int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            renderScreenBlocks[i][j]->render();
        }
    }
    scoreLabel.render();
    scoreDisplay.render();
    levelLabel.render();
    levelDisplay.render();
    linesLabel.render();
    linesDisplay.render();
    holdLabel.render();
    holdDisplay.render();
    nextLabel.render();
    nextDisplay.render();
    holdPiece.render();
    for(int i = 0; i < 3; i++)
    {
        nextPieces[i].render();
    }
}

void game::resize(RECT newScreen)
{
    currentScreenSize = newScreen;
    float blockSize = (newScreen.right * gameSquareBoundary.width)/10.0f < (newScreen.bottom * gameSquareBoundary.height)/20.0f ? (newScreen.right * gameSquareBoundary.width)/10.0f : (newScreen.bottom * gameSquareBoundary.height)/20.0f;
    D2D1_POINT_2F middle = {(newScreen.right * gameSquareBoundary.width)/2.0f + newScreen.right * gameSquareBoundary.left, (newScreen.bottom * gameSquareBoundary.height)/2.0f + newScreen.bottom * gameSquareBoundary.top};
    for (int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            renderScreenBlocks[i][j]->resize({((middle.y - blockSize*10) + j*blockSize)/newScreen.bottom, ((middle.x - blockSize*5) + i*blockSize)/newScreen.right, blockSize/newScreen.right, blockSize/newScreen.bottom}, newScreen);
        }
    }
    scoreLabel.resize(newScreen);
    scoreDisplay.resize(newScreen);
    levelLabel.resize(newScreen);
    levelDisplay.resize(newScreen);
    linesLabel.resize(newScreen);
    linesDisplay.resize(newScreen);
    holdLabel.resize(newScreen);
    holdDisplay.resize(newScreen);
    nextLabel.resize(newScreen);
    nextDisplay.resize(newScreen);
    holdPiece.resize(newScreen);
    for(int i = 0; i < 3; i++)
    {
        nextPieces[i].resize(newScreen);
    }
}

void game::onKey(wchar_t inputChar)
{
    keys[inputChar] = false;
}

void game::keyDown(wchar_t inputChar)
{
    keys[inputChar] = true;
}

void game::sendBlocks() 
{
	if (renderScreenBlocks[3][0]->testBoard() ||
		renderScreenBlocks[4][0]->testBoard() ||
		renderScreenBlocks[5][0]->testBoard() ||
		renderScreenBlocks[6][0]->testBoard() ||
		renderScreenBlocks[3][1]->testBoard() ||
		renderScreenBlocks[4][1]->testBoard() ||
		renderScreenBlocks[5][1]->testBoard() ||
		renderScreenBlocks[6][1]->testBoard())
	{
		died = true;
		return;
	}
	currentBlocks = startingPosistions[nextBlock[0]];
	renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(currentBlocks.type);
	renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(currentBlocks.type);
	renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(currentBlocks.type);
	renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(currentBlocks.type);
	count = setCount;
	canHold = true;
	cycleNextBlocks();
}

bool game::dropBlocks() {
	if (currentBlocks.location[0].y + 1 < 20 && currentBlocks.location[1].y + 1 < 20 &&
		currentBlocks.location[2].y + 1 < 20 && currentBlocks.location[3].y + 1 < 20 &&
		!renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y + 1]->testBoard() &&
		!renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y + 1]->testBoard() &&
		!renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y + 1]->testBoard() &&
		!renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y + 1]->testBoard())
	{
		renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(tetrisBlock::pieceType::Tile);
		renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(tetrisBlock::pieceType::Tile);
		renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(tetrisBlock::pieceType::Tile);
		renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(tetrisBlock::pieceType::Tile);
		currentBlocks.location[0].y++;
		currentBlocks.location[1].y++;
		currentBlocks.location[2].y++;
		currentBlocks.location[3].y++;
		renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(currentBlocks.type);
		count = setCount;
		return true;
	}
	else 
	{
		renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setBoard();
        renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setBoard();
        renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setBoard();
        renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setBoard();
		checkLines();
		effectGenerator.playEffectSound();
		sendBlocks();
		return false;
	}
	return true;
}

void game::findGhost() {
	bool isRunning = true;
	if (renderScreenBlocks[ghostBlocks.location[0].x][ghostBlocks.location[0].y]->testGhost()) {
        renderScreenBlocks[ghostBlocks.location[0].x][ghostBlocks.location[0].y]->setPieceType(tetrisBlock::pieceType::Tile);
	}
	if (renderScreenBlocks[ghostBlocks.location[1].x][ghostBlocks.location[1].y]->testGhost()) {
        renderScreenBlocks[ghostBlocks.location[1].x][ghostBlocks.location[1].y]->setPieceType(tetrisBlock::pieceType::Tile);
	}
	if (renderScreenBlocks[ghostBlocks.location[2].x][ghostBlocks.location[2].y]->testGhost()) {
        renderScreenBlocks[ghostBlocks.location[2].x][ghostBlocks.location[2].y]->setPieceType(tetrisBlock::pieceType::Tile);
	}
	if (renderScreenBlocks[ghostBlocks.location[3].x][ghostBlocks.location[3].y]->testGhost()) {
        renderScreenBlocks[ghostBlocks.location[3].x][ghostBlocks.location[3].y]->setPieceType(tetrisBlock::pieceType::Tile);
	}
	ghostBlocks = currentBlocks;
	while (isRunning) {
		if (ghostBlocks.location[0].y + 1 < 20 && ghostBlocks.location[1].y + 1 < 20 &&
			ghostBlocks.location[2].y + 1 < 20 && ghostBlocks.location[3].y + 1 < 20 &&
			!renderScreenBlocks[ghostBlocks.location[0].x][ghostBlocks.location[0].y + 1]->testBoard() &&
			!renderScreenBlocks[ghostBlocks.location[1].x][ghostBlocks.location[1].y + 1]->testBoard() &&
			!renderScreenBlocks[ghostBlocks.location[2].x][ghostBlocks.location[2].y + 1]->testBoard() &&
			!renderScreenBlocks[ghostBlocks.location[3].x][ghostBlocks.location[3].y + 1]->testBoard())
		{
			if (renderScreenBlocks[ghostBlocks.location[0].x][ghostBlocks.location[0].y]->testGhost())
			{
                renderScreenBlocks[ghostBlocks.location[0].x][ghostBlocks.location[0].y]->setPieceType(tetrisBlock::pieceType::Tile);
			}
			if (renderScreenBlocks[ghostBlocks.location[1].x][ghostBlocks.location[1].y]->testGhost())
			{
                renderScreenBlocks[ghostBlocks.location[1].x][ghostBlocks.location[1].y]->setPieceType(tetrisBlock::pieceType::Tile);
			}
			if (renderScreenBlocks[ghostBlocks.location[2].x][ghostBlocks.location[2].y]->testGhost())
			{
                renderScreenBlocks[ghostBlocks.location[2].x][ghostBlocks.location[2].y]->setPieceType(tetrisBlock::pieceType::Tile);
			}
			if (renderScreenBlocks[ghostBlocks.location[3].x][ghostBlocks.location[3].y]->testGhost())
			{
                renderScreenBlocks[ghostBlocks.location[3].x][ghostBlocks.location[3].y]->setPieceType(tetrisBlock::pieceType::Tile);
			}
			ghostBlocks.location[0].y++;
			ghostBlocks.location[1].y++;
			ghostBlocks.location[2].y++;
			ghostBlocks.location[3].y++;
			if (renderScreenBlocks[ghostBlocks.location[0].x][ghostBlocks.location[0].y]->testTile())
			{
                renderScreenBlocks[ghostBlocks.location[0].x][ghostBlocks.location[0].y]->setPieceType(tetrisBlock::pieceType::GhostBlock);
			}
			if (renderScreenBlocks[ghostBlocks.location[1].x][ghostBlocks.location[1].y]->testTile())
			{
                renderScreenBlocks[ghostBlocks.location[1].x][ghostBlocks.location[1].y]->setPieceType(tetrisBlock::pieceType::GhostBlock);
			}
			if (renderScreenBlocks[ghostBlocks.location[2].x][ghostBlocks.location[2].y]->testTile())
			{
                renderScreenBlocks[ghostBlocks.location[2].x][ghostBlocks.location[2].y]->setPieceType(tetrisBlock::pieceType::GhostBlock);
			}
			if (renderScreenBlocks[ghostBlocks.location[3].x][ghostBlocks.location[3].y]->testTile())
			{
                renderScreenBlocks[ghostBlocks.location[3].x][ghostBlocks.location[3].y]->setPieceType(tetrisBlock::pieceType::GhostBlock);
			}
			isRunning = true;
		}
		else {
			isRunning = false;
		}
	}
}

void game::moveBlocks(int direction) {
	if (currentBlocks.location[0].x + direction <= 9 && currentBlocks.location[1].x + direction <= 9 &&
		currentBlocks.location[2].x + direction <= 9 && currentBlocks.location[3].x + direction <= 9 &&
		currentBlocks.location[0].x + direction >= 0 && currentBlocks.location[1].x + direction >= 0 &&
		currentBlocks.location[2].x + direction >= 0 && currentBlocks.location[3].x + direction >= 0 &&
		!renderScreenBlocks[currentBlocks.location[0].x + direction][currentBlocks.location[0].y]->testBoard() &&
		!renderScreenBlocks[currentBlocks.location[1].x + direction][currentBlocks.location[1].y]->testBoard() &&
		!renderScreenBlocks[currentBlocks.location[2].x + direction][currentBlocks.location[2].y]->testBoard() &&
		!renderScreenBlocks[currentBlocks.location[3].x + direction][currentBlocks.location[3].y]->testBoard())
	{
		renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(tetrisBlock::pieceType::Tile);
		renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(tetrisBlock::pieceType::Tile);
		renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(tetrisBlock::pieceType::Tile);
		renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(tetrisBlock::pieceType::Tile);
		currentBlocks.location[0].x += direction;
		currentBlocks.location[1].x += direction;
		currentBlocks.location[2].x += direction;
		currentBlocks.location[3].x += direction;
		renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(currentBlocks.type);
	}
}

void game::switchHold() {
	if (canHold == false)
	{
		return;
	}
	renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(tetrisBlock::pieceType::Tile);
	renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(tetrisBlock::pieceType::Tile);
	renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(tetrisBlock::pieceType::Tile);
	renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(tetrisBlock::pieceType::Tile);
	if (holding == tetrisBlock::pieceType::Tile)
	{
		holding = currentBlocks.type;
		sendBlocks();
	}
	else
	{
		tetrisBlock::pieceType tempHold = currentBlocks.type;
		currentBlocks = startingPosistions[holding];
		renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(currentBlocks.type);
		renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(currentBlocks.type);
		holding = tempHold;
	}
	holdPiece.changePiece(holding, currentScreenSize);
	canHold = false;
}

void game::checkLines() {
	int tempLines = 0;
	for (int i = 19; i >= 0; i--) 
	{
		if (renderScreenBlocks[0][i]->testBoard() && renderScreenBlocks[1][i]->testBoard() && renderScreenBlocks[2][i]->testBoard() &&
			renderScreenBlocks[3][i]->testBoard() && renderScreenBlocks[4][i]->testBoard() && renderScreenBlocks[5][i]->testBoard() &&
			renderScreenBlocks[6][i]->testBoard() && renderScreenBlocks[7][i]->testBoard() && renderScreenBlocks[8][i]->testBoard() &&
			renderScreenBlocks[9][i]->testBoard())
		{
			for (int k = i; k > 0; k--)
			{
                renderScreenBlocks[0][k]->setPieceType(*renderScreenBlocks[0][k-1]);
                renderScreenBlocks[1][k]->setPieceType(*renderScreenBlocks[1][k-1]);
                renderScreenBlocks[2][k]->setPieceType(*renderScreenBlocks[2][k-1]);
                renderScreenBlocks[3][k]->setPieceType(*renderScreenBlocks[3][k-1]);
                renderScreenBlocks[4][k]->setPieceType(*renderScreenBlocks[4][k-1]);
                renderScreenBlocks[5][k]->setPieceType(*renderScreenBlocks[5][k-1]);
                renderScreenBlocks[6][k]->setPieceType(*renderScreenBlocks[6][k-1]);
                renderScreenBlocks[7][k]->setPieceType(*renderScreenBlocks[7][k-1]);
                renderScreenBlocks[8][k]->setPieceType(*renderScreenBlocks[8][k-1]);
                renderScreenBlocks[9][k]->setPieceType(*renderScreenBlocks[9][k-1]);
			}
            renderScreenBlocks[0][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[1][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[2][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[3][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[4][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[5][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[6][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[7][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[8][0]->setPieceType(tetrisBlock::pieceType::Tile);
            renderScreenBlocks[9][0]->setPieceType(tetrisBlock::pieceType::Tile);
			tempLines++;
			i++;
		}
	}
	lines += tempLines;
	helper::intToText(linesText, 9, lines);
    linesDisplay.changeText(linesText);
	if (tempLines == 1) {
		score += 40 * (level + 1);
	}
	if (tempLines == 2) {
		score += 100 * (level + 1);
	}
	if (tempLines == 3) {
		score += 300 * (level + 1);
	}
	if (tempLines == 4) {
		score += 1200 * (level + 1);
	}
	if (score > 999999999) {
		score = 999999999;
	}
	helper::intToText(scoreText, 9, score);
    scoreDisplay.changeText(scoreText);
	if (level < lines / 10) {
		level = (lines / 10);
	}
	helper::intToText(levelText, 9, level);
    levelDisplay.changeText(levelText);
	if (level <= 18) {
		setCount = timing[level];
	}
	else if (level <= 28) {
		setCount = 2 / 60;
	}
	else {
		setCount = 1 / 60;
	}
}

bool game::turnBlocks(int xShift, int yShift) {
	bool turned = false;
	POINT currentTurn[4] = {{turnOffset[currentBlocks.type][currentBlocks.turn][0].x, turnOffset[currentBlocks.type][currentBlocks.turn][0].y},
							{turnOffset[currentBlocks.type][currentBlocks.turn][1].x, turnOffset[currentBlocks.type][currentBlocks.turn][1].y},
							{turnOffset[currentBlocks.type][currentBlocks.turn][2].x, turnOffset[currentBlocks.type][currentBlocks.turn][2].y},
							{turnOffset[currentBlocks.type][currentBlocks.turn][3].x, turnOffset[currentBlocks.type][currentBlocks.turn][3].y}};
	
	renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(tetrisBlock::pieceType::Tile);
    renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(tetrisBlock::pieceType::Tile);
    renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(tetrisBlock::pieceType::Tile);
    renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(tetrisBlock::pieceType::Tile);
	
	if (currentBlocks.location[0].y + yShift + currentTurn[0].y < 20 && currentBlocks.location[1].y + yShift + currentTurn[1].y < 20 && currentBlocks.location[2].y + yShift + currentTurn[2].y < 20 && currentBlocks.location[3].y + yShift + currentTurn[3].y < 20 &&
			currentBlocks.location[0].y + yShift + currentTurn[0].y >= 0 && currentBlocks.location[1].y + yShift + currentTurn[1].y >= 0 && currentBlocks.location[2].y + yShift + currentTurn[2].y >= 0 && currentBlocks.location[3].y + yShift + currentTurn[3].y >= 0 &&
			currentBlocks.location[0].x + xShift + currentTurn[0].x < 10 && currentBlocks.location[1].x + xShift + currentTurn[1].x < 10 && currentBlocks.location[2].x + xShift + currentTurn[2].x < 10 && currentBlocks.location[3].x + xShift + currentTurn[3].x < 10 &&
			currentBlocks.location[0].x + xShift + currentTurn[0].x >= 0 && currentBlocks.location[1].x + xShift + currentTurn[1].x >= 0 && currentBlocks.location[2].x + xShift + currentTurn[2].x >= 0 && currentBlocks.location[3].x + xShift + currentTurn[3].x >= 0 && 
			!renderScreenBlocks[currentBlocks.location[0].x + xShift + currentTurn[0].x][currentBlocks.location[0].y + yShift + currentTurn[0].y]->testBoard() &&
			!renderScreenBlocks[currentBlocks.location[1].x + xShift + currentTurn[1].x][currentBlocks.location[1].y + yShift + currentTurn[1].y]->testBoard() &&
			!renderScreenBlocks[currentBlocks.location[2].x + xShift + currentTurn[2].x][currentBlocks.location[2].y + yShift + currentTurn[2].y]->testBoard() &&
			!renderScreenBlocks[currentBlocks.location[3].x + xShift + currentTurn[3].x][currentBlocks.location[3].y + yShift + currentTurn[3].y]->testBoard())
	{
		currentBlocks.location[0].x += xShift + currentTurn[0].x;
		currentBlocks.location[0].y += yShift + currentTurn[0].y;
		currentBlocks.location[1].x += xShift + currentTurn[1].x;
		currentBlocks.location[1].y += yShift + currentTurn[1].y;
		currentBlocks.location[2].x += xShift + currentTurn[2].x;
		currentBlocks.location[2].y += yShift + currentTurn[2].y;
		currentBlocks.location[3].x += xShift + currentTurn[3].x;
		currentBlocks.location[3].y += yShift + currentTurn[3].y;
		currentBlocks.turn++;
		currentBlocks.turn = currentBlocks.turn%5;
		if (!currentBlocks.turn)
		{
			currentBlocks.turn = 1;
		}
		turned = true;
	}
    renderScreenBlocks[currentBlocks.location[0].x][currentBlocks.location[0].y]->setPieceType(currentBlocks.type);
    renderScreenBlocks[currentBlocks.location[1].x][currentBlocks.location[1].y]->setPieceType(currentBlocks.type);
    renderScreenBlocks[currentBlocks.location[2].x][currentBlocks.location[2].y]->setPieceType(currentBlocks.type);
    renderScreenBlocks[currentBlocks.location[3].x][currentBlocks.location[3].y]->setPieceType(currentBlocks.type);
	return turned;
}

void game::calculateNextBlocks(int startrange, int endrange) {
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, 7);
	int b;
	bool c;
	for (int i = startrange; i < endrange; i++) {
		b = distrib(gen);
		c = false;
		for (int k = startrange; k < endrange; k++) {
			if (b == nextBlock[k]) {
				c = true;
			}
		}
		if (c == true) {
			i--;
		}
		else {
			nextBlock[i] = (tetrisBlock::pieceType)b;
		}
	}
}

void game::cycleNextBlocks() {
	for (int i = 1; i < 14; i++) {
		nextBlock[i - 1] = nextBlock[i];
	}
	nextBlock[13] = tetrisBlock::pieceType::Tile;
	if (nextBlock[7] == NULL) {
		calculateNextBlocks(7, 14);
	}
    nextPieces[0].changePiece(nextBlock[0], currentScreenSize);
    nextPieces[1].changePiece(nextBlock[1], currentScreenSize);
    nextPieces[2].changePiece(nextBlock[2], currentScreenSize);
}