#include "../headers/game.h"
#include "../headers/scene.h"

const block::location game::gameSquareBoundary = {0.21, 0.3, 0.4, 0.74};
const float game::timing[] = {48.0f/60.0f, 43.0f/60.0f, 38.0f/60.0f, 33.0f/60.0f, 28.0f/60.0f, 23.0f/60.0f, 18.0f/60.0f, 13.0f/60.0f, 8.0f/60.0f, 6.0f/60.0f
					, 5.0f/60.0f, 5.0f/60.0f, 5.0f/60.0f, 4.0f/60.0f, 4.0f/60.0f, 4.0f/60.0f, 3.0f/60.0f, 3.0f/60.0f, 3.0f/60.0f};

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
	died(false), score(0), level(startingLevel), lines(0), effectGenerator(effectGenerator)
{
    for (int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            renderScreenBlocks[i][j] = new tetrisBlock(renderTarget, {0.1, 0.1, 0.1, 0.1}, screenSize);
            screenBlocks[i][j] = 0;
        }
    }
	for(int i = 0; i < 14; i++)
	{
		nextBlock[i] = 0;
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

void game::sendBlocks() {
	if ((screenBlocks[3][0] > 0 && screenBlocks[3][0] < 8) ||
		(screenBlocks[4][0] > 0 && screenBlocks[4][0] < 8) ||
		(screenBlocks[5][0] > 0 && screenBlocks[5][0] < 8) ||
		(screenBlocks[6][0] > 0 && screenBlocks[6][0] < 8) ||
		(screenBlocks[3][1] > 0 && screenBlocks[3][1] < 8) ||
		(screenBlocks[4][1] > 0 && screenBlocks[4][1] < 8) ||
		(screenBlocks[5][1] > 0 && screenBlocks[5][1] < 8) ||
		(screenBlocks[6][1] > 0 && screenBlocks[6][1] < 8)) {
        died = true;
        /*
		screen = DEATHSCREEN;
		calculateLeaderboard();
		if (songNumber == 1) {
			pmainSongBuffer->Stop();
		}
		else if (songNumber == 2) {
			pmainSong2Buffer->Stop();
		}
		else {
			pmainSong3Buffer->Stop();
		}
		pdeathSongBuffer->SetCurrentPosition(0);
		pdeathSongBuffer->Play(
			0,  // Unused.
			0,  // Priority for voice management.
			DSBPLAY_LOOPING); // Flags.
		return;
        */
	}
	if (nextBlock[0] == 1) {
		currentBlocks[0].x = 3;
		currentBlocks[0].y = 0;
		currentBlocks[0].type = 1;
		currentBlocks[0].turn = 1;
		currentBlocks[1].x = 4;
		currentBlocks[1].y = 0;
		currentBlocks[1].type = 1;
		currentBlocks[1].turn = 1;
		currentBlocks[2].x = 5;
		currentBlocks[2].y = 0;
		currentBlocks[2].type = 1;
		currentBlocks[2].turn = 1;
		currentBlocks[3].x = 6;
		currentBlocks[3].y = 0;
		currentBlocks[3].type = 1;
		currentBlocks[3].turn = 1;
	}
	else if (nextBlock[0] == 2) {
		currentBlocks[0].x = 3;
		currentBlocks[0].y = 0;
		currentBlocks[0].type = 2;
		currentBlocks[0].turn = 1;
		currentBlocks[1].x = 3;
		currentBlocks[1].y = 1;
		currentBlocks[1].type = 2;
		currentBlocks[1].turn = 1;
		currentBlocks[2].x = 4;
		currentBlocks[2].y = 1;
		currentBlocks[2].type = 2;
		currentBlocks[2].turn = 1;
		currentBlocks[3].x = 5;
		currentBlocks[3].y = 1;
		currentBlocks[3].type = 2;
		currentBlocks[3].turn = 1;
	}
	else if (nextBlock[0] == 3) {
		currentBlocks[0].x = 3;
		currentBlocks[0].y = 1;
		currentBlocks[0].type = 3;
		currentBlocks[0].turn = 1;
		currentBlocks[1].x = 4;
		currentBlocks[1].y = 1;
		currentBlocks[1].type = 3;
		currentBlocks[1].turn = 1;
		currentBlocks[2].x = 5;
		currentBlocks[2].y = 1;
		currentBlocks[2].type = 3;
		currentBlocks[2].turn = 1;
		currentBlocks[3].x = 5;
		currentBlocks[3].y = 0;
		currentBlocks[3].type = 3;
		currentBlocks[3].turn = 1;
	}
	else if (nextBlock[0] == 4) {
		currentBlocks[0].x = 4;
		currentBlocks[0].y = 0;
		currentBlocks[0].type = 4;
		currentBlocks[0].turn = 1;
		currentBlocks[1].x = 5;
		currentBlocks[1].y = 0;
		currentBlocks[1].type = 4;
		currentBlocks[1].turn = 1;
		currentBlocks[2].x = 4;
		currentBlocks[2].y = 1;
		currentBlocks[2].type = 4;
		currentBlocks[2].turn = 1;
		currentBlocks[3].x = 5;
		currentBlocks[3].y = 1;
		currentBlocks[3].type = 4;
		currentBlocks[3].turn = 1;
	}
	else if (nextBlock[0] == 5) {
		currentBlocks[0].x = 3;
		currentBlocks[0].y = 1;
		currentBlocks[0].type = 5;
		currentBlocks[0].turn = 1;
		currentBlocks[1].x = 4;
		currentBlocks[1].y = 1;
		currentBlocks[1].type = 5;
		currentBlocks[1].turn = 1;
		currentBlocks[2].x = 4;
		currentBlocks[2].y = 0;
		currentBlocks[2].type = 5;
		currentBlocks[2].turn = 1;
		currentBlocks[3].x = 5;
		currentBlocks[3].y = 0;
		currentBlocks[3].type = 5;
		currentBlocks[3].turn = 1;
	}
	else if (nextBlock[0] == 6) {
		currentBlocks[0].x = 3;
		currentBlocks[0].y = 1;
		currentBlocks[0].type = 6;
		currentBlocks[0].turn = 1;
		currentBlocks[1].x = 4;
		currentBlocks[1].y = 1;
		currentBlocks[1].type = 6;
		currentBlocks[1].turn = 1;
		currentBlocks[2].x = 5;
		currentBlocks[2].y = 1;
		currentBlocks[2].type = 6;
		currentBlocks[2].turn = 1;
		currentBlocks[3].x = 4;
		currentBlocks[3].y = 0;
		currentBlocks[3].type = 6;
		currentBlocks[3].turn = 1;
	}
	else if (nextBlock[0] == 7) {
		currentBlocks[0].x = 3;
		currentBlocks[0].y = 0;
		currentBlocks[0].type = 7;
		currentBlocks[0].turn = 1;
		currentBlocks[1].x = 4;
		currentBlocks[1].y = 0;
		currentBlocks[1].type = 7;
		currentBlocks[1].turn = 1;
		currentBlocks[2].x = 4;
		currentBlocks[2].y = 1;
		currentBlocks[2].type = 7;
		currentBlocks[2].turn = 1;
		currentBlocks[3].x = 5;
		currentBlocks[3].y = 1;
		currentBlocks[3].type = 7;
		currentBlocks[3].turn = 1;
	}
	screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = currentBlocks[0].type + 7;
	screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = currentBlocks[1].type + 7;
	screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = currentBlocks[2].type + 7;
	screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = currentBlocks[3].type + 7;
	renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(currentBlocks[0].type);
	renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(currentBlocks[1].type);
	renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(currentBlocks[2].type);
	renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(currentBlocks[3].type);
	count = setCount;
	canHold = true;
	cycleNextBlocks();
}

bool game::dropBlocks() {
	if ((screenBlocks[currentBlocks[0].x][currentBlocks[0].y + 1] == 0 || screenBlocks[currentBlocks[0].x][currentBlocks[0].y + 1] > 7)
		&& (screenBlocks[currentBlocks[1].x][currentBlocks[1].y + 1] == 0 || screenBlocks[currentBlocks[1].x][currentBlocks[1].y + 1] > 7)
		&& (screenBlocks[currentBlocks[2].x][currentBlocks[2].y + 1] == 0 || screenBlocks[currentBlocks[2].x][currentBlocks[2].y + 1] > 7)
		&& (screenBlocks[currentBlocks[3].x][currentBlocks[3].y + 1] == 0 || screenBlocks[currentBlocks[3].x][currentBlocks[3].y + 1] > 7)
		&& currentBlocks[0].y + 1 < 20 && currentBlocks[1].y + 1 < 20 && currentBlocks[2].y + 1 < 20 && currentBlocks[3].y + 1 < 20) {
		screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = 0;
		screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = 0;
		screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = 0;
		screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = 0;
        renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(0);
        renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(0);
        renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(0);
        renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(0);
		currentBlocks[0].y++;
		currentBlocks[1].y++;
		currentBlocks[2].y++;
		currentBlocks[3].y++;
		screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = currentBlocks[0].type + 7;
		screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = currentBlocks[0].type + 7;
		screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = currentBlocks[0].type + 7;
		screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = currentBlocks[0].type + 7;
        renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(currentBlocks[0].type);
        renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(currentBlocks[1].type);
        renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(currentBlocks[2].type);
        renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(currentBlocks[3].type);
		count = setCount;
		return true;
	}
	else {
		screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = currentBlocks[0].type;
		screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = currentBlocks[1].type;
		screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = currentBlocks[2].type;
		screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = currentBlocks[3].type;
        renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(currentBlocks[0].type);
        renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(currentBlocks[1].type);
        renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(currentBlocks[2].type);
        renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(currentBlocks[3].type);
		checkLines();
		effectGenerator.playEffectSound();
		sendBlocks();
		return false;
	}
}

void game::findGhost() {
	bool isRunning = true;
	if (screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y] == 15) {
		screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y] = 0;
        renderScreenBlocks[ghostBlocks[0].x][ghostBlocks[0].y]->changeColor(0);
	}
	if (screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y] == 15) {
		screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y] = 0;
        renderScreenBlocks[ghostBlocks[1].x][ghostBlocks[1].y]->changeColor(0);
	}
	if (screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y] == 15) {
		screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y] = 0;
        renderScreenBlocks[ghostBlocks[2].x][ghostBlocks[2].y]->changeColor(0);
	}
	if (screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y] == 15) {
		screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y] = 0;
        renderScreenBlocks[ghostBlocks[3].x][ghostBlocks[3].y]->changeColor(0);
	}
	ghostBlocks[0].x = currentBlocks[0].x;
	ghostBlocks[0].y = currentBlocks[0].y;
	ghostBlocks[1].x = currentBlocks[1].x;
	ghostBlocks[1].y = currentBlocks[1].y;
	ghostBlocks[2].x = currentBlocks[2].x;
	ghostBlocks[2].y = currentBlocks[2].y;
	ghostBlocks[3].x = currentBlocks[3].x;
	ghostBlocks[3].y = currentBlocks[3].y;
	while (isRunning) {
		if ((screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y + 1] == 0 || screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y + 1] > 7)
			&& (screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y + 1] == 0 || screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y + 1] > 7)
			&& (screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y + 1] == 0 || screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y + 1] > 7)
			&& (screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y + 1] == 0 || screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y + 1] > 7)
			&& ghostBlocks[0].y + 1 < 20 && ghostBlocks[1].y + 1 < 20 && ghostBlocks[2].y + 1 < 20 && ghostBlocks[3].y + 1 < 20) {
			if (screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y] == 15) {
				screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y] = 0;
                renderScreenBlocks[ghostBlocks[0].x][ghostBlocks[0].y]->changeColor(0);
			}
			if (screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y] == 15) {
				screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y] = 0;
                renderScreenBlocks[ghostBlocks[1].x][ghostBlocks[1].y]->changeColor(0);
			}
			if (screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y] == 15) {
				screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y] = 0;
                renderScreenBlocks[ghostBlocks[2].x][ghostBlocks[2].y]->changeColor(0);
			}
			if (screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y] == 15) {
				screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y] = 0;
                renderScreenBlocks[ghostBlocks[3].x][ghostBlocks[3].y]->changeColor(0);
			}
			ghostBlocks[0].y++;
			ghostBlocks[1].y++;
			ghostBlocks[2].y++;
			ghostBlocks[3].y++;
			if (screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y] == 0) {
				screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y] = ghostBlocks[0].type;
                renderScreenBlocks[ghostBlocks[0].x][ghostBlocks[0].y]->changeColor(ghostBlocks[0].type);
			}
			if (screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y] == 0) {
				screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y] = ghostBlocks[0].type;
                renderScreenBlocks[ghostBlocks[1].x][ghostBlocks[1].y]->changeColor(ghostBlocks[0].type);
			}
			if (screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y] == 0) {
				screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y] = ghostBlocks[0].type;
                renderScreenBlocks[ghostBlocks[2].x][ghostBlocks[2].y]->changeColor(ghostBlocks[0].type);
			}
			if (screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y] == 0) {
				screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y] = ghostBlocks[0].type;
                renderScreenBlocks[ghostBlocks[3].x][ghostBlocks[3].y]->changeColor(ghostBlocks[0].type);
			}
			isRunning = true;
		}
		else {
			if (screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y] == 0) {
				screenBlocks[ghostBlocks[0].x][ghostBlocks[0].y] = ghostBlocks[0].type;
                renderScreenBlocks[ghostBlocks[0].x][ghostBlocks[0].y]->changeColor(ghostBlocks[0].type);
			}
			if (screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y] == 0) {
				screenBlocks[ghostBlocks[1].x][ghostBlocks[1].y] = ghostBlocks[0].type;
                renderScreenBlocks[ghostBlocks[1].x][ghostBlocks[1].y]->changeColor(ghostBlocks[0].type);
			}
			if (screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y] == 0) {
				screenBlocks[ghostBlocks[2].x][ghostBlocks[2].y] = ghostBlocks[0].type;
                renderScreenBlocks[ghostBlocks[2].x][ghostBlocks[2].y]->changeColor(ghostBlocks[0].type);
			}
			if (screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y] == 0) {
				screenBlocks[ghostBlocks[3].x][ghostBlocks[3].y] = ghostBlocks[0].type;
                renderScreenBlocks[ghostBlocks[3].x][ghostBlocks[3].y]->changeColor(ghostBlocks[0].type);
			}
			isRunning = false;
		}
	}
}

