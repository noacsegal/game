#include "FileInput.h"
#include <iostream>

void FileInput::readNextStep()
{
	if (inputFile.is_open() && !inputFile.eof()) {
		if (inputFile >> nextStepCycle >> nextStepKey) {
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