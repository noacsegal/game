//bomb.cpp

#include "Bomb.h"
#include "player.h"

void Bomb::goOff(player& p1, player& p2, Screen& currScreen)
{
	int x = place.getX() - 3;
	int y = place.getY() - 3;
	int x_end = place.getX() + 3;
	int y_end = place.getY() + 3;

	//check left and top of screen
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	//check right and bottom screen
	if (x_end > Screen::MAX_X - 1)
		x_end = Screen::MAX_X - 1;
	if (y_end > Screen::MAX_Y )
		y_end = Screen::MAX_Y;

	int save_y = y;
	//erases everything around the bomb
	while (x < x_end) {
		y = save_y;
		while (y < y_end) {
			
			//check if bomb met a player
			if (x == p1.getBody().getX() && y == p1.getBody().getY()) {
				p1.lowerLives();
			}
			if (x == p2.getBody().getX() && y == p2.getBody().getY()) {
				p2.lowerLives();
			}

			gotoxy(x, y);
			std::cout << ' ' << std::flush; // change the screen we see
			point todelete(x, y, Direction::directions[Direction::STAY], ' ');
			//changed ***************************************************************************************************************
			currScreen.setCharCurrent(todelete, ' ');//change the actual screen
			y++;
		}
		x++;
	}
	ticking = false;
}

void Bomb::countdown(player& p1, player& p2, Screen& currScreen){
	timer--;
	
	//display the countdown properly
	int display = ((timer - 1) / 12) + 1;

	if (timer > 0) {
		place.draw(display + '0');
	}

	else
		goOff(p1, p2, currScreen);
}



