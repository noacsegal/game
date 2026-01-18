#pragma once
#include "player.h"
#include "screen.h"
#include "GameScreens.h"
#include "keyBoardInput.h"
#include "RecordingInput.h"
#include "FileInput.h"

class Game
{

	int const darkMinX = 1;
	int const darkMaxX = 21;
	int const darkMinY = 1;
	int const darkMaxY = 9;
	bool inDarkZone = false;
	int const radiusSq = 25; //the radius of the torch is 5

	//add
	Input* input;
	keyBoardInput keyboardPlay;
	RecordingInput recordingPlay;
	FileInput filePlay;
	int sleepTime = 50;
	bool silentMode = false;

public:
	Game() {
		input = &recordingPlay; //default
	}
	enum KeyBoardKeys { ESC = 27, HOME = 104, SPACE = 32};
	
	enum class levelStatus {
		NEXT_LEVEL, 
		RESTARTFAIL,
		RESTARTKEY,
		EXIT
	};

	void startGame();
	void startingScreen();

	//add
	void handlePlayerMove(player& p, Screen* currScreenPtr, GameScreens& gs, bool& finished, player* players);

	void drawLevel(Screen* currScreenPtr, player* players, int indexScreen, bool* playerFinished);

	levelStatus handleInput(player* players, Screen* currScreenPtr, GameScreens& gs, long cycle);

	levelStatus playLevel(Screen* currScreen, player* players, GameScreens& gs, int indexScreen);
	
	void setGameMode(bool isLoadMode);

};