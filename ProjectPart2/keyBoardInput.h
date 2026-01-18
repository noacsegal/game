#pragma once
#include <conio.h>
#include "input.h"

class keyBoardInput : public Input
{
public:

    virtual char getInput(long cycle) override {
        if (_kbhit()) {
            return _getch();
        }
        return 0; 
    }
};

