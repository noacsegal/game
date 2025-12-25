#pragma once
#include "point.h"
#include "screen.h"
#include "utillities.h"

class player;
class Screen;

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
	point& getPlaceP() {
		return place;
	}

	void goOff(player& p1, player& p2, Screen& currScreen);

	void setTaken(bool flag) {
		taken = flag;
	}

	bool getTaken() {
		return taken;
	}

	void turnOn() {
		ticking = true;
		timer = 60;//so it takes 3 seconds to explode
	}

	bool isTicking() const{
		return ticking;
	}

	void countdown(player& p1, player& p2, Screen& currScreen);


};

