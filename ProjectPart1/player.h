//player.h
#pragma once
#include "screen.h"
#include "point.h"
#include "Key.h"
#include "Bomb.h"

class player
{
public:
	static constexpr size_t NUM_KEYS = 5;
	static constexpr char OBSTACLE = '*';
	static constexpr char RIDDLE = '?';


private:
	// each player has: an array of keys, screen, body and a seperate key to dispose elements
	char keys[NUM_KEYS];
	point body;
	char dispose;
	int riddle_trys_left = 3;
	//ADD
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
	bool move(Screen& currScreen);//once the direction is decided the player moves in this direction until anthor one is chosen
	void draw();
	void draw(char ch);
	void keyPressed(char ch);//moves player in diffrent directions

	

	point getBody() {
		return body;
	}


	point& bodyToChange() {
		return body;
	}

	//ADD ON:

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
};




