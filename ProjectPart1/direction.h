#pragma once

class Direction {

	friend class point;
public:
	int dirx, diry;
	//     0     1      2     3     4         5
	enum { UP, RIGHT, DOWN, LEFT, STAY, NUM_DIRECTIONS };

	//
	static const Direction directions[NUM_DIRECTIONS];

	Direction(int dir_x, int dir_y)
		: dirx(dir_x), diry(dir_y) {
	}

	//check if same
	bool samedir(Direction check) {
		return (this->dirx == check.dirx && this->diry == check.diry);
	}

};

