#pragma once
#include "utillities.h"
#include <fstream>
#include "screen.h"

class files
{
	std::vector<std::string> fileNames;


public:
	static constexpr char END_SCREEN = 'Q';

	files() {}
	bool getAllScreenFileNames();

	std::vector<std::string> fileNameRef() {
		return fileNames;
	}

	bool createScreen(std::ifstream& screenFile, Screen& screenToFill);

	bool createMetaData(std::ifstream& screenFile, Screen& screenToFill);

	void errorFunction(std::string cause);

};



//file errors: 
//couldnt open
//no posotion for either players
//screen too big or too small
//no legend
//nothing after the screen - need at least one door