#pragma once
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <filesystem>

enum class ItemType { EMPTY, KEY, BOMB, TORCH };

void gotoxy(int x, int y);

void hideCursor();

void cls();