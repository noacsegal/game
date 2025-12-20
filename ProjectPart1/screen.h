//screen.h
#pragma once
#include "Point.h"
#include "key.h"
#include "Door.h"
#include "Switch.h"


class Screen {
public:
	//size of screen
	static constexpr int MAX_X = 81; //length of row, number of collums
	static constexpr int MAX_Y = 24; //length of collum, number of rows
	static constexpr char WALL = 'W';



private:
	char screen[MAX_Y][MAX_X + 1];
	vector < key> screenKeys;
	vector <Switch> screenSwitches;
	vector <Door> screenDoors;
	vector <Bomb> screenBombs;

	//return the char from a specific point of the screen
	char charAt(const point& p) const {
		return screen[p.getY()][p.getX()];
	}

public:
	Screen() {}
	void createScreen(const char** content);

	vector<Switch>& ScreenSwitches() {
		return screenSwitches;
	}

	vector<key>& ScreenKeys() {
		return screenKeys;
	}

	vector <Bomb>& screenBombs() {
		return screenBombs;
	}

	char getChar(int y, int x) const {
		return screen[y][x];
	}

	//checks if point is the ch we are looking for
	bool isChar(const point& p, char ch) const {
		return charAt(p) == ch;
	}

	//checks if a specific spot has a char
	bool isFree(int x, int y) const {
		char xy = screen[y][x];
		return xy == ' ';
	}

	void draw() const;

	void createKeyArray();

	void createSwitchArray();

	
	//puts a char onto the screen array
	void setChar(const point& p, char ch) {
		if (p.getX() >= 0 && p.getX() < MAX_X && p.getY() >= 0 && p.getY() < MAX_Y) {
			screen[p.getY()][p.getX()] = ch;
		}
	}
};

