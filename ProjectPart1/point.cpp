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

void point::changePosition(int new_x, int new_y, Direction new_direction) {
	x = new_x;
	y = new_y;
	dir = new_direction;
}


