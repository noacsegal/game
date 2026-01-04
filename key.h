
#pragma once
#include "point.h"

class key
{
public:
	static constexpr char KEY = 'K';

private:
	point place;
	bool taken = false;

public:
	key(point p) : place(p) {}

	point& getPlaceP() {
		return place;
	}

	bool isTaken() const {
		return taken;
	}

	void changeTaken(bool new_t) {
		taken = new_t;
	}

	//draws the keys that aren't taken
	void draw() {
		if (!taken) {
			place.draw();
		}
	}


};