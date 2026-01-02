//GameScreen.h
#pragma once
#include "screen.h"
#include "key.h"
#include "Door.h"
#include "utillities.h"
#include "Bomb.h"



class GameScreens
{
public:
	static constexpr char NUM_OF_SCREENS = 4;
	static constexpr char NUM_OF_GAME_SCREENS = 4;
	static constexpr char NUM_OF_PLAYERS = 2;

private:
	//data members
	//IMPORTANT: the gameScreens first and last index are not playing screens
	std::vector <Screen> gameScreens;
	Screen startScreen;
	Screen endScreen;
	//*************************************************************************************************************************
	// PRETTY SURE WE NEVER ACTUALLY USE THIS
	//std::vector <std::vector<key>> screenKeys; // vector of key arrays for each screen
	//std::vector <std::vector<Door>> screenDoors;
	//std::vector <std::vector<Switch>> screenSwitches;
	//std::vector <std::vector<Bomb>> screenBombs;

public:
	GameScreens() {}
	bool LoadGameScreens();
	void fillAddedData(Screen& screen);

	Screen& ScreeniByRef(int i) {
		return gameScreens[i];
	}

	static void printPlayorInventory(point topLeft, player& p1, player& p2);

	void createStartAndEndScreen();

	Screen& endScreenByRef() {
		return endScreen;
	}

	Screen& startScreenByRef() {
		return startScreen;
	}
};

