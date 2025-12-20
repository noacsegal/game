
#include "screen.h"
#include <iostream>
#include "utillities.h"
#include <cstring>


void Screen::createScreen(const char** content)
{
	for (int i = 0; i < MAX_Y; i++)
	{
		strcpy_s(this->screen[i], (MAX_X), content[i]);
	}
}


//draws the entire screen
void Screen::draw() const {
	int y = 0;
	for (const auto& row : screen) {
		gotoxy(0, y++);
		std::cout << row << std::flush;
	}

}

//go over the entire screen and save where the keys are
void Screen::createKeyArray()
{
	for (int row = 0; row < MAX_Y; row++) {
		for (int col = 0; col < MAX_X; col++) {
			point p = point(col, row, Direction::directions[Direction::STAY], screen[row][col]);
			if (isChar(p, key::KEY)) {
				key k = { p };
				screenKeys.push_back(k);
			}
		}
	}
}

//function written by AI
void Screen::createSwitchArray()
{
	// Iterate rows (y)
	for (int y = 0; y < MAX_Y; ++y) {
		// Iterate columns (x)
		for (int x = 0; x < MAX_X; ++x) {

			char ch = screen[y][x];

			// Check if this character is one of the switch states
			if (ch == Switch::OPEN || ch == Switch::CLOSED) {
				// Save the location
				point p = { x, y, Direction::directions[Direction::STAY] , ch };
				Switch sw = { p, false };
				screenSwitches.push_back(sw);
			}
		}
	}
}




