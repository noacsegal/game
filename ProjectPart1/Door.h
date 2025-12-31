//door.h
#pragma once
#include "point.h"
#include "key.h"
#include "Switch.h"
#include "utillities.h"


class Door
{
public:
	static constexpr int NUM_KEYS_FOR_DOOR = 2;

private:
	point place;
	std::vector<key*> my_keys;
	std::vector<bool> keyUsed;
	std::vector<Switch*> my_switches; // switches that control the door
	std::vector<char> switch_states; // corresponding states for the switches
	int keyCounter = 0; // counts how many keys have reached the door
	int playerCounter = 0; //counts how many players have gone through the door

public:
	Door(point _place): place(_place) {}

	Door(point _place, std::vector<key*> keys, std::vector<Switch*> switches, std::vector<char> states) :
		place(_place), my_keys(keys), my_switches(switches), switch_states(states)  {
		keyUsed.resize(keys.size(), false);
	}

	void draw() {
		place.draw();
	}

	void intializekeyUsed();

	bool switchStatesCorrect();

	int getPlayerCounter() const {
		return playerCounter;
	}

	int getKeyCounter() const {
		return keyCounter;
	}

	size_t getNumOfKeysNeeded() const {
		return my_keys.size();
	}

	point getPlace() const {
		return place;
	}

	char GetDoorChar() const {
		return place.getChar();
	}


	bool canOpen(key* k);

	void addPlayer() {
		playerCounter++;
	}

	void addKey() {
		keyCounter++;
	}

	
};

