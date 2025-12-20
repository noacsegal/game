//player.cpp

#include "player.h"
#include <cstdlib>
#include <cctype>
#include "direction.h"
#include <algorithm>
#include "riddle.h"
#include "GameScreens.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Bomb.h"

//draw body character
void player::draw() {
	body.draw();
}

//draw different charcter (usually space)
void player::draw(char ch) {
	body.draw(ch);
}


bool player::move(Screen& currScreen) {
	point target_pos = body;
	target_pos.move();

	int dest_x = target_pos.getX();
	int dest_y = target_pos.getY();

	// Get the char at the target position
	char charAtTarget = currScreen.getChar(dest_y, dest_x);

	// if player ran into an obstacle
	if (charAtTarget == OBSTACLE) {
		// Calculate where the obstacle needs to be pushed to
		Direction dir = body.getDir();
		int push_x = dest_x + dir.dirx;
		int push_y = dest_y + dir.diry;

		// check if we can move the obstacle
		if (currScreen.isFree(push_x, push_y)) {

			point obstacle_curr_loc = target_pos;
			obstacle_curr_loc.draw(' ');
			currScreen.setChar(obstacle_curr_loc, ' ');

			// Draw obstacle at new spot
			point obstacle_new_loc(push_x, push_y, Direction::directions[Direction::STAY], OBSTACLE);
			obstacle_new_loc.draw();
			currScreen.setChar(obstacle_new_loc, OBSTACLE);

			body.draw(' ');
			body.move();
			body.draw();

		}
		else {
			body.changeDir(Direction::directions[Direction::STAY]);
		}
	}


	else if (charAtTarget == Screen::WALL) {
		body.changeDir(Direction::directions[Direction::STAY]);
	}

	//if player will ran into a riddle
	else if (charAtTarget == RIDDLE) {

		//save the level as it is
		char current_screen[Screen::MAX_Y][Screen::MAX_X];
		key key1 = currScreen.changeScreenKeys()[0];
		key key2 = currScreen.changeScreenKeys()[1];
	

		for (int y = 0; y < Screen::MAX_Y; y++) {
			for (int x = 0; x < Screen::MAX_X; x++) {
				char charOnScreen = currScreen.getChar(y, x);
				current_screen[y][x] = charOnScreen;

				//check if we are on a key that has been picked up
				if (currScreen.getChar(y, x) == key::KEY) {

					//check which key we are on
					if (key1.getPlace().getX() == x && key1.getPlace().getY() == y && !key1.isTaken())
						current_screen[y][x] = key::KEY;
					else if (key2.getPlace().getX() == x && key2.getPlace().getY() == y && !key2.isTaken())
						current_screen[y][x] = key::KEY;
					if (key1.getPlace().getX() == x && key1.getPlace().getY() == y && key1.isTaken())
						current_screen[y][x] = ' ';
					else if (key2.getPlace().getX() == x && key2.getPlace().getY() == y && key2.isTaken())
						current_screen[y][x] = ' ';

				}
				else
					current_screen[y][x] = currScreen.getChar(y, x);

			}
			current_screen[y][Screen::MAX_X - 1] = '\0';
		}
		bool success = false;

		while (riddle_trys_left > 0 && !success) {
			int x = body.getX();
			int y = body.getY();
			current_screen[y][x] = RIDDLE;

			cls();
			std::cout << "you have " << riddle_trys_left << " trys" << std::endl;
			//ask the question and resive true if the answer is correct and false otherwise
			bool result = riddle::ask_riddle();
			cls();

			if (result) {
				//when the answer is correct print the level as it was
				std::cout << "You are correct!" << std::endl;
				Sleep(500);
				cls();
				success = true;
			}
			else {
				std::cout << "You are wrong :(" << std::flush;
				riddle_trys_left--;
				std::cout << "You have " << riddle_trys_left << " trys left" << std::endl;
				Sleep(1000);
			}
		}

		if (riddle_trys_left == 0) {
			//if we got 0 trys exit the level to the home screen
			std::cout << "you have failed, the game is reset";
			Sleep(1000);
			cls();
			return false;
		}
		else {
			// the answer was right
			point riddle_pos(dest_x, dest_y, Direction::directions[Direction::STAY], ' ');
			currScreen.setChar(riddle_pos, ' ');
			currScreen.setChar(body, ' ');
			current_screen[dest_y][dest_x] = ' ';
			cls();
			int y = 0;
			for (const auto& row : current_screen) {
				gotoxy(0, y++);
				std::cout << row << std::endl;
			}
		}
	}
	
	else {
		Direction currentDir = body.getDir();

		if (currentDir.dirx != 0 || currentDir.diry != 0) {
			body.draw(' ');
			body.move();
			body.draw();
		}
	}


	return true;
}

void player::keyPressed(char ch) {
	size_t index = 0;
	
	//when the key is one of the movment ones the direction of the player is changing accordingly
	for (char key : keys) {
		if (std::tolower(key) == std::tolower(ch)) {
			body.changeDir(Direction::directions[index]);
			break;
		}
		++index;
	}
}


//ADD ON :

//lower number of lives and return true if no more lives
bool player::lowerLives() {
	if (lives == 1) {
		return true;
	}

	else {
		lives--;

		//takes item
		heldType = ItemType::EMPTY;
	}
	return false;
}


