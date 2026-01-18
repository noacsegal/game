#pragma once
#include "KeyboardInput.h"
#include <fstream>
#include <iostream>


class RecordingInput : public keyBoardInput
{
private:
	std::ofstream outputFile;

public:
	RecordingInput() {}
	virtual char getInput(long cycle) override;
	
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
};

