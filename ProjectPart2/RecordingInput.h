#pragma once
#include "KeyboardInput.h"
#include <fstream>
#include <iostream>
#include "player.h"

//writes all input into a file 
// format: cycle key
class RecordingInput : public keyBoardInput
{
private:
	std::ofstream outputFile;

public:
	RecordingInput() {}

	virtual char getInput(long cycle, player* players) override;
	
	void init() {
		if (!outputFile.is_open()) {
			outputFile.open("adv-world.steps");
		}
	}

	~RecordingInput() {
		if (outputFile.is_open()) {
			outputFile.close();
		}
	}

	bool neededInput(player* players, char input);

	virtual std::string getString(long cycle) override;
};

