//player.h
#pragma once
#include "screen.h"
#include "point.h"
#include "Key.h"
#include "Bomb.h"
#include "utillities.h"

class riddle;

class player
{
public:
	static constexpr size_t NUM_KEYS = 5;
	static constexpr char OBSTACLE = '*';
	static constexpr char SPRING = '#';
	static constexpr char TORCH = '!';
	int compressedCount = 0;
	int launchSpeed = 0;
	int launchTimer = 0;
	Direction launchDir = { 0,0 };
	std::vector<point> springPath;


private:
	// each player has: an array of keys, screen, body and a seperate key to dispose elements
	char keys[NUM_KEYS];
	point body;
	char dispose;
	int riddle_trys_left = 3;
	int lives = 3;
	ItemType heldType = ItemType::EMPTY;
	key* heldKey = nullptr;
	Bomb* heldBomb = nullptr;

public:

	player(const point& point, const char(&the_keys)[NUM_KEYS + 1], char dis_key)
		: body(point), dispose(dis_key) {
		memcpy(keys, the_keys, sizeof(keys[0]) * (NUM_KEYS));
	}

	//player functions
	//once the direction is decided the player moves in this direction until anthor one is chosen
	bool move(Screen& currScreen, riddle& rid);
	void draw();

	void draw(char ch);
	void keyPressed(char ch, Screen& currScreen);//moves player in diffrent directions



	point getBody() {
		return body;
	}


	point& bodyToChange() {
		return body;
	}

	bool lowerLives();

	key* changeKey() {
		return heldKey;
	}

	Bomb* changeBomb() {
		return heldBomb;
	}

	ItemType getItemType() {
		return heldType;
	}

	void updateKey(key* k) {
		heldKey = k;
	}

	void updateBomb(Bomb* b) {
		heldBomb = b;
	}

	void updateItemType(ItemType type) {
		heldType = type;
	}

	char getDisposeChar() const {
		return dispose;
	}

	void changeBodyChar(char c) {
		body.changeChar(c);
	}

	void moveScreen(point p) {
		body.changePosition(p);
	}

	int getNumLives() const {
		return lives;
	}

	int RiddleTrysByRef()const {
		return riddle_trys_left;
	}

	bool hasTorch() const {
		return heldType == ItemType::TORCH;
	}

	void changeHeld() {
		heldBomb = nullptr;
		heldKey = nullptr;
	}

	char* getKeys() {
		return keys;
	}

	char getDisposeChar() {
		return dispose;
	}
};