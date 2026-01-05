//player.cpp

#include "player.h"
#include <cstdlib>
#include <cctype>
#include "direction.h"
#include <algorithm>
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


bool player::move(Screen& currScreen, riddle& rid) {
	point target_pos = body;
	target_pos.move();
	int dest_x = target_pos.getX();
	int dest_y = target_pos.getY();

	// Get the char at the target position
	char charAtTarget = currScreen.getChar(target_pos);
	// if player ran into a torch
	if (charAtTarget == TORCH) {
		//**************************************************************************************************

		if (heldType == ItemType::EMPTY) {

			heldType = ItemType::TORCH;
			heldBomb = nullptr;
			heldKey = nullptr;
			currScreen.setCharCurrent(target_pos, ' ');
			body.draw(' ');
			body.move();
			body.draw();
			return true;
		}
		else {
			currScreen.setCharCurrent(body, ' ');
			body.draw(' ');
			point temp = body;
			body.move();
			body.draw(); 

			currScreen.setCharCurrent(body, player::TORCH);
			temp.draw('!');
			return true;
		}
		//**************************************************************************************************

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
				//******************************************************************************************
				char charUnderPlayer = currScreen.getChar(body);

				body.draw(charUnderPlayer);
				
				body.changePosition(next_step);
				body.draw();
				//******************************************************************************************

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

	else if (charAtTarget == riddle::RIDDLE) {

		//return false if the player was wrong 3 times
		if (!rid.answerRiddle(*this, currScreen)) {
			lives--;
			heldType = ItemType::EMPTY;
		}
		body.move();
		currScreen.setCharCurrent(body, ' ');
		body.changeDir(Direction::directions[Direction::STAY]);

	}


	else if (charAtTarget == Screen::WALL) {
		body.changeDir(Direction::directions[Direction::STAY]);
	}

	
	else {
		Direction currentDir = body.getDir();

		if (currentDir.dirx != 0 || currentDir.diry != 0) {
			//***********************************************************************************
			char charUnderPlayer = currScreen.getChar(body);
			body.draw(charUnderPlayer);

			body.move();
			body.draw();
			//***********************************************************************************

		}
	}
	return true;

}

void player::keyPressed(char ch, Screen& currScreen) {
	size_t index = 0;
	Direction oldDir = body.getDir();
	bool correctKey = false;

	//when the key is one of the movment ones the direction of the player is changing accordingly
	for (char key : keys) {
		if (std::tolower(key) == std::tolower(ch)) {
			body.changeDir(Direction::directions[index]);
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


