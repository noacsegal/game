#include "Spring.h"
#include "player.h"
#include "Screen.h"
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <windows.h>
#include <conio.h>

void Spring::handleCollision(point& body, Screen& currScreen, const point& target_pos, int& compressedCount, std::vector<point>& springPath) {
	compressedCount++;
	springPath.push_back(target_pos);
	body.draw(' ');
	currScreen.setCharCurrent(target_pos, ' ');
	body.move();
	body.draw();
}

void Spring::triggerLaunch(point& body, Screen& currScreen, int& compressedCount, int& launchSpeed, int& launchTimer, Direction& launchDir, std::vector<point>& springPath) {
	launchSpeed = compressedCount;
	launchTimer = compressedCount * compressedCount;
	launchDir.dirx = -body.getDir().dirx;
	launchDir.diry = -body.getDir().diry;
	for (auto& pos : springPath) {
		currScreen.setCharCurrent(pos, player::SPRING);
		pos.draw(player::SPRING);
	}
	body.draw();
	springPath.clear();
	compressedCount = 0;
	body.changeDir(Direction::directions[Direction::STAY]);
}


bool Spring::handleActiveLaunch(point& body,Screen& currScreen, int& launchSpeed, int& launchTimer, Direction& launchDir) {
	for (int i = 0; i < launchSpeed; ++i) {
		point next_step = body;
		next_step.changeDir(launchDir);
		next_step.move();
		char ahead = currScreen.getChar(next_step);
		if (ahead == Screen::WALL) {
			launchTimer = 0;
			break;
		}

		if (ahead == player::OBSTACLE) {
			int push_x = next_step.getX() + launchDir.dirx;
			int push_y = next_step.getY() + launchDir.diry;
			if (currScreen.isFree(push_x, push_y)) {
				point new_obs(push_x, push_y, Direction::directions[Direction::STAY], player::OBSTACLE);
				currScreen.setCharCurrent(new_obs, player::OBSTACLE);
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
			char charUnderPlayer = currScreen.getChar(body);

			body.draw(charUnderPlayer);

			body.changePosition(next_step);
			body.draw();
		}
	}
	launchTimer--;
	return true;
}

