//player.h
#pragma once
#include "direction.h"
#include "utillities.h"
#include "screen.h"
#include "point.h"
#include "Key.h"

class Bomb;


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
	key* inventory;
	size_t num_collected = 0;
	int riddle_trys_left = 3;
	Bomb* invenBomb; //ADD
	int lives = 3;//ADD

public:

	player(const point& point, const char(&the_keys)[NUM_KEYS + 1], char dis_key, key* new_k)
		: body(point), dispose(dis_key), inventory(new_k){
		memcpy(keys, the_keys, sizeof(keys[0]) * (NUM_KEYS));
	}

	//player functions
	bool move(Screen& currScreen);//once the direction is decided the player moves in this direction until anthor one is chosen
	void draw();
	void draw(char ch);
	void keyPressed(char ch);//moves player in diffrent directions

	void updateKey(key* new_key) {
		inventory = new_key;
	}

	point getBody() {
		return body;
	}

	key* getInventory() {
		return inventory;
	}

	point& bodyToChange() {
		return body;
	}

	//ADD ON:

	bool lowerLives();

};
