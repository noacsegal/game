#include "Obstacle.h"
#include "player.h"
#include "Screen.h"
#include "direction.h"

void Obstacle::handleCollision(point& body, Screen& currScreen, point& target_pos) {
    Direction dir = body.getDir();
    int push_x = target_pos.getX() + dir.dirx;
    int push_y = target_pos.getY() + dir.diry;

    if (currScreen.isFree(push_x, push_y)) {
        // Remove obstacle from old spot
        currScreen.setCharCurrent(target_pos, ' ');
        target_pos.draw(' ');

        // Place obstacle in new spot
        point new_loc(push_x, push_y, Direction::directions[Direction::STAY], player::OBSTACLE);
        new_loc.draw();
        currScreen.setCharCurrent(new_loc, player::OBSTACLE);

        // Move player into the now-empty spot
        body.draw(' ');
        body.move();
        body.draw();
    }
    else {
        body.changeDir(Direction::directions[Direction::STAY]);
    }
}
