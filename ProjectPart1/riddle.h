#pragma once
#include "utillities.h"
#include "player.h"
#include "screen.h"

class riddle
{
	std::vector<std::string> riddles;
	std::vector<std::string> answers;
	std::vector<int> trys;
	int numRid = 0;

public:
	riddle(){}

	static constexpr char RIDDLE = '?';

	bool ask_riddle();

	void addRiddle(std::string rid, std::string ans);

	bool answerRiddle(player& p1, Screen& currscreen);

	
};


