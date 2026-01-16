#pragma once
#include <conio.h>
#include "input.h"

class keyBoardInput : public input
{
public:
    virtual char getInput() override {
        if (_kbhit()) {
            return _getch();
        }
        return 0; 
    }
};

