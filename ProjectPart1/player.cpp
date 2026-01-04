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


void player::move(Screen& currScreen, riddle& rid) {
	point target_pos = body;
	target_pos.move();
	int dest_x = target_pos.getX();
	int dest_y = target_pos.getY();

	// Get the char at the target position
	char charAtTarget = currScreen.getChar(target_pos);

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
			body.draw(' ');
			body.move();
			body.draw();
		}
	}


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


