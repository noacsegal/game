#include "Bomb.h"

void Bomb::goOff(player& p)
{
	int x = place.getX() - 3;
	int y = place.getY() - 3;
	int x_end = place.getY() + 3;
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
			if (x == p.getBody().getX() && y == p.getBody().getY()) {
				p.lowerLives();
			}
			gotoxy(x, y);
			cout << ' ' << endl;
			y++;
		}
		x++;
	}

}

//    3210123
// 3  WWWWWWW
// 2  WWWWWWW
// 1  WWWWWWW
// 0  WWW WWW
// 1  WWWWWWW
// 2  WWWWWWW
// 3  WWWWWWW



