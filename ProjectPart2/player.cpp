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
#include "Spring.h"
#include "Obstacle.h"
#include "Torch.h"
#include "Game.h"

//draw body character
void player::draw() {
	body.draw();
}

//draw different charcter (usually space)
void player::draw(char ch) {
	body.draw(ch);
}


bool player::move(Screen& currScreen, riddle& rid, Game& game) {
	point target_pos = body;
	target_pos.move();
	char charAtTarget = currScreen.getChar(target_pos);

	// Handle Launching
	if (launchTimer > 0) {
		return Spring::handleActiveLaunch(body, currScreen, launchSpeed, launchTimer, launchDir);
	}

	// Handle collisions based on the character hit
	switch (charAtTarget) {
	case TORCH:
		Torch::handleCollision(*this, body, currScreen, target_pos);
		break;

	case OBSTACLE:
		Obstacle::handleCollision(body, currScreen, target_pos);
		break;

	case SPRING:
		Spring::handleCollision(body, currScreen, target_pos, compressedCount, springPath);
		break;

	case riddle::RIDDLE:
		if (!rid.answerRiddle(*this, currScreen, game)) {
			lives--;
		}
		body.move();
		currScreen.setCharCurrent(body, ' ');
		body.changeDir(Direction::directions[Direction::STAY]);
		break;

	case Screen::WALL:
		if (compressedCount > 0) {
			Spring::triggerLaunch(body, currScreen, compressedCount, launchSpeed, launchTimer, launchDir, springPath);
		}
		else {
			body.changeDir(Direction::directions[Direction::STAY]);
		}
		break;

	default:
		// Standard move into empty space
		if (body.getDir().dirx != 0 || body.getDir().diry != 0) {
			char under = currScreen.getChar(body);
			body.draw(under);
			body.move();
			body.draw();
		}
		break;
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
			for (auto& pos : springPath) {
				currScreen.setCharCurrent(pos, SPRING);
				pos.draw(SPRING);
			}
			body.draw();
			springPath.clear();
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
			point lateral_pos = body;
			lateral_pos.changePosition({ body.getX() + newDir.dirx, body.getY() + newDir.diry, newDir, ' '});
			body.draw(' ');
			body.changePosition({ body.getX(), body.getY(), newDir, ' ' });
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