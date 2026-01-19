//Game.cpp

#include "Game.h"
#include "player.h"
#include <iostream>
#include <windows.h>
#include <conio.h>

//outer game function
void Game::startGame() {

    //in case of restart
    hideCursor();
    bool restart = false;

    do {
        GameScreens gs;


        if (!gs.LoadGameScreens()) break;

        restart = false; // Reset flag for this run

        Screen* currScreenPtr = &gs.startScreenByRef();


        //waits to start the game
        if (!silentMode) {
            currScreenPtr->drawOriginal();
            startingScreen();

        }
        int indexScreen = 0;

        levelStatus result = levelStatus::RESTARTFAIL;
        
        //create two players
        player players[] = {
            player(currScreenPtr->player1posRef(), "wdxas", 'e'),
            player(currScreenPtr->player2posRef(), "ilmjk", 'o')
        };

        //loop all screens
        while (indexScreen < gs.numOfScreens()) {

            //load screen
            currScreenPtr = &gs.ScreeniByRef(indexScreen);

            players[0] = player(currScreenPtr->player1posRef(), "wdxas", 'e');
            players[1] = player(currScreenPtr->player2posRef(), "ilmjk", 'o');

            players[0].changeBodyChar('$');
            players[1].changeBodyChar('&');

            cls();
            gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);

            currScreenPtr->drawOriginal();
            //draw players
            for (auto& s : players) {
                s.draw();
            }

            //play this specific level
            result = playLevel(currScreenPtr, players, gs, indexScreen);
            
            //check what happened at the end of the level
            if (result == levelStatus::RESTARTFAIL || result == levelStatus::RESTARTKEY) {
                restart = true; // Break inner loop, trigger outer do-while
                break;
            }

            //log when players moved screens
            else if (result == levelStatus::NEXT_LEVEL) {
                indexScreen++; // Advance to next level
                logEvent("Player moved to Screen " + std::to_string(indexScreen + 2)); // +2 because index 0 is Screen 1, moving to Screen 2

            }

            //exit output
            else if (result == levelStatus::EXIT) {
                logEvent("Game exited by user.");
                cls();
                gotoxy(0, 0);
                std::cout << "You have stopped the game :(" << std::endl;
                Sleep(1000);
                std::cout << "Goodbye" << std::endl;
                Sleep(1000);
                if (resultFile.is_open()) resultFile.close();
                return;
            }
        }

        //end screen
        if (!restart && indexScreen == gs.numOfScreens()) {
            if (!silentMode) {
                int score = (players[0].getNumLives() + players[1].getNumLives()) * 100;
                logEvent("Game Ended. Final Score: " + std::to_string(score));

                if (resultFile.is_open()) resultFile.close();
                
                currScreenPtr = &gs.endScreenByRef();
                cls();
                currScreenPtr->drawOriginal();

                gotoxy(48, 13);
                std::cout << "The Final Score: " << score;

                // Simple wait for exit
                while (true) {
                    if (_kbhit() && _getch() == KeyBoardKeys::ESC) break;
                }
            }
           
        }

        //restart if lives ended
        if (result == levelStatus::RESTARTFAIL && !silentMode) {

            cls();
            gotoxy(0, 0);
            std::cout << "You have lost the game :(" << std::endl;
            Sleep(1000);
            std::cout << "better luck next time" << std::endl;
            Sleep(1000);
            std::cout << "the game will restart now" << std::endl;
            Sleep(1000);
        }
        if (silentMode) restart = false;
    } while (restart);

}

//prints the first screen and waits to start
void Game::startingScreen() {
    bool flag = true;
    while (flag) {
        if (_kbhit()) {
            char keyBoard = _getch();
            if (keyBoard == KeyBoardKeys::SPACE) {
                cls();
                flag = false;
            }
        }
        Sleep(10);
    }

}

//play one level
Game::levelStatus Game::playLevel(Screen* currScreenPtr, player* players, GameScreens& gs, int indexScreen)
{
    bool running = true;
    bool playerFinished[GameScreens::NUM_OF_PLAYERS] = { false, false };

    // Initial Draw
    currScreenPtr->drawOriginal();
    for (const auto& d : currScreenPtr->screenDoorByRef()) d.getPlace().draw();

    while (running) {

        // input handle
        levelStatus inputStatus = handleInput(players, currScreenPtr, gs, currentCycle);
        if (inputStatus == levelStatus::RESTARTKEY || inputStatus == levelStatus::EXIT) return inputStatus;
        
        int livesBeforePlayer1 = players[0].getNumLives();
        int livesBeforePlayer2 = players[1].getNumLives();
        
        // moving
        for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {
            if (!playerFinished[i]) {
                handlePlayerMove(players[i], currScreenPtr, gs, playerFinished[i], players);
            }
        }

        //checks if any lives were lost when player moved
        int livesAfterPlayer1 = players[0].getNumLives();
        int livesAfterPlayer2 = players[1].getNumLives();

        if (livesAfterPlayer1 < livesBeforePlayer1) {
            logEvent("Player 1 lost a life");
        }

        if (livesAfterPlayer2 < livesBeforePlayer2) {
            logEvent("Player 2 lost a life");
        }
        // draw whats needed
        drawLevel(currScreenPtr, players, indexScreen, playerFinished);

        //next level check
        if (playerFinished[0] && playerFinished[1]) return levelStatus::NEXT_LEVEL;

        //if one of the players is out of lives leave
        if (players[0].getNumLives() == 0 || players[1].getNumLives() == 0) {
            return levelStatus::RESTARTFAIL;
        }

        // checks bombs
        for (auto& b : currScreenPtr->screenBombsByRef()) {
            if (b.isTicking()) b.countdown(players[0], players[1], *currScreenPtr);
        }

        Sleep(sleepTime);
        currentCycle++;
    }
    return levelStatus::RESTARTFAIL;
}



