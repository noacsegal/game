//screen.h
#pragma once
#include <vector>
#include "key.h"
#include "Switch.h"
#include "Door.h"
#include "Bomb.h"
#include "point.h"
class Bomb;

class Screen {
public:
	//size of screen
	static constexpr int MAX_X = 81; //length of row, number of collums
	static constexpr int MAX_Y = 25; //length of collum, number of rows
	static constexpr char WALL = 'W';



private:
	char originalScreen[MAX_Y][MAX_X + 1];
	char currentScreen[MAX_Y][MAX_X + 1];
	std::vector < key> screenKeys;
	std::vector <Switch> screenSwitches;
	std::vector <Door> screenDoors;
	std::vector <Bomb> screenBombs;
	point startPosPlayer1 = { 0,0, Direction::directions[Direction::STAY], ' ' };
	point startPosPlayer2 = { 0,0, Direction::directions[Direction::STAY], ' ' };
	point legendPos = { 0,0, Direction::directions[Direction::STAY], ' ' };
	bool isDark = false;//*********************************************************

	//return the char from a specific point of the  current screen
	char charAtCurrent(const point& p) const {
		return currentScreen[p.getY()][p.getX()];
	}
	char charAtOriginal(const point& p) const {
		return originalScreen[p.getY()][p.getX()];
	}

public:
	void createScreenLine(const char* content, int i);

	std::vector<Switch>& screenSwitchesByRef() {
		return screenSwitches;
	}

	std::vector<key>& screenKeysByRef() {
		return screenKeys;
	}

	std::vector <Bomb>& screenBombsByRef() {
		return screenBombs;
	}

	std::vector <Door>& screenDoorByRef() {
		return screenDoors;
	}

	char getChar(point& p) const {
		return currentScreen[p.getY()][p.getX()];
	}

	char getChar(int y, int x) const {
		return currentScreen[y][x];
	}

	//checks if point is the ch we are looking for
	bool isCharOriginal(const point& p, char ch) const {
		return charAtOriginal(p) == ch;
	}
	bool isCharCurrent(const point& p, char ch) const {
		return charAtCurrent(p) == ch;
	}

	//checks if a specific spot has a char
	bool isFree(int x, int y) const {
		char xy = currentScreen[y][x];
		return xy == ' ';
	}

	void draw() const;

	void createKeyArray();

	void createSwitchArray();

	void createBombArray();


	//puts a char onto the original screen 
	void setCharOriginal(const point& p, char ch) {
		if (p.getX() >= 0 && p.getX() < MAX_X && p.getY() >= 0 && p.getY() < MAX_Y) {
			originalScreen[p.getY()][p.getX()] = ch;
		}
	}
	//changes the current screen
	void setCharCurrent(const point& p, char ch) {
		if (p.getX() >= 0 && p.getX() < MAX_X && p.getY() >= 0 && p.getY() < MAX_Y) {
			currentScreen[p.getY()][p.getX()] = ch;
		}
	}

	point& player1posRef() {
		return startPosPlayer1;
	}
	point& player2posRef() {
		return startPosPlayer2;
	}

	void updatePlayer1Pos(point p) {
		startPosPlayer1 = p;
	}

	void updatePlayer2Pos(point p) {
		startPosPlayer2 = p;
	}

	void createDoorArray();

	Door* getDoorID(char id);

	key* specificKeyRef(int i);

	Switch* specificSwitchRef(int i);

	size_t getDoorVectorSize() {
		return screenDoors.size();
	}

	void updateLegendPos(point p) {
		legendPos = p;
	}

	point& legendPosByRef() {
		return legendPos;
	}
	//*****************************************************************************
	void setDarkness(bool status) { isDark = status; }
	bool getIsDark() const { return isDark; }
	void drawTile(int x, int y, bool lit);
	//*****************************************************************************

};