//GameScreen.h
#pragma once
#include "screen.h"
#include "key.h"
#include "Door.h"
#include "utillities.h"
#include "Bomb.h"

//TO DO:decide how to choose which key for which door


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
	vector <Screen> gameScreens; 
	vector <vector<key>> screenKeys; // vector of key arrays for each screen
	vector <vector<Door>> screenDoors;
	vector <vector<Switch>> screenSwitches;
	vector <vector<Bomb>> screenBombs;

public:
	GameScreens() {}
	void createScreenArray();
	Screen& Screeni(int i) {
		return gameScreens[i];
	}

	vector<key>& KeyArray(int i)  {
		return screenKeys[i];
	}

	vector<Switch>& SwitchArray(int i)  {
		return screenSwitches[i];
	}

	vector <Door>& Doori(int i) {
		return screenDoors[i];
	}

	vector <Bomb>& Bombi(int i) {
		return screenBombs[i];
	}

	void buildLevel(int i);

	static key* nullkey() {
		key nullKey = { point(-1,-1, {Direction::directions[Direction::STAY]}, key::KEY) }; //a null key when player uses their key
		return &nullKey;
	}

	static bool isNullKey(key* check) {

		return (check->getPlace().getX() == nullkey()->getPlace().getX() &&
			check->getPlace().getY() == nullkey()->getPlace().getY());
	}

	static void printPlayorInventory(key* player1, key* player2);

};

