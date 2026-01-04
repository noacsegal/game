//door.cpp

#include "Door.h"
//if the key is one of the door's keys



//checks if all the switches are in the correct state
bool Door::switchStatesCorrect()
{
	for (int i = 0; i < my_switches.size(); ++i) {
		if (my_switches[i]->getPlace().getChar() != switch_states[i]) {
			return false;
		}
	}
	return true;
}

//changed****************************************************************************************************************
bool Door::keyForDoor(key* k) {

	if (k == nullptr) {
		return false;
	}
	gotoxy(0, 27);
	std::cout << "in keyForDoor going to return ";

	for (int i = 0; i < my_keys.size(); ++i) {

		if (keyUsed[i]) {
			continue; // This key has already been used
		}
		if (k == my_keys[i]) {
			keyUsed[i] = true;
			keyCounter++;
			std::cout << "true" << std::endl;
			return true;
		}
	}
	std::cout << "false" << std::endl;

	return false;
}

//added ********************************************************************************************************
bool Door::canOpen()
{
	if (keyCounter == my_keys.size() && switchStatesCorrect()) {
		return true;
	}
	bool keysReady = (keyCounter == my_keys.size());
	bool switchesReady = switchStatesCorrect();
	gotoxy(0, 28);
	std::cout << "Keys: " << keyCounter << "/" << my_keys.size()
		<< " | Switch OK? " << (switchesReady ? "YES" : "NO ") << std::endl;
	return false;
}