//Game.cpp

#include "Game.h"
#include "player.h"
#include <iostream>
#include <windows.h>
#include <conio.h>


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

Game::levelStatus Game::playLevel(Screen* currScreenPtr, player* players, GameScreens& gs, int indexScreen)
{
    bool running = true;

    std::vector<Door>* currentDoors = &currScreenPtr->screenDoorByRef();
    std::vector<Switch>* currentSwitches = &currScreenPtr->screenSwitchesByRef();
    std::vector<key>* currentKeys = &currScreenPtr->screenKeysByRef();
    std::vector<Bomb>* currentBombs = &currScreenPtr->screenBombsByRef();

    // 2. Track specific level logic
    bool isScreen1 = (indexScreen == 0);
    // Defines for darkness
    int darkMinX = 0, darkMaxX = 21;
    int darkMinY = 0, darkMaxY = 9;

    // Track completion
    bool playerFinished[GameScreens::NUM_OF_PLAYERS] = { false, false };

    if (isScreen1) {
        for (int y = darkMinY; y <= darkMaxY; y++) {
            for (int x = darkMinX; x <= darkMaxX; x++) {
                gotoxy(x, y);
                std::cout << ' ';
            }
        }
    }



    for (const auto& d : *currentDoors)
        d.getPlace().draw();


    while (running) {

        for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {

            player& s = players[i];

            // skip logic if this specific player already finished
            if (playerFinished[i]) continue;

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
                        playerFinished[i] = true;
                    }
                }
            }

            if (!hitDoor) {
                s.getBody().draw();

                if (s.move(*currScreenPtr, gs.riddleByRef())) {

                    s.draw();

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

                    //check collision with torch
                    bool playerHasTorch = false;
                    point torchPos;

                    for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i ++) {
                        player& p = players[i];

                        if (p.getItemType() == ItemType::TORCH) {
                            playerHasTorch = true;
                            torchPos = p.getBody();
                            break;
                        }
                    }

                    auto isVisible = [&](int x, int y) {
                        if (!isScreen1) return true;
                        if (x < darkMinX || x > darkMaxX || y < darkMinY || y > darkMaxY) {
                            return true;
                        }
                        // If inside the torch radiu
                        int dx = x - torchPos.getX();
                        int dy = y - torchPos.getY();
                        return (dx * dx + dy * dy) <= 25; // radius 5
                        };

                    if (isScreen1) {
                        for (int y = darkMinY; y <= darkMaxY; y++) {
                            for (int x = darkMinX; x <= darkMaxX; x++) {
                                gotoxy(x, y);
                                if (isVisible(x, y)) {
                                    std::cout << currScreenPtr->getChar(y, x); // Reveal map
                                }
                                else {
                                    std::cout << ' '; // Hide with space
                                }
                            }
                        }
                    }

                    //if the torch is in the dark place the torch will still be visible
                    if (!playerHasTorch) {
                        for (int y = darkMinY; y <= darkMaxY; y++) {
                            for (int x = darkMinX; x <= darkMaxX; x++) {
                                if (currScreenPtr->getChar(y, x) == player::TORCH) {
                                    torchPos = point(x, y, Direction::directions[Direction::STAY], player::TORCH);
                                    torchPos.draw();
                                }
                            }
                        }
                    }



                    // --- Redraw Logic ---

                    // 1. Draw Keys (if not taken)
                    for (auto& k : *currentKeys) {
                        if (isVisible(k.getPlaceP().getX(), k.getPlaceP().getY())) {
                            k.draw();
                        }
                    }

                    // 2. Draw Switches (To show state changes / or \)
                    for (auto& sw : *currentSwitches) {
                        sw.draw();
                    }

                    // 3. Draw Doors
                    for (const auto& d : *currentDoors) {
                        point doorPos = d.getPlace();
                        // Only draw the door if it is within the torch light radius
                        if (isVisible(doorPos.getX(), doorPos.getY())) {
                            doorPos.draw();
                        }
                    }

                    for (auto& b : *currentBombs) {
                        if (!b.getTaken()) {
                            if (isVisible(b.getPlaceP().getX(), b.getPlaceP().getY())) {
                                b.getPlaceP().draw();
                            }
                        }
                    }

                    // 4. Draw Players
                    for (int j = 0; j < GameScreens::NUM_OF_PLAYERS; j++) {
                        if (!playerFinished[j]) {
                            if (isVisible(players[j].getBody().getX(), players[j].getBody().getY())) {
                                players[j].draw();
                            }
                        }
                    }

                }
                else {
                    return levelStatus::RESTARTFAIL;
                }

            }

            if (s.getNumLives() == 0) {
                return levelStatus::RESTARTFAIL;

            }
        } // End of Player Loop

        //check iteration for bomb
        for (auto& b : *currentBombs) {
            if (b.isTicking()) {
                b.countdown(players[0], players[1], *currScreenPtr);
            }
        }


        // --- Input Handling (Pause/Exit) ---
        if (_kbhit()) {
            char keyBoard = _getch();
            //if we pressed ESC- pause and check if we are restarting 
            if (keyBoard == KeyBoardKeys::ESC) {
                gotoxy(0, Screen::MAX_Y);
                std::cout << "Paused. 'h' to home, ESC to continue.                                                     " << std::flush;
                gs.clearPlayerInventory(currScreenPtr->legendPosByRef());
                keyBoard = _getch();
                if (tolower(keyBoard) == HOME) {
                    return levelStatus::RESTARTKEY;
                }
                else if (tolower(keyBoard) == ESC) {
                    gotoxy(0, Screen::MAX_Y);
                    std::cout << "                                                                                ";
                    gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);
                }
            }

            else {
                //if we pressed a direction key

                for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {
                    if (!playerFinished[i]) players[i].keyPressed(std::tolower(keyBoard), *currScreenPtr);
                }

                //if player let go of an element
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
        if (playerFinished[0] && playerFinished[1]) {
            return levelStatus::NEXT_LEVEL;
        }


        Sleep(50);

    }
    return levelStatus::RESTARTFAIL;
}


