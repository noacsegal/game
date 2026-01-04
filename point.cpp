#include "point.h"
#include "screen.h"

//draws a specific character on the screen
void point::draw(char c) {
	gotoxy(x, y);
	std::cout << c;
}

//
void point::move() {
	x += dir.dirx;
	y += dir.diry;
}

void point::changePosition(point newp) {
	x = newp.getX();
	y = newp.getY();
}