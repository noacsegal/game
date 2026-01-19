//scren.cpp
#include "screen.h"
#include <iostream>
#include <cstring>
#include "Bomb.h"


void Screen::createScreenLine(const char* content, int i)
{
	strcpy_s(this->currentScreen[i], (MAX_X), content);
	strcpy_s(this->originalScreen[i], (MAX_X), content);

}

//draws the entire screen
void Screen::drawOriginal() const {
	int y = 0;
	for (const auto& row : originalScreen) {
		gotoxy(0, y++);
		std::cout << row << std::flush;
	}

}

void Screen::drawCurrent() const {
	int y = 0;
	for (const auto& row : currentScreen) {
		gotoxy(0, y++);
		std::cout << row << std::flush;
	}

	// 1. Draw Keys (if not taken)
	for (auto k : screenKeys) {
		k.draw();
	}

	// 2. Draw Switches (To show state changes / or \)
	for (auto sw : screenSwitches) {
		sw.draw();
	}

	// 3. Draw Doors
	for (const auto& d : screenDoors) {
		d.getPlace().draw();
	}

	for (auto b : screenBombs) {
		if (!b.getTaken())
			b.getPlaceP().draw();
	}
}

//go over the entire screen and save where the keys are
void Screen::createKeyArray()
{
	for (int row = 0; row < MAX_Y; row++) {
		for (int col = 0; col < MAX_X; col++) {
			point p = point(col, row, Direction::directions[Direction::STAY], originalScreen[row][col]);
			if (isCharOriginal(p, key::KEY)) {
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

			char ch = originalScreen[y][x];

			// Check if this character is one of the switch states
			if (ch == Switch::OPEN || ch == Switch::CLOSED) {
				// Save the location
				point p = { x, y, Direction::directions[Direction::STAY] , ch };
				Switch sw = { p, false };

				if (ch == Switch::OPEN) {
					sw.toggle();
				}
				screenSwitches.push_back(sw);
			}
		}
	}
}

void Screen::createBombArray()
{
	for (int row = 0; row < MAX_Y; row++) {
		for (int col = 0; col < MAX_X; col++) {
			point p = point(col, row, Direction::directions[Direction::STAY], originalScreen[row][col]);
			if (isCharOriginal(p, Bomb::BOMB)) {
				Bomb b = { p };
				screenBombs.push_back(b);
			}
		}
	}
}

void Screen::createDoorArray() {
	for (int row = 0; row < MAX_Y; row++) {
		for (int col = 0; col < MAX_X; col++) {
			char c = originalScreen[row][col];
			point p = point(col, row, Direction::directions[Direction::STAY], c);

			//check if c is a number
			if (c >= '1' && c <= '9') {
				Door d = { p };
				screenDoors.push_back(d);
			}
		}
	}
}

//gets a char that represents the char of the door and finds which place in the vector that door is
Door* Screen::getDoorID(char id)
{
	for (int i = 0; i < screenDoors.size(); i++) {
		if (screenDoors[i].GetDoorChar() == id)
			return &screenDoors[i];
	}
	return nullptr;//if not found
}

key* Screen::specificKeyRef(int i)
{
	if (i >= 0 && i < screenKeys.size())
		return &screenKeys[i];
	else
		return nullptr;
}

Switch* Screen::specificSwitchRef(int i)
{
	if (i >= 0 && i < screenSwitches.size())
		return &screenSwitches[i];
	else
		return nullptr;
}

void Screen::setDarkMode(bool dark, int rows, int cols) {
	this->isDark = dark;
	this->darkLimitRows = rows;
	this->darkLimitCols = cols;
}