void game::moveBlocks(int direction) {
	if ((screenBlocks[currentBlocks[0].x + direction][currentBlocks[0].y] == 0 || screenBlocks[currentBlocks[0].x + direction][currentBlocks[0].y] > 7)
		&& (screenBlocks[currentBlocks[1].x + direction][currentBlocks[1].y] == 0 || screenBlocks[currentBlocks[1].x + direction][currentBlocks[1].y] > 7)
		&& (screenBlocks[currentBlocks[2].x + direction][currentBlocks[2].y] == 0 || screenBlocks[currentBlocks[2].x + direction][currentBlocks[2].y] > 7)
		&& (screenBlocks[currentBlocks[3].x + direction][currentBlocks[3].y] == 0 || screenBlocks[currentBlocks[3].x + direction][currentBlocks[3].y] > 7)
		&& currentBlocks[0].x + direction <= 9 && currentBlocks[1].x + direction <= 9 && currentBlocks[2].x + direction <= 9 && currentBlocks[3].x + direction <= 9 &&
		currentBlocks[0].x + direction >= 0 && currentBlocks[1].x + direction >= 0 && currentBlocks[2].x + direction >= 0 && currentBlocks[3].x + direction >= 0) {

		screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = 0;
		screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = 0;
		screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = 0;
		screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = 0;
        renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(0);
        renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(0);
        renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(0);
        renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(0);
		currentBlocks[0].x += direction;
		currentBlocks[1].x += direction;
		currentBlocks[2].x += direction;
		currentBlocks[3].x += direction;
		screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = currentBlocks[0].type + 7;
		screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = currentBlocks[0].type + 7;
		screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = currentBlocks[0].type + 7;
		screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = currentBlocks[0].type + 7;
        renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(currentBlocks[0].type);
        renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(currentBlocks[1].type);
        renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(currentBlocks[2].type);
        renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(currentBlocks[3].type);
	}
}

