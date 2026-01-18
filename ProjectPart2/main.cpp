#include "Game.h"

int main(int argc, char* argv[]) {

	Game game;

	bool loadMode = false;
	if (argc > 1 && strcmp(argv[1], "-load") == 0) {
		loadMode = true;
	}
	game.setGameMode(true);
	game.startGame();

	return 0;
}