void Game::startGame() {

    //in case of restart
    hideCursor();
    bool restart = false;

    do {
        GameScreens gs;


        if (!gs.LoadGameScreens()) break;

        restart = false; // Reset flag for this run

        Screen* currScreenPtr = &gs.startScreenByRef();
        currScreenPtr->drawOriginal();
       

        //waits to start the game
        startingScreen();
        int indexScreen = 0;

        levelStatus result;

        //loop all screens
        while (indexScreen < gs.numOfScreens()) {
            
            //load screen
            currScreenPtr = &gs.ScreeniByRef(indexScreen);
            
            //create two players
            player players[] = {
                player(currScreenPtr->player1posRef(), "wdxas", 'e'),
                player(currScreenPtr->player2posRef(), "ilmjk", 'o')
            };

            players[0].changeBodyChar('$');
            players[1].changeBodyChar('&');
            
            cls();
            gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);
            
            currScreenPtr->drawOriginal();
            //draw players
            for (auto& s : players) {
                s.draw();
            }

            result = playLevel(currScreenPtr, players, gs, indexScreen);

            if (result == levelStatus::RESTARTFAIL || result == levelStatus::RESTARTKEY) {
                restart= true; // Break inner loop, trigger outer do-while
                break;
            }
            else if (result == levelStatus::NEXT_LEVEL) {
                indexScreen++; // Advance to next level
            }
        }
        
        if (!restart && indexScreen == gs.numOfScreens()) {
            currScreenPtr = &gs.endScreenByRef();
            cls();
            currScreenPtr->drawOriginal();

            // Simple wait for exit
            while (true) {
                if (_kbhit() && _getch() == KeyBoardKeys::ESC) break;
            }
        }

        if (result == levelStatus::RESTARTFAIL) {

            cls();
            gotoxy(0, 0);
            std::cout << "You have lost the game :(" << std::endl;
            Sleep(1000);
            std::cout << "better luck next time" << std::endl;
            Sleep(1000);
            std::cout << "the game will restart now" << std::endl;
            Sleep(1000);
        }
    } while (restart);

}