//handles input for the level
Game::levelStatus Game::handleInput(player* players, Screen* currScreenPtr, GameScreens& gs, long cycle)
{
    char keyBoard = input->getInput(cycle, players);

    //only checks input when in normal game
    if (!silentMode) {
        if (keyBoard != 0) {

            if (keyBoard == KeyBoardKeys::ESC) {

                gotoxy(0, Screen::MAX_Y);
                std::cout << "Paused. press h to restart or ESC to exit. press anything else to continue                 ";
                keyBoard = _getch();
                if (tolower(keyBoard) == HOME) {
                    gotoxy(0, Screen::MAX_Y);
                    std::cout << "                                                                          ";
                    gs.clearPlayerInventory(currScreenPtr->legendPosByRef());
                    return levelStatus::RESTARTKEY;
                }
                if (tolower(keyBoard) == ESC) {
                    return levelStatus::EXIT;
                }
                // Clear pause message
                gotoxy(0, Screen::MAX_Y);
                std::cout << "                                                                               ";
            }

            else {
                // Movement Keys
                for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {
                    players[i].keyPressed(std::tolower(keyBoard), *currScreenPtr);
                }

                for (int j = 0; j < GameScreens::NUM_OF_PLAYERS; j++) {

                    player& p = players[j];

                    //if we pressed the char to get rid of something and we're holding something
                    if (keyBoard == p.getDisposeChar() && p.getItemType() != ItemType::EMPTY) {

                        if (p.getItemType() == ItemType::KEY) {
                            p.changeKey()->getPlaceP().changePosition(p.getBody());//change position of key
                            p.changeKey()->changeTaken(false); //key knows it isnt being held
                            p.updateKey(nullptr); //player doesnt have key
                            p.updateItemType(ItemType::EMPTY); // the player knows it isn't holding an item
                        }

                        else if (p.getItemType() == ItemType::BOMB) {
                            p.changeBomb()->turnOn();
                            p.changeBomb()->getPlaceP().changePosition(p.getBody());//change position of bomb
                            p.updateBomb(nullptr);
                            p.updateItemType(ItemType::EMPTY);
                        }

                        else if (p.getItemType() == ItemType::TORCH) {
                            currScreenPtr->setCharCurrent(p.getBody(), player::TORCH);
                            p.updateItemType(ItemType::EMPTY);
                            p.getBody().draw(player::TORCH);
                        }
                    }
                }
            }
        }
    }
    
    return levelStatus::NEXT_LEVEL; // Means "Continue Playing", not actually next level yet
}

//checks all the things for one players moving
void Game::handlePlayerMove(player& s, Screen* currScreenPtr, GameScreens& gs, bool& finished, player* players)
{
    std::vector<Door>* currentDoors = &currScreenPtr->screenDoorByRef();
    std::vector<Switch>* currentSwitches = &currScreenPtr->screenSwitchesByRef();
    std::vector<key>* currentKeys = &currScreenPtr->screenKeysByRef();
    std::vector<Bomb>* currentBombs = &currScreenPtr->screenBombsByRef();

    point temp = s.getBody();
    temp.move();

    //check if we met a door
    bool hitDoor = false;
    for (auto& door : *currentDoors) {

        if (temp == door.getPlace()) {
            hitDoor = true;

            //checks if the player is holding a key that the door needs
            if (s.getItemType() == ItemType::KEY) {
                if (door.keyForDoor(s.changeKey())) {
                    //take the key from the player
                    s.updateKey(nullptr);
                    s.updateItemType(ItemType::EMPTY);
                    gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);

                }
            }

            // The door itself now checks its own keys and switches via pointers
            if (door.canOpen()) {
                door.addPlayer(); // Add player to door count

                // Visuals: Remove player
                s.draw(' ');
                finished = true;
                return;
            }
        }
    }

    //check collisions

    if (!hitDoor) {
        s.draw();

        if (s.move(*currScreenPtr, gs.riddleByRef())) {

            gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);


            // --- Check Collision with KEYS ---
            for (auto& k : *currentKeys) {
                if (s.getBody() == k.getPlaceP() && !k.isTaken()) {
                    if (s.getItemType() == ItemType::EMPTY) {
                        //only if player isn't holding anything and key isn't taken the player takes the key
                        s.updateKey(&k);
                        k.changeTaken(true);
                        s.updateItemType(ItemType::KEY);
                        currScreenPtr->setCharCurrent(k.getPlaceP(), ' ');
                    }
                    gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);
                }
            }

            // --- Check Collision with SWITCHES ---
            for (auto& sw : *currentSwitches) {
                if (s.getBody() == sw.getPlace()) {
                    sw.toggle(); // This updates the boolean inside the switch
                    currScreenPtr->setCharCurrent(sw.getPlace(), ' ');
                }
            }

            //check collision with bomb
            for (auto& b : *currentBombs) {

                if (s.getBody() == b.getPlaceP() && !b.isTicking() && !b.getTaken()) {
                    if (s.getItemType() == ItemType::EMPTY) {
                        s.updateBomb(&b);
                        s.updateItemType(ItemType::BOMB);
                        b.setTaken(true);
                        currScreenPtr->setCharCurrent(b.getPlaceP(), ' ');
                    }
                }
            }

            //torch check written by AI:
            // We check the map directly to see if we are standing on a torch char
            char itemOnFloor = currScreenPtr->getChar(s.getBody());

            // Assuming player::TORCH is the static char (e.g. '@' or similar)
            if (itemOnFloor == player::TORCH) {
                if (s.getItemType() == ItemType::EMPTY) {
                    s.updateItemType(ItemType::TORCH);       // 1. Pick it up
                    currScreenPtr->setCharCurrent(s.getBody(), ' '); // 2. Remove from map
                    // The Inventory UI will update in the main loop or you can call printPlayorInventory here
                }
            }
        }
    }
}

