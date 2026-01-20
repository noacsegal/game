#include "Game.h"

int main(int argc, char* argv[]) {

    Game game;

    //default to reguler game like part 2
    Game::gameType type = Game::gameType::KEYBOARD;
    bool silent = false;

    // Check arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-load") {
            type = Game::gameType::FILE; // Replay Mode
        }
        else if (arg == "-save") {
            type = Game::gameType::RECORDING_KEYBOARD; // Record Mode
        }
        else if (arg == "-silent") {
            silent = true; // Replay in silent mode
        }
    }
    if (type == Game::gameType::FILE && silent) {
        game.setSilent(true);
    }
    else {
        game.setSilent(false);
    }

	game.setGameMode(type);
	game.startGame();

    if (type == Game::gameType::FILE && silent) {
        game.verifySilentResults();
    }

    return 0;
}