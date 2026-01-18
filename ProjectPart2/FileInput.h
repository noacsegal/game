#pragma once
#include "Input.h"
#include <fstream>

class FileInput : public Input
{
private:
	std::ifstream inputFile;
	long nextStepCycle = -1;
	char nextStepKey = 0;

public:
	FileInput() {}

	void init() {
		if (!inputFile.is_open()) {
			inputFile.open("adv-world.steps");
			readNextStep(); // Read the first line immediately
		}
	}
	void readNextStep();

	virtual char getInput(long currentCycle) override;

	~FileInput() {
		if (inputFile.is_open()) inputFile.close();
	}
};

