#pragma once
#include "point.h"
#include "screen.h"
#include "player.h"

class Bomb
{
private:
	point place;


public:
	Bomb() {}
	Bomb(point p) : place(p) {}
	point getPlace() const {
		return place;
	}

	void goOff(player& p);


};

