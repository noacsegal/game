#include "files.h"
#include <fstream>
#include "Screen.h"


//function from lab with changes- return false if no files were found
bool files::getAllBoardFileNames()
{
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 9) == "adv-world" && filename.extension() == ".screen") {
			fileNames.push_back(filenameStr);
		}
	}
	//sort the files 
	std::sort(fileNames.begin(), fileNames.end());
	if (fileNames.empty()) {
		false;
	}
	return true;
}

//function from lab with improvments - return false if something was wrong with the file
bool files::createScreen(const std::string& filename, Screen& screenToFill){ 
	std::ifstream screenFile(filename);
	
	if (!screenFile.is_open()) {
		return false;
	}

	int curr_row = 0;
	int curr_col = 0;
	char c;
	
	//we are not at the end of the file or past the last row
	while (!screenFile.get(c).eof() && curr_row < Screen::MAX_Y) {
		if (c == '\n') {

			//screen is too small
			if (curr_col < Screen::MAX_X) {
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
	if (curr_row < Screen::MAX_Y) {
		return false;
	}
	
}
