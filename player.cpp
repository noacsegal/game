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
	char charAtTarget = currScreen.getChar(target_pos);

	// if player ran into a torch
	if (charAtTarget == TORCH) {
		if (heldType != ItemType::EMPTY) {
			char itemToDrop = ' ';
			if (heldType == ItemType::BOMB) itemToDrop = Bomb::BOMB;
			if (heldType == ItemType::KEY) itemToDrop = key::KEY;
			point dropPos(body.getX(), body.getY(), Direction::directions[Direction::STAY], itemToDrop);
			currScreen.setCharCurrent(dropPos, itemToDrop);
			dropPos.draw();
		}
		heldType = ItemType::TORCH;
		heldBomb = nullptr;
		heldKey = nullptr;
		currScreen.setCharCurrent(target_pos, ' ');
		body.draw(' ');
		body.move();
		body.draw();
		return true;
	}

	// if player ran into a spring
	if (launchTimer > 0) {
		for (int i = 0; i < launchSpeed; ++i) {
			point next_step = body;
			body.changeDir(launchDir);
			next_step.move();
			char ahead = currScreen.getChar(next_step.getY(), next_step.getX());
			if (ahead == Screen::WALL) {
				launchTimer = 0;
				break;
			}
			//need to add:: when there is 2 obstacle move slower by one and move them both
			if (ahead == OBSTACLE) {
				point push = next_step;
				int push_x = next_step.getX() + launchDir.dirx;
				int push_y = next_step.getY() + launchDir.diry;
				if (currScreen.isFree(push_x, push_y)) {
					point new_obs(push_x, push_y, Direction::directions[Direction::STAY], OBSTACLE);
					currScreen.setCharCurrent(new_obs, OBSTACLE);
					new_obs.draw();
					body.draw(' ');
					body.changePosition(next_step);
					body.draw();
					currScreen.setCharCurrent(next_step, ' ');
				}
				else {
					launchTimer = 0;
					break;
				}
			}
			else {
				body.draw(' ');
				body.changePosition(next_step);
				body.draw();
			}
		}
		launchTimer--;
		return true;
	}
	if (charAtTarget == SPRING) {
		compressedCount++;
		body.draw(' ');
		currScreen.setCharCurrent(target_pos, ' ');
		body.move();
		body.draw();
		return true;
	}
	if (compressedCount > 0 && charAtTarget == Screen::WALL) {
		launchSpeed = compressedCount;
		launchTimer = compressedCount * compressedCount;
		launchDir.dirx = -body.getDir().dirx;
		launchDir.diry = -body.getDir().diry;
		for (int j = 0; j < compressedCount; ++j) {
			int springX = body.getX() + (launchDir.dirx * j);
			int springY = body.getY() + (launchDir.diry * j);
			point springPos(springX, springY, Direction::directions[Direction::STAY], SPRING);
			currScreen.setCharCurrent(springPos, SPRING);
			springPos.draw();
		}
		body.draw();
		compressedCount = 0;
		body.changeDir(Direction::directions[Direction::STAY]);
		return true;
	}

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
			currScreen.setCharCurrent(obstacle_curr_loc, ' ');

			// Draw obstacle at new spot
			point obstacle_new_loc(push_x, push_y, Direction::directions[Direction::STAY], OBSTACLE);
			obstacle_new_loc.draw();
			currScreen.setCharCurrent(obstacle_new_loc, OBSTACLE);

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
		//key key1 = currScreen.changeScreenKeys()[0];
		//key key2 = currScreen.changeScreenKeys()[1];


		for (int y = 0; y < Screen::MAX_Y; y++) {
			for (int x = 0; x < Screen::MAX_X; x++) {
				char charOnScreen = currScreen.getChar(y, x);
				current_screen[y][x] = charOnScreen;

				//check if we are on a key that has been picked up
				//if (currScreen.getChar(y, x) == key::KEY) {

				//	//check which key we are on
				//	if (key1.getPlaceP().getX() == x && key1.getPlaceP().getY() == y && !key1.isTaken())
				//		current_screen[y][x] = key::KEY;
				//	else if (key2.getPlaceP().getX() == x && key2.getPlaceP().getY() == y && !key2.isTaken())
				//		current_screen[y][x] = key::KEY;
				//	if (key1.getPlaceP().getX() == x && key1.getPlaceP().getY() == y && key1.isTaken())
				//		current_screen[y][x] = ' ';
				//	else if (key2.getPlaceP().getX() == x && key2.getPlaceP().getY() == y && key2.isTaken())
				//		current_screen[y][x] = ' ';

				//}
				//else
				//	current_screen[y][x] = currScreen.getChar(y, x);

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
			currScreen.setCharCurrent(riddle_pos, ' ');
			currScreen.setCharCurrent(body, ' ');
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

void player::keyPressed(char ch, Screen& currScreen) {
	size_t index = 0;
	Direction oldDir = body.getDir();
	bool correctKey = false;

	for (char key : keys) {
		if (std::tolower(key) == std::tolower(ch)) {
			correctKey = true;
			break;
		}
		++index;
	}

	if (!correctKey) return;
	Direction newDir = Direction::directions[index];

	if (compressedCount > 0) {
		if (index == Direction::STAY || newDir.dirx != oldDir.dirx || newDir.diry != oldDir.diry) {
			launchSpeed = compressedCount;
			launchTimer = compressedCount * compressedCount;
			launchDir.dirx = -oldDir.dirx;
			launchDir.diry = -oldDir.diry;
			for (int j = 0; j < compressedCount; ++j) {
				int springX = body.getX() + (oldDir.dirx * -j);
				int springY = body.getY() + (oldDir.diry * -j);
				point springPos(springX, springY, Direction::directions[Direction::STAY], SPRING);
				currScreen.setCharCurrent(springPos, SPRING);
				springPos.draw();
			}
			body.draw();
			compressedCount = 0;
			body.changeDir(Direction::directions[Direction::STAY]);
			return;
		}
	}
	if (launchTimer > 0) {
		if (newDir.dirx == -launchDir.dirx && newDir.diry == -launchDir.diry) {
			return;
		}
		if ((newDir.dirx != 0 && launchDir.dirx == 0) || (newDir.diry != 0 && launchDir.diry == 0)) {
			point lateral_pos(body.getX() + newDir.dirx, body.getY() + newDir.diry, newDir, body.getChar());
			body.draw(' ');
			body.changeDir(newDir);
			body.draw();
		}
		return;
	}
	if (std::tolower(ch) == std::tolower(dispose)) {
		if (heldType != ItemType::EMPTY) {
			char charToDrop = ' ';
			if (heldType == ItemType::TORCH) charToDrop = TORCH;
			if (heldType == ItemType::BOMB) charToDrop = Bomb::BOMB;
			if (heldType == ItemType::KEY) charToDrop = key::KEY;

			currScreen.setCharCurrent(body, charToDrop);
			heldType = ItemType::EMPTY;
			heldBomb = nullptr;
			heldKey = nullptr;
			body.draw();
		}
		return;
	}

	body.changeDir(newDir);
}


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