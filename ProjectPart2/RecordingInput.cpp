#include "RecordingInput.h"

char RecordingInput::getInput(long cycle)
{
	char c = keyBoardInput::getInput(cycle);

	//write the key in the file
	if (c != 0) {
		if (outputFile.is_open()) {
			// Format: [Cycle] [Key]
			outputFile << cycle << " " << c << std::endl;
		}
	}

	return c;
}
