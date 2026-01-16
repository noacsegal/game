#pragma once
#include "player.h"
#include "screen.h"
#include "GameScreens.h"
#include "input.h"

class Game
{

	int const darkMinX = 1, const darkMaxX = 21;
	int const darkMinY = 1, const darkMaxY = 9;
	bool inDarkZone = false;
	int const radiusSq = 25; //the radius of the torch is 5

	//add
	input* inp;

public:
	enum KeyBoardKeys { ESC = 27, HOME = 104, SPACE = 32 };
	
	enum class levelStatus {
		NEXT_LEVEL, 
		RESTARTFAIL,
		RESTARTKEY,
		EXIT        
	};

	void startGame();
	void startingScreen();

	//add
	levelStatus playLevel(Screen* currScreen, player* players, GameScreens& gs, int indexScreen);
};