void game::switchHold() {
	if (canHold == false) {
		return;
	}
    renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(0);
    renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(0);
    renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(0);
    renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(0);
	screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = 0;
	screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = 0;
	screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = 0;
	screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = 0;
	if (holding == 0) {
		holding = currentBlocks[0].type;
		sendBlocks();
	}
	else {
		if (holding == 1) {
			holding = currentBlocks[0].type;
			currentBlocks[0].x = 3;
			currentBlocks[0].y = 0;
			currentBlocks[0].type = 1;
			currentBlocks[0].turn = 1;
			currentBlocks[1].x = 4;
			currentBlocks[1].y = 0;
			currentBlocks[1].type = 1;
			currentBlocks[1].turn = 1;
			currentBlocks[2].x = 5;
			currentBlocks[2].y = 0;
			currentBlocks[2].type = 1;
			currentBlocks[2].turn = 1;
			currentBlocks[3].x = 6;
			currentBlocks[3].y = 0;
			currentBlocks[3].type = 1;
			currentBlocks[3].turn = 1;
		}
		else if (holding == 2) {
			holding = currentBlocks[0].type;
			currentBlocks[0].x = 3;
			currentBlocks[0].y = 0;
			currentBlocks[0].type = 2;
			currentBlocks[0].turn = 1;
			currentBlocks[1].x = 3;
			currentBlocks[1].y = 1;
			currentBlocks[1].type = 2;
			currentBlocks[1].turn = 1;
			currentBlocks[2].x = 4;
			currentBlocks[2].y = 1;
			currentBlocks[2].type = 2;
			currentBlocks[2].turn = 1;
			currentBlocks[3].x = 5;
			currentBlocks[3].y = 1;
			currentBlocks[3].type = 2;
			currentBlocks[3].turn = 1;
		}
		else if (holding == 3) {
			holding = currentBlocks[0].type;
			currentBlocks[0].x = 3;
			currentBlocks[0].y = 1;
			currentBlocks[0].type = 3;
			currentBlocks[0].turn = 1;
			currentBlocks[1].x = 4;
			currentBlocks[1].y = 1;
			currentBlocks[1].type = 3;
			currentBlocks[1].turn = 1;
			currentBlocks[2].x = 5;
			currentBlocks[2].y = 1;
			currentBlocks[2].type = 3;
			currentBlocks[2].turn = 1;
			currentBlocks[3].x = 5;
			currentBlocks[3].y = 0;
			currentBlocks[3].type = 3;
			currentBlocks[3].turn = 1;
		}
		else if (holding == 4) {
			holding = currentBlocks[0].type;
			currentBlocks[0].x = 4;
			currentBlocks[0].y = 0;
			currentBlocks[0].type = 4;
			currentBlocks[0].turn = 1;
			currentBlocks[1].x = 5;
			currentBlocks[1].y = 0;
			currentBlocks[1].type = 4;
			currentBlocks[1].turn = 1;
			currentBlocks[2].x = 4;
			currentBlocks[2].y = 1;
			currentBlocks[2].type = 4;
			currentBlocks[2].turn = 1;
			currentBlocks[3].x = 5;
			currentBlocks[3].y = 1;
			currentBlocks[3].type = 4;
			currentBlocks[3].turn = 1;
		}
		else if (holding == 5) {
			holding = currentBlocks[0].type;
			currentBlocks[0].x = 3;
			currentBlocks[0].y = 1;
			currentBlocks[0].type = 5;
			currentBlocks[0].turn = 1;
			currentBlocks[1].x = 4;
			currentBlocks[1].y = 1;
			currentBlocks[1].type = 5;
			currentBlocks[1].turn = 1;
			currentBlocks[2].x = 4;
			currentBlocks[2].y = 0;
			currentBlocks[2].type = 5;
			currentBlocks[2].turn = 1;
			currentBlocks[3].x = 5;
			currentBlocks[3].y = 0;
			currentBlocks[3].type = 5;
			currentBlocks[3].turn = 1;
		}
		else if (holding == 6) {
			holding = currentBlocks[0].type;
			currentBlocks[0].x = 3;
			currentBlocks[0].y = 1;
			currentBlocks[0].type = 6;
			currentBlocks[0].turn = 1;
			currentBlocks[1].x = 4;
			currentBlocks[1].y = 1;
			currentBlocks[1].type = 6;
			currentBlocks[1].turn = 1;
			currentBlocks[2].x = 5;
			currentBlocks[2].y = 1;
			currentBlocks[2].type = 6;
			currentBlocks[2].turn = 1;
			currentBlocks[3].x = 4;
			currentBlocks[3].y = 0;
			currentBlocks[3].type = 6;
			currentBlocks[3].turn = 1;
		}
		else if (holding == 7) {
			holding = currentBlocks[0].type;
			currentBlocks[0].x = 3;
			currentBlocks[0].y = 0;
			currentBlocks[0].type = 7;
			currentBlocks[0].turn = 1;
			currentBlocks[1].x = 4;
			currentBlocks[1].y = 0;
			currentBlocks[1].type = 7;
			currentBlocks[1].turn = 1;
			currentBlocks[2].x = 4;
			currentBlocks[2].y = 1;
			currentBlocks[2].type = 7;
			currentBlocks[2].turn = 1;
			currentBlocks[3].x = 5;
			currentBlocks[3].y = 1;
			currentBlocks[3].type = 7;
			currentBlocks[3].turn = 1;
		}
	}
	screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = currentBlocks[0].type + 7;
	screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = currentBlocks[1].type + 7;
	screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = currentBlocks[2].type + 7;
	screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = currentBlocks[3].type + 7;
    renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(currentBlocks[0].type);
    renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(currentBlocks[1].type);
    renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(currentBlocks[2].type);
    renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(currentBlocks[3].type);
    holdPiece.changePiece(holding, currentScreenSize);
	canHold = false;
}

