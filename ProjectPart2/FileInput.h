#pragma once
#include "Input.h"
#include <fstream>
#include <iostream>
#include "utillities.h"
#include <windows.h>
#include <conio.h>

//reads input from file instead of keyboard
class FileInput : public Input
{
private:
	std::ifstream inputFile;
	long nextStepCycle = -1;
	char nextStepKey = 0;
	std::string nextStepString = "";

public:
	FileInput() {}

	void init() {
		if (!inputFile.is_open()) {
			inputFile.open("adv-world.steps");
			readNextStep(); // Read the first line immediately
			
		}
	}
	void readNextStep();

	virtual char getInput(long currentCycle, player* players) override;

	~FileInput() {
		if (inputFile.is_open()) inputFile.close();
	}

	virtual std::string getString(long cycle) override;
};