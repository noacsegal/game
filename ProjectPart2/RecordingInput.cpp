#include "RecordingInput.h"
#include "GameScreens.h"

char RecordingInput::getInput(long cycle, player* players)
{
	char c = keyBoardInput::getInput(cycle, players);

	//write the key in the file
	if (c != 0) {
		if (outputFile.is_open() && neededInput(players, c)) {
			// Format: [Cycle] [Key]
			outputFile << cycle << " " << c << std::endl;
		}
	}

	return c;
}

//makes sure the input is needed for the game play
bool RecordingInput::neededInput(player* players, char input)
{
	for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {
		player& p = players[i];

		char* keys = p.getKeys();
		char disposeChar = p.getDisposeChar();

		//check if its one of the direction keys
		for (int j = 0; j < player::NUM_KEYS; j++) {
			if (input == keys[j]) {
				return true;
			}
		}

		if (input == disposeChar) {
			return true;
		}
	}
	return false;
}

std::string RecordingInput::getString(long cycle)
{
	//Get input from keyboard - for riddles
	std::string s = keyBoardInput::getString(cycle);

	// Save it to file with special format: [Cycle] [~] [String]
	if (outputFile.is_open()) {
		outputFile << cycle << " ~" << s << std::endl;
	}
	return s;
}
