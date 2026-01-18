#pragma once
#include <vector>
class Direction;
class player;
class Screen;
class point;

class Spring
{
	
public:
	static void handleCollision(point& body, Screen& currScreen, const point& target_pos, int& compressedCount, std::vector<point>& springPath);
	static void triggerLaunch(point& body, Screen& currScreen, int& compressedCount, int& launchSpeed, int& launchTimer, Direction& launchDir, std::vector<point>& springPath);
	static bool handleActiveLaunch(point& body, Screen& currScreen, int& launchSpeed, int& launchTimer, Direction& launchDir);
};

