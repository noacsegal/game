#pragma once
#include "direction.h"

// a class to be the foundation for everything
class point
{
	// each point has: (x, y), direction and a character
	int x = 0, y = 0;
	Direction dir = Direction::directions[Direction::STAY];
	char ch = '*';

public:
	point() {}
	//there is start point a direction (usely will be stay at the start) and charecter
	point(int x1, int y1, const Direction& the_dir, char c) : x(x1), y(y1), ch(c), dir(the_dir) {}
	point(int x1, int y1) : x(x1), y(y1) {}

	//draw, default is the character that the point is
	void draw() {
		draw(ch);
	}

	//draw a specific character
	void draw(char c);

	//return data members value
	int getX() const {
		return x;
	}
	int getY() const {
		return y;
	}
	char getChar() const {
		return ch;
	}
	Direction getDir() const {
		return dir;
	}

	//move in a new direction
	void changeDir(const Direction& new_dir) {
		dir = new_dir;
	}

	//for the switch
	void changeChar(char new_ch) {
		ch = new_ch;
	}

	void move();

	//change position when on new screen
	void changePosition(point newp);

	//function written by AI
	bool operator==(const point& other) const {
		return (x == other.x && y == other.y);
	}
};