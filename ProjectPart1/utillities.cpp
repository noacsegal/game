#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "utillities.h"

//game consts
//charecters:

static constexpr char OBSTACLE = '*';
static constexpr char SPRING = '#';
static constexpr char SWITCH_ON = '/';
static constexpr char SWITCH_OFF = 47; // can't do '\'
static constexpr char RIDDLE = '?';
static constexpr size_t NUM_KEYS = 5;




//all of this cpp is from the lab

void gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Hide the cursor
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void cls() {
    system("cls");
}
