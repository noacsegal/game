#pragma once
#include "player.h"
#include "screen.h"
#include "GameScreens.h"
#include "utillities.h"


class Game
{

public:
	enum KeyBoardKeys { ESC = 27, HOME = 104, SPACE = 32 };

	void startGame();
	void startingScreen();
};