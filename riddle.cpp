#include "riddle.h"
#include <cstdlib>
#include <cctype>
#include <algorithm>

// turn the input to lower case (string)
void toLower(std::string& s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); });
}

//print the riddle, recive the answer and return if true or false
bool riddle::ask_riddle() {
	std::string answer = "struct";
	std::string input;
	std::cout << "I am a keyword used to group together Data of different types, in good or bad weather.\nI am often given a name, like a blueprint you draft.\nI lay out the members, fore and aft.\nWhat am I ? " << std::endl;
	std::cin >> input;
	toLower(input);
	return (input == answer);
}