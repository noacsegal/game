#include "Game.h"

int main(int argc, char* argv[]) {

    Game game;

    //default to reguler game like part 2
    Game::gameType type = Game::gameType::KEYBOARD;

    // Check arguments
    if (argc > 1) {
        if (strcmp(argv[1], "-load") == 0) {
            type = Game::gameType::FILE; // Replay Mode
        }
        else if (strcmp(argv[1], "-save") == 0) {

            type = Game::gameType::RECORDING_KEYBOARD; // Record Mode
        }
    }

    game.setGameMode(type);
    game.startGame();

    return 0;
}