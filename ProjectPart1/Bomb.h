#pragma once
#include "point.h"
#include "screen.h"
#include "utillities.h"

class player;

class Bomb
{
public:
	static constexpr char BOMB = '@';
private:
	point place;
	int timer = -1;
	bool ticking = false;
	bool taken = false;

public:
	Bomb() {}
	Bomb(point p) : place(p) {}
	point getPlace() const {
		return place;
	}

	void goOff(player& p1, player& p2);

	void setTaken(bool flag) {
		taken = flag;
	}

	void turnOn() {
		ticking = true;
		timer = 5;
	}

	bool isTicking() const{
		return ticking;
	}

	void countdown(player& p1, player& p2);

};