void game::checkLines() {
	int tempLines = 0;
	for (int i = 19; i >= 0; i--) {
		if (screenBlocks[0][i] > 0 && screenBlocks[1][i] > 0 && screenBlocks[2][i] > 0 && screenBlocks[3][i] > 0 &&
			screenBlocks[4][i] > 0 && screenBlocks[5][i] > 0 && screenBlocks[6][i] > 0 && screenBlocks[7][i] > 0 &&
			screenBlocks[8][i] > 0 && screenBlocks[9][i] > 0) {
			for (int k = i; k > 0; k--) {
				screenBlocks[0][k] = screenBlocks[0][k - 1];
				screenBlocks[1][k] = screenBlocks[1][k - 1];
				screenBlocks[2][k] = screenBlocks[2][k - 1];
				screenBlocks[3][k] = screenBlocks[3][k - 1];
				screenBlocks[4][k] = screenBlocks[4][k - 1];
				screenBlocks[5][k] = screenBlocks[5][k - 1];
				screenBlocks[6][k] = screenBlocks[6][k - 1];
				screenBlocks[7][k] = screenBlocks[7][k - 1];
				screenBlocks[8][k] = screenBlocks[8][k - 1];
				screenBlocks[9][k] = screenBlocks[9][k - 1];
                renderScreenBlocks[0][k]->changeColor(screenBlocks[0][k]);
                renderScreenBlocks[1][k]->changeColor(screenBlocks[1][k]);
                renderScreenBlocks[2][k]->changeColor(screenBlocks[2][k]);
                renderScreenBlocks[3][k]->changeColor(screenBlocks[3][k]);
                renderScreenBlocks[4][k]->changeColor(screenBlocks[4][k]);
                renderScreenBlocks[5][k]->changeColor(screenBlocks[5][k]);
                renderScreenBlocks[6][k]->changeColor(screenBlocks[6][k]);
                renderScreenBlocks[7][k]->changeColor(screenBlocks[7][k]);
                renderScreenBlocks[8][k]->changeColor(screenBlocks[8][k]);
                renderScreenBlocks[9][k]->changeColor(screenBlocks[9][k]);

			}
			screenBlocks[0][0] = 0;
			screenBlocks[1][0] = 0;
			screenBlocks[2][0] = 0;
			screenBlocks[3][0] = 0;
			screenBlocks[4][0] = 0;
			screenBlocks[5][0] = 0;
			screenBlocks[6][0] = 0;
			screenBlocks[7][0] = 0;
			screenBlocks[8][0] = 0;
			screenBlocks[9][0] = 0;
            renderScreenBlocks[0][0]->changeColor(0);
            renderScreenBlocks[1][0]->changeColor(0);
            renderScreenBlocks[2][0]->changeColor(0);
            renderScreenBlocks[3][0]->changeColor(0);
            renderScreenBlocks[4][0]->changeColor(0);
            renderScreenBlocks[5][0]->changeColor(0);
            renderScreenBlocks[6][0]->changeColor(0);
            renderScreenBlocks[7][0]->changeColor(0);
            renderScreenBlocks[8][0]->changeColor(0);
            renderScreenBlocks[9][0]->changeColor(0);
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
	//pmainSongBuffer->SetFrequency(44100 + lines * 100);
}

bool game::turnBlocks(int xShift, int yShift) {
    bool turned = false;
    screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = 0;
	screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = 0;
	screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = 0;
	screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = 0;
    renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(0);
    renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(0);
    renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(0);
    renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(0);
	if (currentBlocks[0].type == 1) {
		if (currentBlocks[0].turn == 1) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 2][currentBlocks[0].y + yShift - 1] == 0 || screenBlocks[currentBlocks[0].x + xShift + 2][currentBlocks[0].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 1] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift + 2] == 0 || screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift + 2] > 7)
				&& currentBlocks[0].y + yShift - 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift + 1 < 20 && currentBlocks[3].y + yShift + 2 < 20
				&& currentBlocks[0].y + yShift - 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift + 1 >= 0 && currentBlocks[3].y + yShift + 2 >= 0
				&& currentBlocks[0].x + xShift + 2 < 10 && currentBlocks[1].x + xShift + 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift - 1 < 10
				&& currentBlocks[0].x + xShift + 2 >= 0 && currentBlocks[1].x + xShift + 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift - 1 >= 0) {
				currentBlocks[0].x += xShift + 2;
				currentBlocks[0].y += yShift + -1;
				currentBlocks[1].x += xShift + 1;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 1;
				currentBlocks[3].x += xShift + -1;
				currentBlocks[3].y += yShift + 2;
				currentBlocks[0].turn = 2;
				currentBlocks[1].turn = 2;
				currentBlocks[2].turn = 2;
				currentBlocks[3].turn = 2;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 2) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift + 2] == 0 || screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift + 2] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 1] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 2][currentBlocks[3].y + yShift - 1] == 0 || screenBlocks[currentBlocks[3].x + xShift - 2][currentBlocks[3].y + yShift - 1] > 7)
				&& currentBlocks[0].y + yShift + 2 < 20 && currentBlocks[1].y + yShift + 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift - 1 < 20
				&& currentBlocks[0].y + yShift + 2 >= 0 && currentBlocks[1].y + yShift + 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift - 1 >= 0
				&& currentBlocks[0].x + xShift + 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift - 1 < 10 && currentBlocks[3].x + xShift - 2 < 10
				&& currentBlocks[0].x + xShift + 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift - 1 >= 0 && currentBlocks[3].x + xShift - 2 >= 0) {
				currentBlocks[0].x += xShift + 1;
				currentBlocks[0].y += yShift + 2;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 1;
				currentBlocks[2].x += xShift + -1;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + -2;
				currentBlocks[3].y += yShift + -1;
				currentBlocks[0].turn = 3;
				currentBlocks[1].turn = 3;
				currentBlocks[2].turn = 3;
				currentBlocks[3].turn = 3;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 3) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 2][currentBlocks[0].y + yShift + 1] == 0 || screenBlocks[currentBlocks[0].x + xShift - 2][currentBlocks[0].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift - 1] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift - 2] == 0 || screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift - 2] > 7)
				&& currentBlocks[0].y + yShift + 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift - 1 < 20 && currentBlocks[3].y + yShift - 2 < 20
				&& currentBlocks[0].y + yShift + 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift - 1 >= 0 && currentBlocks[3].y + yShift - 2 >= 0
				&& currentBlocks[0].x + xShift - 2 < 10 && currentBlocks[1].x + xShift - 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift + 1 < 10
				&& currentBlocks[0].x + xShift - 2 >= 0 && currentBlocks[1].x + xShift - 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift + 1 >= 0) {
				currentBlocks[0].x += xShift + -2;
				currentBlocks[0].y += yShift + 1;
				currentBlocks[1].x += xShift + -1;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + -1;
				currentBlocks[3].x += xShift + 1;
				currentBlocks[3].y += yShift + -2;
				currentBlocks[0].turn = 4;
				currentBlocks[1].turn = 4;
				currentBlocks[2].turn = 4;
				currentBlocks[3].turn = 4;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 4) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift - 2] == 0 || screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift - 2] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift - 1] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 2][currentBlocks[3].y + yShift + 1] == 0 || screenBlocks[currentBlocks[3].x + xShift + 2][currentBlocks[3].y + yShift + 1] > 7)
				&& currentBlocks[0].y + yShift - 2 < 20 && currentBlocks[1].y + yShift - 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift + 1 < 20
				&& currentBlocks[0].y + yShift - 2 >= 0 && currentBlocks[1].y + yShift - 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift + 1 >= 0
				&& currentBlocks[0].x + xShift - 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift + 1 < 10 && currentBlocks[3].x + xShift + 2 < 10
				&& currentBlocks[0].x + xShift - 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift + 1 >= 0 && currentBlocks[3].x + xShift + 2 >= 0) {
				currentBlocks[0].x += xShift + -1;
				currentBlocks[0].y += yShift + -2;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + -1;
				currentBlocks[2].x += xShift + 1;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + 2;
				currentBlocks[3].y += yShift + 1;
				currentBlocks[0].turn = 1;
				currentBlocks[1].turn = 1;
				currentBlocks[2].turn = 1;
				currentBlocks[3].turn = 1;
				turned = true;
			}
		}
	}
	else if (currentBlocks[0].type == 2) {
		if (currentBlocks[0].turn == 1) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 2][currentBlocks[0].y + yShift + 0] == 0 || screenBlocks[currentBlocks[0].x + xShift + 2][currentBlocks[0].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift - 1] == 0 || screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift + 1] == 0 || screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift + 1] > 7)
				&& currentBlocks[0].y + yShift + 0 < 20 && currentBlocks[1].y + yShift - 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift + 1 < 20
				&& currentBlocks[0].y + yShift + 0 >= 0 && currentBlocks[1].y + yShift - 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift + 1 >= 0
				&& currentBlocks[0].x + xShift + 2 < 10 && currentBlocks[1].x + xShift + 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift - 1 < 10
				&& currentBlocks[0].x + xShift + 2 >= 0 && currentBlocks[1].x + xShift + 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift - 1 >= 0) {
				currentBlocks[0].x += xShift + 2;
				currentBlocks[0].y += yShift + 0;
				currentBlocks[1].x += xShift + 1;
				currentBlocks[1].y += yShift + -1;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + -1;
				currentBlocks[3].y += yShift + 1;
				currentBlocks[0].turn = 2;
				currentBlocks[1].turn = 2;
				currentBlocks[2].turn = 2;
				currentBlocks[3].turn = 2;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 2) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 0][currentBlocks[0].y + yShift + 2] == 0 || screenBlocks[currentBlocks[0].x + xShift + 0][currentBlocks[0].y + yShift + 2] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift + 1] == 0 || screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift - 1] == 0 || screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift - 1] > 7)
				&& currentBlocks[0].y + yShift + 2 < 20 && currentBlocks[1].y + yShift + 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift - 1 < 20
				&& currentBlocks[0].y + yShift + 2 >= 0 && currentBlocks[1].y + yShift + 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift - 1 >= 0
				&& currentBlocks[0].x + xShift + 0 < 10 && currentBlocks[1].x + xShift + 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift - 1 < 10
				&& currentBlocks[0].x + xShift + 0 >= 0 && currentBlocks[1].x + xShift + 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift - 1 >= 0) {
				currentBlocks[0].x += xShift + 0;
				currentBlocks[0].y += yShift + 2;
				currentBlocks[1].x += xShift + 1;
				currentBlocks[1].y += yShift + 1;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + -1;
				currentBlocks[3].y += yShift + -1;
				currentBlocks[0].turn = 3;
				currentBlocks[1].turn = 3;
				currentBlocks[2].turn = 3;
				currentBlocks[3].turn = 3;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 3) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 2][currentBlocks[0].y + yShift + 0] == 0 || screenBlocks[currentBlocks[0].x + xShift - 2][currentBlocks[0].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift + 1] == 0 || screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift - 1] == 0 || screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift - 1] > 7)
				&& currentBlocks[0].y + yShift + 0 < 20 && currentBlocks[1].y + yShift + 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift - 1 < 20
				&& currentBlocks[0].y + yShift + 0 >= 0 && currentBlocks[1].y + yShift + 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift - 1 >= 0
				&& currentBlocks[0].x + xShift - 2 < 10 && currentBlocks[1].x + xShift - 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift + 1 < 10
				&& currentBlocks[0].x + xShift - 2 >= 0 && currentBlocks[1].x + xShift - 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift + 1 >= 0) {
				currentBlocks[0].x += xShift + -2;
				currentBlocks[0].y += yShift + 0;
				currentBlocks[1].x += xShift + -1;
				currentBlocks[1].y += yShift + 1;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + 1;
				currentBlocks[3].y += yShift + -1;
				currentBlocks[0].turn = 4;
				currentBlocks[1].turn = 4;
				currentBlocks[2].turn = 4;
				currentBlocks[3].turn = 4;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 4) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 0][currentBlocks[0].y + yShift - 2] == 0 || screenBlocks[currentBlocks[0].x + xShift + 0][currentBlocks[0].y + yShift - 2] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift - 1] == 0 || screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift + 1] == 0 || screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift + 1] > 7)
				&& currentBlocks[0].y + yShift - 2 < 20 && currentBlocks[1].y + yShift - 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift + 1 < 20
				&& currentBlocks[0].y + yShift - 2 >= 0 && currentBlocks[1].y + yShift - 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift + 1 >= 0
				&& currentBlocks[0].x + xShift + 0 < 10 && currentBlocks[1].x + xShift - 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift + 1 < 10
				&& currentBlocks[0].x + xShift + 0 >= 0 && currentBlocks[1].x + xShift - 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift + 1 >= 0) {
				currentBlocks[0].x += xShift + 0;
				currentBlocks[0].y += yShift + -2;
				currentBlocks[1].x += xShift + -1;
				currentBlocks[1].y += yShift + -1;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + 1;
				currentBlocks[3].y += yShift + 1;
				currentBlocks[0].turn = 1;
				currentBlocks[1].turn = 1;
				currentBlocks[2].turn = 1;
				currentBlocks[3].turn = 1;
				turned = true;
			}
		}
	}
	else if (currentBlocks[0].type == 3) {
		if (currentBlocks[0].turn == 1) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift - 1] == 0 || screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift + 1] == 0 || screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 0][currentBlocks[3].y + yShift + 2] == 0 || screenBlocks[currentBlocks[3].x + xShift + 0][currentBlocks[3].y + yShift + 2] > 7)
				&& currentBlocks[0].y + yShift - 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift + 1 < 20 && currentBlocks[3].y + yShift + 2 < 20
				&& currentBlocks[0].y + yShift - 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift + 1 >= 0 && currentBlocks[3].y + yShift + 2 >= 0
				&& currentBlocks[0].x + xShift + 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift - 1 < 10 && currentBlocks[3].x + xShift + 0 < 10
				&& currentBlocks[0].x + xShift + 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift - 1 >= 0 && currentBlocks[3].x + xShift + 0 >= 0) {
				currentBlocks[0].x += xShift + 1;
				currentBlocks[0].y += yShift + -1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + -1;
				currentBlocks[2].y += yShift + 1;
				currentBlocks[3].x += xShift + 0;
				currentBlocks[3].y += yShift + 2;
				currentBlocks[0].turn = 2;
				currentBlocks[1].turn = 2;
				currentBlocks[2].turn = 2;
				currentBlocks[3].turn = 2;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 2) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift + 1] == 0 || screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift - 1] == 0 || screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 2][currentBlocks[3].y + yShift + 0] == 0 || screenBlocks[currentBlocks[3].x + xShift - 2][currentBlocks[3].y + yShift + 0] > 7)
				&& currentBlocks[0].y + yShift + 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift - 1 < 20 && currentBlocks[3].y + yShift + 0 < 20
				&& currentBlocks[0].y + yShift + 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift - 1 >= 0 && currentBlocks[3].y + yShift + 0 >= 0
				&& currentBlocks[0].x + xShift + 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift - 1 < 10 && currentBlocks[3].x + xShift - 2 < 10
				&& currentBlocks[0].x + xShift + 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift - 1 >= 0 && currentBlocks[3].x + xShift - 2 >= 0) {
				currentBlocks[0].x += xShift + 1;
				currentBlocks[0].y += yShift + 1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + -1;
				currentBlocks[2].y += yShift + -1;
				currentBlocks[3].x += xShift + -2;
				currentBlocks[3].y += yShift + 0;
				currentBlocks[0].turn = 3;
				currentBlocks[1].turn = 3;
				currentBlocks[2].turn = 3;
				currentBlocks[3].turn = 3;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 3) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift + 1] == 0 || screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift - 1] == 0 || screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 0][currentBlocks[3].y + yShift - 2] == 0 || screenBlocks[currentBlocks[3].x + xShift + 0][currentBlocks[3].y + yShift - 2] > 7)
				&& currentBlocks[0].y + yShift + 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift - 1 < 20 && currentBlocks[3].y + yShift - 2 < 20
				&& currentBlocks[0].y + yShift + 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift - 1 >= 0 && currentBlocks[3].y + yShift - 2 >= 0
				&& currentBlocks[0].x + xShift - 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift + 1 < 10 && currentBlocks[3].x + xShift + 0 < 10
				&& currentBlocks[0].x + xShift - 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift + 1 >= 0 && currentBlocks[3].x + xShift + 0 >= 0) {
				currentBlocks[0].x += xShift + -1;
				currentBlocks[0].y += yShift + 1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + 1;
				currentBlocks[2].y += yShift + -1;
				currentBlocks[3].x += xShift + 0;
				currentBlocks[3].y += yShift + -2;
				currentBlocks[0].turn = 4;
				currentBlocks[1].turn = 4;
				currentBlocks[2].turn = 4;
				currentBlocks[3].turn = 4;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 4) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift - 1] == 0 || screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift + 1] == 0 || screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 2][currentBlocks[3].y + yShift + 0] == 0 || screenBlocks[currentBlocks[3].x + xShift + 2][currentBlocks[3].y + yShift + 0] > 7)
				&& currentBlocks[0].y + yShift - 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift + 1 < 20 && currentBlocks[3].y + yShift + 0 < 20
				&& currentBlocks[0].y + yShift - 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift + 1 >= 0 && currentBlocks[3].y + yShift + 0 >= 0
				&& currentBlocks[0].x + xShift - 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift + 1 < 10 && currentBlocks[3].x + xShift + 2 < 10
				&& currentBlocks[0].x + xShift - 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift + 1 >= 0 && currentBlocks[3].x + xShift + 2 >= 0) {
				currentBlocks[0].x += xShift + -1;
				currentBlocks[0].y += yShift + -1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + 1;
				currentBlocks[2].y += yShift + 1;
				currentBlocks[3].x += xShift + 2;
				currentBlocks[3].y += yShift + 0;
				currentBlocks[0].turn = 1;
				currentBlocks[1].turn = 1;
				currentBlocks[2].turn = 1;
				currentBlocks[3].turn = 1;
				turned = true;
			}
		}
	}
	else if (currentBlocks[0].type == 5) {
		if (currentBlocks[0].turn == 1) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift - 1] == 0 || screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift + 1] == 0 || screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 0][currentBlocks[3].y + yShift + 2] == 0 || screenBlocks[currentBlocks[3].x + xShift + 0][currentBlocks[3].y + yShift + 2] > 7)
				&& currentBlocks[0].y + yShift - 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift + 1 < 20 && currentBlocks[3].y + yShift + 2 < 20
				&& currentBlocks[0].y + yShift - 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift + 1 >= 0 && currentBlocks[3].y + yShift + 2 >= 0
				&& currentBlocks[0].x + xShift + 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift + 1 < 10 && currentBlocks[3].x + xShift + 0 < 10
				&& currentBlocks[0].x + xShift + 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift + 1 >= 0 && currentBlocks[3].x + xShift + 0 >= 0) {
				currentBlocks[0].x += xShift + 1;
				currentBlocks[0].y += yShift + -1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + 1;
				currentBlocks[2].y += yShift + 1;
				currentBlocks[3].x += xShift + 0;
				currentBlocks[3].y += yShift + 2;
				currentBlocks[0].turn = 2;
				currentBlocks[1].turn = 2;
				currentBlocks[2].turn = 2;
				currentBlocks[3].turn = 2;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 2) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift + 1] == 0 || screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift + 1] == 0 || screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 2][currentBlocks[3].y + yShift + 0] == 0 || screenBlocks[currentBlocks[3].x + xShift - 2][currentBlocks[3].y + yShift + 0] > 7)
				&& currentBlocks[0].y + yShift + 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift + 1 < 20 && currentBlocks[3].y + yShift + 0 < 20
				&& currentBlocks[0].y + yShift + 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift + 1 >= 0 && currentBlocks[3].y + yShift + 0 >= 0
				&& currentBlocks[0].x + xShift + 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift - 1 < 10 && currentBlocks[3].x + xShift - 2 < 10
				&& currentBlocks[0].x + xShift + 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift - 1 >= 0 && currentBlocks[3].x + xShift - 2 >= 0) {
				currentBlocks[0].x += xShift + 1;
				currentBlocks[0].y += yShift + 1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + -1;
				currentBlocks[2].y += yShift + 1;
				currentBlocks[3].x += xShift + -2;
				currentBlocks[3].y += yShift + 0;
				currentBlocks[0].turn = 3;
				currentBlocks[1].turn = 3;
				currentBlocks[2].turn = 3;
				currentBlocks[3].turn = 3;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 3) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift + 1] == 0 || screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift - 1] == 0 || screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 0][currentBlocks[3].y + yShift - 2] == 0 || screenBlocks[currentBlocks[3].x + xShift + 0][currentBlocks[3].y + yShift - 2] > 7)
				&& currentBlocks[0].y + yShift + 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift - 1 < 20 && currentBlocks[3].y + yShift - 2 < 20
				&& currentBlocks[0].y + yShift + 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift - 1 >= 0 && currentBlocks[3].y + yShift - 2 >= 0
				&& currentBlocks[0].x + xShift - 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift - 1 < 10 && currentBlocks[3].x + xShift + 0 < 10
				&& currentBlocks[0].x + xShift - 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift - 1 >= 0 && currentBlocks[3].x + xShift + 0 >= 0) {
				currentBlocks[0].x += xShift + -1;
				currentBlocks[0].y += yShift + 1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + -1;
				currentBlocks[2].y += yShift + -1;
				currentBlocks[3].x += xShift + 0;
				currentBlocks[3].y += yShift + -2;
				currentBlocks[0].turn = 4;
				currentBlocks[1].turn = 4;
				currentBlocks[2].turn = 4;
				currentBlocks[3].turn = 4;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 4) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift - 1] == 0 || screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift - 1] == 0 || screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 2][currentBlocks[3].y + yShift + 0] == 0 || screenBlocks[currentBlocks[3].x + xShift + 2][currentBlocks[3].y + yShift + 0] > 7)
				&& currentBlocks[0].y + yShift - 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift - 1 < 20 && currentBlocks[3].y + yShift + 0 < 20
				&& currentBlocks[0].y + yShift - 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift - 1 >= 0 && currentBlocks[3].y + yShift + 0 >= 0
				&& currentBlocks[0].x + xShift - 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift + 1 < 10 && currentBlocks[3].x + xShift + 2 < 10
				&& currentBlocks[0].x + xShift - 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift + 1 >= 0 && currentBlocks[3].x + xShift + 2 >= 0) {
				currentBlocks[0].x += xShift + -1;
				currentBlocks[0].y += yShift + -1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + 1;
				currentBlocks[2].y += yShift + -1;
				currentBlocks[3].x += xShift + 2;
				currentBlocks[3].y += yShift + 0;
				currentBlocks[0].turn = 1;
				currentBlocks[1].turn = 1;
				currentBlocks[2].turn = 1;
				currentBlocks[3].turn = 1;
				turned = true;
			}
		}
	}
	else if (currentBlocks[0].type == 6) {
		if (currentBlocks[0].turn == 1) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift - 1] == 0 || screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift + 1] == 0 || screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift + 1] == 0 || screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift + 1] > 7)
				&& currentBlocks[0].y + yShift - 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift + 1 < 20 && currentBlocks[3].y + yShift + 1 < 20
				&& currentBlocks[0].y + yShift - 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift + 1 >= 0 && currentBlocks[3].y + yShift + 1 >= 0
				&& currentBlocks[0].x + xShift + 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift - 1 < 10 && currentBlocks[3].x + xShift + 1 < 10
				&& currentBlocks[0].x + xShift + 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift - 1 >= 0 && currentBlocks[3].x + xShift + 1 >= 0) {
				currentBlocks[0].x += xShift + 1;
				currentBlocks[0].y += yShift + -1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + -1;
				currentBlocks[2].y += yShift + 1;
				currentBlocks[3].x += xShift + 1;
				currentBlocks[3].y += yShift + 1;
				currentBlocks[0].turn = 2;
				currentBlocks[1].turn = 2;
				currentBlocks[2].turn = 2;
				currentBlocks[3].turn = 2;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 2) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift + 1] == 0 || screenBlocks[currentBlocks[0].x + xShift + 1][currentBlocks[0].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift - 1] == 0 || screenBlocks[currentBlocks[2].x + xShift - 1][currentBlocks[2].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift + 1] == 0 || screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift + 1] > 7)
				&& currentBlocks[0].y + yShift + 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift - 1 < 20 && currentBlocks[3].y + yShift + 1 < 20
				&& currentBlocks[0].y + yShift + 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift - 1 >= 0 && currentBlocks[3].y + yShift + 1 >= 0
				&& currentBlocks[0].x + xShift + 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift - 1 < 10 && currentBlocks[3].x + xShift - 1 < 10
				&& currentBlocks[0].x + xShift + 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift - 1 >= 0 && currentBlocks[3].x + xShift - 1 >= 0) {
				currentBlocks[0].x += xShift + 1;
				currentBlocks[0].y += yShift + 1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + -1;
				currentBlocks[2].y += yShift + -1;
				currentBlocks[3].x += xShift + -1;
				currentBlocks[3].y += yShift + 1;
				currentBlocks[0].turn = 3;
				currentBlocks[1].turn = 3;
				currentBlocks[2].turn = 3;
				currentBlocks[3].turn = 3;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 3) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift + 1] == 0 || screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift - 1] == 0 || screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift - 1] == 0 || screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift - 1] > 7)
				&& currentBlocks[0].y + yShift + 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift - 1 < 20 && currentBlocks[3].y + yShift - 1 < 20
				&& currentBlocks[0].y + yShift + 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift - 1 >= 0 && currentBlocks[3].y + yShift - 1 >= 0
				&& currentBlocks[0].x + xShift - 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift + 1 < 10 && currentBlocks[3].x + xShift - 1 < 10
				&& currentBlocks[0].x + xShift - 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift + 1 >= 0 && currentBlocks[3].x + xShift - 1 >= 0) {
				currentBlocks[0].x += xShift + -1;
				currentBlocks[0].y += yShift + 1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + 1;
				currentBlocks[2].y += yShift + -1;
				currentBlocks[3].x += xShift + -1;
				currentBlocks[3].y += yShift + -1;
				currentBlocks[0].turn = 4;
				currentBlocks[1].turn = 4;
				currentBlocks[2].turn = 4;
				currentBlocks[3].turn = 4;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 4) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift - 1] == 0 || screenBlocks[currentBlocks[0].x + xShift - 1][currentBlocks[0].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] == 0 || screenBlocks[currentBlocks[1].x + xShift + 0][currentBlocks[1].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift + 1] == 0 || screenBlocks[currentBlocks[2].x + xShift + 1][currentBlocks[2].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift + -1] == 0 || screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift - 1] > 7)
				&& currentBlocks[0].y + yShift - 1 < 20 && currentBlocks[1].y + yShift + 0 < 20 && currentBlocks[2].y + yShift + 1 < 20 && currentBlocks[3].y + yShift - 1 < 20
				&& currentBlocks[0].y + yShift - 1 >= 0 && currentBlocks[1].y + yShift + 0 >= 0 && currentBlocks[2].y + yShift + 1 >= 0 && currentBlocks[3].y + yShift - 1 >= 0
				&& currentBlocks[0].x + xShift - 1 < 10 && currentBlocks[1].x + xShift + 0 < 10 && currentBlocks[2].x + xShift + 1 < 10 && currentBlocks[3].x + xShift + 1 < 10
				&& currentBlocks[0].x + xShift - 1 >= 0 && currentBlocks[1].x + xShift + 0 >= 0 && currentBlocks[2].x + xShift + 1 >= 0 && currentBlocks[3].x + xShift + 1 >= 0) {
				currentBlocks[0].x += xShift + -1;
				currentBlocks[0].y += yShift + -1;
				currentBlocks[1].x += xShift + 0;
				currentBlocks[1].y += yShift + 0;
				currentBlocks[2].x += xShift + 1;
				currentBlocks[2].y += yShift + 1;
				currentBlocks[3].x += xShift + 1;
				currentBlocks[3].y += yShift + -1;
				currentBlocks[0].turn = 1;
				currentBlocks[1].turn = 1;
				currentBlocks[2].turn = 1;
				currentBlocks[3].turn = 1;
				turned = true;
			}
		}
	}
	else if (currentBlocks[0].type == 7) {
		if (currentBlocks[0].turn == 1) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 2][currentBlocks[0].y + yShift + 0] == 0 || screenBlocks[currentBlocks[0].x + xShift + 2][currentBlocks[0].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift + 1] == 0 || screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift + 1] == 0 || screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift + 1] > 7)
				&& currentBlocks[0].y + yShift + 0 < 20 && currentBlocks[1].y + yShift + 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift + 1 < 20
				&& currentBlocks[0].y + yShift + 0 >= 0 && currentBlocks[1].y + yShift + 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift + 1 >= 0
				&& currentBlocks[0].x + xShift + 2 < 10 && currentBlocks[1].x + xShift + 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift - 1 < 10
				&& currentBlocks[0].x + xShift + 2 >= 0 && currentBlocks[1].x + xShift + 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift - 1 >= 0) {
				currentBlocks[0].x += xShift + 2;
				currentBlocks[0].y += yShift + 0;
				currentBlocks[1].x += xShift + 1;
				currentBlocks[1].y += yShift + 1;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + -1;
				currentBlocks[3].y += yShift + 1;
				currentBlocks[0].turn = 2;
				currentBlocks[1].turn = 2;
				currentBlocks[2].turn = 2;
				currentBlocks[3].turn = 2;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 2) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 0][currentBlocks[0].y + yShift + 2] == 0 || screenBlocks[currentBlocks[0].x + xShift + 0][currentBlocks[0].y + yShift + 2] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift + 1] == 0 || screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift + 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift - 1] == 0 || screenBlocks[currentBlocks[3].x + xShift - 1][currentBlocks[3].y + yShift - 1] > 7)
				&& currentBlocks[0].y + yShift + 2 < 20 && currentBlocks[1].y + yShift + 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift - 1 < 20
				&& currentBlocks[0].y + yShift + 2 >= 0 && currentBlocks[1].y + yShift + 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift - 1 >= 0
				&& currentBlocks[0].x + xShift + 0 < 10 && currentBlocks[1].x + xShift - 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift - 1 < 10
				&& currentBlocks[0].x + xShift + 0 >= 0 && currentBlocks[1].x + xShift - 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift - 1 >= 0) {
				currentBlocks[0].x += xShift + 0;
				currentBlocks[0].y += yShift + 2;
				currentBlocks[1].x += xShift + -1;
				currentBlocks[1].y += yShift + 1;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + -1;
				currentBlocks[3].y += yShift + -1;
				currentBlocks[0].turn = 3;
				currentBlocks[1].turn = 3;
				currentBlocks[2].turn = 3;
				currentBlocks[3].turn = 3;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 3) {
			if ((screenBlocks[currentBlocks[0].x + xShift - 2][currentBlocks[0].y + yShift + 0] == 0 || screenBlocks[currentBlocks[0].x + xShift - 2][currentBlocks[0].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift - 1] == 0 || screenBlocks[currentBlocks[1].x + xShift - 1][currentBlocks[1].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift - 1] == 0 || screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift - 1] > 7)
				&& currentBlocks[0].y + yShift + 0 < 20 && currentBlocks[1].y + yShift - 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift - 1 < 20
				&& currentBlocks[0].y + yShift + 0 >= 0 && currentBlocks[1].y + yShift - 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift - 1 >= 0
				&& currentBlocks[0].x + xShift - 2 < 10 && currentBlocks[1].x + xShift - 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift + 1 < 10
				&& currentBlocks[0].x + xShift - 2 >= 0 && currentBlocks[1].x + xShift - 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift + 1 >= 0) {
				currentBlocks[0].x += xShift + -2;
				currentBlocks[0].y += yShift + 0;
				currentBlocks[1].x += xShift + -1;
				currentBlocks[1].y += yShift + -1;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + 1;
				currentBlocks[3].y += yShift + -1;
				currentBlocks[0].turn = 4;
				currentBlocks[1].turn = 4;
				currentBlocks[2].turn = 4;
				currentBlocks[3].turn = 4;
				turned = true;
			}
		}
		else if (currentBlocks[0].turn == 4) {
			if ((screenBlocks[currentBlocks[0].x + xShift + 0][currentBlocks[0].y + yShift - 2] == 0 || screenBlocks[currentBlocks[0].x + xShift + 0][currentBlocks[0].y + yShift - 2] > 7)
				&& (screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift - 1] == 0 || screenBlocks[currentBlocks[1].x + xShift + 1][currentBlocks[1].y + yShift - 1] > 7)
				&& (screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] == 0 || screenBlocks[currentBlocks[2].x + xShift + 0][currentBlocks[2].y + yShift + 0] > 7)
				&& (screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift + 1] == 0 || screenBlocks[currentBlocks[3].x + xShift + 1][currentBlocks[3].y + yShift + 1] > 7)
				&& currentBlocks[0].y + yShift - 2 < 20 && currentBlocks[1].y + yShift - 1 < 20 && currentBlocks[2].y + yShift + 0 < 20 && currentBlocks[3].y + yShift + 1 < 20
				&& currentBlocks[0].y + yShift - 2 >= 0 && currentBlocks[1].y + yShift - 1 >= 0 && currentBlocks[2].y + yShift + 0 >= 0 && currentBlocks[3].y + yShift + 1 >= 0
				&& currentBlocks[0].x + xShift + 0 < 10 && currentBlocks[1].x + xShift + 1 < 10 && currentBlocks[2].x + xShift + 0 < 10 && currentBlocks[3].x + xShift + 1 < 10
				&& currentBlocks[0].x + xShift + 0 >= 0 && currentBlocks[1].x + xShift + 1 >= 0 && currentBlocks[2].x + xShift + 0 >= 0 && currentBlocks[3].x + xShift + 1 >= 0) {
				currentBlocks[0].x += xShift + 0;
				currentBlocks[0].y += yShift + -2;
				currentBlocks[1].x += xShift + 1;
				currentBlocks[1].y += yShift + -1;
				currentBlocks[2].x += xShift + 0;
				currentBlocks[2].y += yShift + 0;
				currentBlocks[3].x += xShift + 1;
				currentBlocks[3].y += yShift + 1;
				currentBlocks[0].turn = 1;
				currentBlocks[1].turn = 1;
				currentBlocks[2].turn = 1;
				currentBlocks[3].turn = 1;
				turned = true;
			}
		}
	}
	screenBlocks[currentBlocks[0].x][currentBlocks[0].y] = currentBlocks[0].type + 7;
	screenBlocks[currentBlocks[1].x][currentBlocks[1].y] = currentBlocks[0].type + 7;
	screenBlocks[currentBlocks[2].x][currentBlocks[2].y] = currentBlocks[0].type + 7;
	screenBlocks[currentBlocks[3].x][currentBlocks[3].y] = currentBlocks[0].type + 7;
    renderScreenBlocks[currentBlocks[0].x][currentBlocks[0].y]->changeColor(currentBlocks[0].type);
    renderScreenBlocks[currentBlocks[1].x][currentBlocks[1].y]->changeColor(currentBlocks[1].type);
    renderScreenBlocks[currentBlocks[2].x][currentBlocks[2].y]->changeColor(currentBlocks[2].type);
    renderScreenBlocks[currentBlocks[3].x][currentBlocks[3].y]->changeColor(currentBlocks[3].type);
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
			nextBlock[i] = b;
		}
	}
}

void game::cycleNextBlocks() {
	for (int i = 1; i < 14; i++) {
		nextBlock[i - 1] = nextBlock[i];
	}
	nextBlock[13] = NULL;
	if (nextBlock[7] == NULL) {
		calculateNextBlocks(7, 14);
	}
    nextPieces[0].changePiece(nextBlock[0], currentScreenSize);
    nextPieces[1].changePiece(nextBlock[1], currentScreenSize);
    nextPieces[2].changePiece(nextBlock[2], currentScreenSize);
}
