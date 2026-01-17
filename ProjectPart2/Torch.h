#include "point.h"

class player;
class Screen;

class Torch {
public:
    static void handleCollision(player& p, point& body, Screen& currScreen, const point& target_pos);
};
