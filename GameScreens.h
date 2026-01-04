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
	static constexpr char NUM_OF_PLAYERS = 2;

private:
	//data members
	std::vector <Screen> gameScreens;
	Screen startScreen;
	Screen endScreen;

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

	//added ***************************************************************************************************************
	size_t numOfScreens() {
		return gameScreens.size();
	}
};