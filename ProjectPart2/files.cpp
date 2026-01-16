//files.cpp

#include "files.h"
#include "Screen.h"
#include "Game.h"
#include <conio.h>
#include "utillities.h"
#include <windows.h>
#include <cctype> 
#include "riddle.h"

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
bool files::createScreen(std::ifstream& screenFile, Screen& screenToFill) {
	int curr_row = 0;
	int curr_col = 0;
	char c;

	//we are not at the end of the screen or past the last row
	screenFile.get(c);
	while (c != END_SCREEN) {
		if (c == '\n') {

			//screen is too small
			if (curr_col < Screen::MAX_X - 1) {
				errorFunction("Screen is too small- not enough collums");
				return false;
			}
			point terminatorPos = { Screen::MAX_X - 1, curr_row };
			screenToFill.setCharOriginal(terminatorPos, '\0');
			screenToFill.setCharCurrent(terminatorPos, '\0');

			++curr_row;
			curr_col = 0;
			screenFile.get(c);
			continue;
		}
		point p = { curr_col, curr_row };

		if (c == 'L') {
			screenToFill.updateLegendPos(p);
		}

		if (curr_col < Screen::MAX_X) {
			// check where the players are supposed to be

			if (c == '$') {
				screenToFill.updatePlayer1Pos(p);
				screenToFill.setCharOriginal(p, c);
				screenToFill.setCharCurrent(p, ' ');
			}
			else if (c == '&') {
				screenToFill.updatePlayer2Pos(p);
				screenToFill.setCharOriginal(p, c);
				screenToFill.setCharCurrent(p, ' ');
			}
			else {
				screenToFill.setCharCurrent(p, c);
				screenToFill.setCharOriginal(p, c);
			}
			curr_col++;
		}
		screenFile.get(c);
	}

	if (curr_row < Screen::MAX_Y) {
		errorFunction("Screen is too small- not enough rows");
		return false;
	}
	return true;
}

bool files::createMetaData(std::ifstream& screenFile, Screen& screenToFill)
{
	std::string word;

	while (screenFile >> word && word != "DONE") {
		if (word == "DOOR") {
			char id;
			int kCount, sCount;

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
		if (word == "L") {
			point p = { 0, 26 };
			screenToFill.updateLegendPos(p);
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
	std::cout << "Game was stopped because: " << cause << std::endl;
	std::cout << "Press escape to exit the program" << std::endl;


}

bool files::createRiddles(riddle& ridToFill)
{
	namespace fs = std::filesystem;
	std::string riddleFileName = "";

	// 1. Loop through ALL files in the current folder
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();

		if (filenameStr.length() >= 6 &&
			filenameStr.substr(0, 6) == "riddle" &&
			filename.extension() == ".txt") {

			riddleFileName = filenameStr;
			break;
		}
	}

	// 3. If we found a file, open it and read
	if (!riddleFileName.empty()) {
		std::ifstream riddleFile(riddleFileName);

		if (riddleFile.is_open()) {
			std::string word;

			while (riddleFile >> word) {
				if (word == "RIDDLE") {
					int numRows;
					riddleFile >> numRows;
					std::string dummy;
					std::getline(riddleFile, dummy);

					std::string entireRid = "";

					for (int i = 0; i < numRows; i++) {
						std::string line;

						if (std::getline(riddleFile, line)) {

							entireRid += line + "\n";
						}
					}

					std::string answer;
					std::getline(riddleFile, answer);
					riddle::toLower(answer);
					ridToFill.addRiddle(entireRid, answer);

				}
			}
			riddleFile.close();
		}

		else {
			errorFunction("riddle file couldn't be opened");
			return false;
		}
	}

	else {
		errorFunction("No riddle file found!");
		return false;
	}
	return true;
}