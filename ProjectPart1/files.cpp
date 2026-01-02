#include "files.h"
#include "Screen.h"
#include "Game.h"
#include <conio.h>



//function from lab with changes- return false if no files were found
bool files::getAllScreenFileNames()
{
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 9) == "adv-world" && filename.extension() == ".screen") {
			fileNames.push_back(filenameStr);
		}
	}
	if (fileNames.empty()) {
		errorFunction("No files found");
		return false;
	}

	//sort the files 
	std::sort(fileNames.begin(), fileNames.end());
	
	return true;
}

//function from lab with improvments - return false if something was wrong with the file
// Q- shows when the screen is done in the file
bool files::createScreen(std::ifstream& screenFile, Screen& screenToFill){
	int curr_row = 0;
	int curr_col = 0;
	char c;
	
	//we are not at the end of the file or past the last row
	while (!screenFile.get(c) == END_SCREEN && curr_row < Screen::MAX_Y) {
		if (c == '\n') {

			//screen is too small
			if (curr_col < Screen::MAX_X) {
				errorFunction("Screen is too small");
				return false;
			}
			++curr_row;
			curr_col = 0;
			continue;
		}
		point p = { curr_col, curr_row, Direction::directions[Direction::STAY], ' ' };
		if (curr_col < Screen::MAX_X) {
			// check where the players are supposed to be
			if (c == '$') {
				screenToFill.updatePlayer1Pos(p);
			}
			else if (c == '&') {
				screenToFill.updatePlayer2Pos(p);
			}
			screenToFill.setChar(p, c);
		}
	}

	screenFile.close();
	if (curr_row < Screen::MAX_Y) {
		errorFunction("Screen is too small");
		return false;
	}
	return true;
}

bool files::createMetaData(std::ifstream& screenFile, Screen& screenToFill)
{
	std::string word;

	while (screenFile >> word && word != "DONE") {
		if (word == "DOOR") {
			int id, kCount, sCount;

			//makes sure we can read all the data
			if (!(screenFile >> id >> kCount >> sCount)) {
				errorFunction("Not enough meta data");
				return false;
			}

			Door* d;
			d = screenToFill.getDoorID(id);

			if (d == nullptr) {
				errorFunction("Invalid door");
				return false;
			}

			for (int i = 0; i < kCount; ++i) {

				//finds the key needed for the door and adds it to the doors vector
				int keyId;
				screenFile >> keyId;
				key* key = screenToFill.specificKeyRef(keyId);
				if (key == nullptr) {
					errorFunction("Invalid key");
					return false;
				}
				d->addKeyToVector(key);
			}

			for (int i = 0; i < sCount; ++i) {

				//finds the switch needed for the door and adds it to the doors vector
				int swId;
				screenFile >> swId;
				Switch* sw = screenToFill.specificSwitchRef(swId);
				if (sw == nullptr) {
					errorFunction("Invalid switch");
					return false;
				}
				d->addSwitchToVector(sw);
			}
		}
	}

	//if no doors where added
	if (screenToFill.getDoorVectorSize() == 0) {
		errorFunction("No door");
		return false;
	}

	return true;
}

void files::errorFunction(std::string cause)
{
	cls();
	std::cout << "Game wa stopped because: " << cause << std::endl;
	std::cout << "Press escape to exit the program" << std::endl;

	//wait for escape char
	while (true) {
		if (_kbhit()) {
			if (_getch() == Game::KeyBoardKeys::ESC) return;
		}
	}
}
