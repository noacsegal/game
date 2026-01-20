#pragma once
#include <conio.h>
#include "input.h"

class player;

//reguler game (like part 2)
class keyBoardInput : public Input
{
public:

    virtual char getInput(long cycle, player* players) override {
        if (_kbhit()) {
            return _getch();
        }
        return 0;
    }

    virtual std::string getString(long cycle) override {
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

};