#pragma once
#include "player.h"
#include "screen.h"
#include "GameScreens.h"
#include "keyBoardInput.h"
#include "RecordingInput.h"
#include "FileInput.h"

class Game
{
	int const radiusSq = 25; //the radius of the torch is 5
	Input* input; //parent class
	keyBoardInput keyboardPlay; //normal play
	RecordingInput recordingPlay; //saves all input to file steps
	FileInput filePlay; //takes input from file and not keyboard
	int sleepTime = 50;
	bool loadMode = false; // check if start and end screen are needed
	std::ofstream resultFile; //saves big events
	long currentCycle = 0; //tracks game timing for files
	std::vector<std::string> actualEvents; // Stores "Time: Event" strings
	bool silentMode = false;
	 
public:
	Game() {
		input = &recordingPlay; //default
	}
	enum KeyBoardKeys { ESC = 27, HOME = 104, SPACE = 32 };

	enum class levelStatus {
		NEXT_LEVEL,
		RESTARTFAIL,
		RESTARTKEY,
		EXIT
	};

	enum class gameType {
		KEYBOARD,
		RECORDING_KEYBOARD,
		FILE
	};

	void startGame();
	bool startingScreen(Screen* currScreenPtr);

	//add
	void handlePlayerMove(player& p, Screen* currScreenPtr, GameScreens& gs, bool& finished, player* players);

	void drawLevel(Screen* currScreenPtr, player* players, int indexScreen, bool* playerFinished);

	levelStatus handleInput(player* players, Screen* currScreenPtr, GameScreens& gs, long cycle);

	levelStatus playLevel(Screen* currScreen, player* players, GameScreens& gs, int indexScreen);

	void setGameMode(gameType type);

	//add improtant things to the result file
	void logEvent(std::string message);

	void logRiddleEvent(std::string riddle, std::string answer, bool isCorrect);

	std::string getAnswerInput() {
		return input->getString(currentCycle);
	}

	bool isLoadMode() const{
		return loadMode;
	}

	void setSilent(bool mode);

	void verifySilentResults();

private:
	gameType activeType;
};