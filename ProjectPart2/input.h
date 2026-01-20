#pragma once
#include <iostream>
#include <string>

class player;

class Input
{

public:
	virtual char getInput(long cycle, player* players) = 0;

	virtual std::string getString(long cycle) = 0;
	
	virtual ~Input(){}
};

