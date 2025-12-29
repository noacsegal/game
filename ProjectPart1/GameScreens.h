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
	static constexpr char NUM_OF_GAME_SCREENS = 2;
	static constexpr char NUM_OF_PLAYERS = 2;

private:
	//data members
	//IMPORTANT: the gameScreens first and last index are not playing screens
	// the keys for gamescreens[i] is in screenKeys[i - 1] (because of the start screen)
	std::vector <Screen> gameScreens;
	std::vector <std::vector<key>> screenKeys; // vector of key arrays for each screen
	std::vector <std::vector<Door>> screenDoors;
	std::vector <std::vector<Switch>> screenSwitches;
	std::vector <std::vector<Bomb>> screenBombs;

public:
	GameScreens() {}
	void createScreenArray();
	Screen& changeScreeni(int i) {
		return gameScreens[i];
	}

	std::vector<key>& changeKeyArray(int i)  {
		return screenKeys[i];
	}

	std::vector<Switch>& changeSwitchArray(int i)  {
		return screenSwitches[i];
	}

	std::vector <Door>& changeDoori(int i) {
		return screenDoors[i];
	}

	std::vector <Bomb>& Bombi(int i) {
		return screenBombs[i];
	}

	void buildLevel(int i);


	static void printPlayorInventory(key* player1, key* player2);

};

