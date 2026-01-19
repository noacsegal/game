#pragma once
class player;

class Input
{

public:
	virtual char getInput(long cycle, player* players) = 0;
	virtual ~Input(){}
};

