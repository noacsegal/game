#include "FileInput.h"
#include <iostream>

void FileInput::readNextStep()
{
	if (inputFile.is_open() && !inputFile.eof()) {
		if (inputFile >> nextStepCycle >> nextStepKey) {
			if (nextStepKey == '~') {
				std::getline(inputFile, nextStepString);
			}
			
			return;
		}
	}

	//end of file or error
	nextStepCycle = -1;
}

char FileInput::getInput(long currentCycle, player* players)
{
	//finds when to read the next key
	if (nextStepCycle != -1 && currentCycle == nextStepCycle) {
		char keyReturn = nextStepKey;
		readNextStep();
		return keyReturn;
	}
	//wrong time
	return 0;
}

//reads input for riddles
std::string FileInput::getString(long cycle)
{
	// If the file is waiting with a string at the correct time
	if (nextStepCycle != -1 && cycle == nextStepCycle && nextStepKey == '~') {
		std::string s = nextStepString;
		readNextStep(); // Prepare for the next move
		return s;
	}
	return "";
}