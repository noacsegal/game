//door.cpp

#include "Door.h"
//if the key is one of the door's keys

void Door::intializekeyUsed()
{
	for (int i = 0; i < my_keys.size(); i++) {
		keyUsed.push_back(false);
	}
}

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


bool Door::canOpen(key* k) {

	if (switchStatesCorrect() == false) {
		return false;
	}

	for (int i = 0; i < my_keys.size(); ++i) {

		if (keyUsed[i]) {
			continue; // This key has already been used
		}
		if (k->getPlaceP().getX() == my_keys[i]->getPlaceP().getX() &&
			k->getPlaceP().getY() == my_keys[i]->getPlaceP().getY()) {
			keyUsed[i] = true;
			keyCounter++;
			return true;
		}
	}
	return false;
}


