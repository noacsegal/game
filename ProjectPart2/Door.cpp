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

bool Door::keyForDoor(key* k) {

	if (k == nullptr) {
		return false;
	}

	for (int i = 0; i < my_keys.size(); ++i) {

		if (keyUsed[i]) {
			continue; // This key has already been used
		}
		if (k == my_keys[i]) {
			keyUsed[i] = true;
			keyCounter++;
			return true;
		}
	}

	return false;
}

bool Door::canOpen()
{
	if (keyCounter == my_keys.size() && switchStatesCorrect()) {
		return true;
	}
	bool keysReady = (keyCounter == my_keys.size());
	bool switchesReady = switchStatesCorrect();
	return false;
}