//handles all the redrawing needed
void Game::drawLevel(Screen* currScreenPtr, player* players, int indexScreen, bool* playerFinished)
{
    bool isScreen1 = (indexScreen == 0);
    int darkMinX = 0, darkMaxX = 21;
    int darkMinY = 0, darkMaxY = 9;

    // 1. Find Torch Position
    bool playerHasTorch = false;
    point torchPos;
    for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {
        if (players[i].getItemType() == ItemType::TORCH) {
            playerHasTorch = true;
            torchPos = players[i].getBody();
            break;
        }
    }

    // 2. Define Visibility Logic 
    auto isVisible = [&](int x, int y) {
        if (!isScreen1) return true;
        if (x < darkMinX || x > darkMaxX || y < darkMinY || y > darkMaxY) return true;
        int dx = x - torchPos.getX();
        int dy = y - torchPos.getY();
        return (dx * dx + dy * dy) <= 25; // Radius 5
        };

    // 3. Draw Darkness / Reveal Map
    if (isScreen1) {
        for (int y = darkMinY; y <= darkMaxY; y++) {
            for (int x = darkMinX; x <= darkMaxX; x++) {
                gotoxy(x, y);
                if (isVisible(x, y)) std::cout << currScreenPtr->getChar(y, x);
                else std::cout << ' ';
            }
        }
    }

    // 4. Draw Unheld Torches in the dark
    if (!playerHasTorch) {
        for (int y = darkMinY; y <= darkMaxY; y++) {
            for (int x = darkMinX; x <= darkMaxX; x++) {
                if (currScreenPtr->getChar(y, x) == player::TORCH) {
                    point(x, y, Direction::directions[Direction::STAY], player::TORCH).draw();
                }
            }
        }
    }

    // 5. Draw Game Objects (Only if Visible)
    auto& keys = currScreenPtr->screenKeysByRef();
    for (auto& k : keys) {
        if (isVisible(k.getPlaceP().getX(), k.getPlaceP().getY())) k.draw();
    }

    auto& switches = currScreenPtr->screenSwitchesByRef();
    for (auto& sw : switches) sw.draw();

    auto& doors = currScreenPtr->screenDoorByRef();
    for (const auto& d : doors) {
        if (isVisible(d.getPlace().getX(), d.getPlace().getY())) d.getPlace().draw();
    }

    auto& bombs = currScreenPtr->screenBombsByRef();
    for (auto& b : bombs) {
        if (!b.getTaken() && isVisible(b.getPlaceP().getX(), b.getPlaceP().getY())) {
            b.getPlaceP().draw();
        }
    }
    //isVisible(players[j].getBody().getX(), players[j].getBody().getY()) &&
    //  Draw Players (Only if Visible)
    for (int j = 0; j < GameScreens::NUM_OF_PLAYERS; j++) {
        if (!playerFinished[j]) {
            players[j].draw();
        }
    }
}

//checks what kind of game we are playing
void Game::setGameMode(bool isLoadMode)
{
    silentMode = isLoadMode;

    if (isLoadMode) {
        input = &filePlay;
        filePlay.init();
        sleepTime = 10; 
    }
    else {
        input = &recordingPlay;
        recordingPlay.init();
        sleepTime = 50;
        resultFile.open("adv-world.result");
    }
}


void Game::logRiddleEvent(std::string riddle, std::string answer, bool isCorrect)
{
    if (resultFile.is_open()) {
        std::string status = isCorrect ? "Correct" : "Incorrect";
        resultFile << currentCycle << ": Riddle: " << riddle
            << " | Answer: " << answer
            << " | Result: " << status << std::endl;
    }
}
