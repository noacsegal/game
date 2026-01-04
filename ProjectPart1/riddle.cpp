#include "riddle.h"
#include <cstdlib>
#include <cctype>
#include <windows.h>
#include <algorithm>

// turn the input to lower case (string)
void toLower(std::string& s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); });
}

//print the riddle, recive the answer and return if true or false
bool riddle::ask_riddle() {

	cls();
	std::cout << riddles[numRid] << std::endl;
	std::string input;
	std::cin >> input;
	toLower(input);

	return (input == answers[numRid]);

}

void riddle::addRiddle(std::string rid, std::string ans)
{
	riddles.push_back(rid);
	answers.push_back(ans);
	trys.push_back(3);
}

bool riddle::answerRiddle(player& p, Screen& currscreen)
{
	bool success = false;
	int riddle_trys_left = p.RiddleTrysByRef();
	while (riddle_trys_left > 0 && !success) {
		
		cls();
		std::cout << "you have " << riddle_trys_left << " trys" << std::endl;
		//ask the question and resive true if the answer is correct and false otherwise
		bool result = ask_riddle();
		cls();

		if (result) {
			//when the answer is correct print the level as it was
			std::cout << "You are correct!" << std::endl;
			Sleep(500);
			cls();
			success = true;
		}
		else {
			std::cout << "You are wrong :(" << std::flush;
			riddle_trys_left--;
			std::cout << "You have " << riddle_trys_left << " trys left" << std::endl;
			Sleep(1000);
		}
	}

	if (riddle_trys_left == 0) {
		//if we got 0 trys exit the level to the home screen
		std::cout << "you have failed, the game is reset";
		Sleep(1000);
		cls();
		return false;
	}

	else {
		// the answer was right
		currscreen.setCharCurrent(p.getBody(), ' ');
		cls();
		currscreen.draw();
		numRid++;
		return true;
	}
}
