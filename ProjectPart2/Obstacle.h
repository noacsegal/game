#pragma once
#include "point.h"

class player; 
class Screen;

class Obstacle {
public:
    // Handles logic when a player tries to move into an obstacle
    static void handleCollision(point& body, Screen& currScreen, point& target_pos);
};