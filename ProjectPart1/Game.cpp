//Game.cpp

#include "Game.h"
#include "player.h"
#include <iostream>

#include <windows.h>
#include <conio.h>
#include "utillities.h"\
#include "files.h"

enum KeyBoardKeys { ESC = 27, HOME = 104, SPACE = 32 };

void startingScreen() {
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


void Game::startGame() {

    //in case of restart
    hideCursor();
    bool restart = false;

    do {
        restart = false; // Reset flag for this run

        //create screen
        GameScreens gs;
        files allfiles;
        gs.createScreenArray();

        int indexScreen = 0;
        Screen* currScreenPtr = &gs.changeScreeni(indexScreen);
        currScreenPtr->draw();
        bool running = true; //is false at the end of the game


        //waits to start the game
        startingScreen();
        indexScreen++;

        gs.buildLevel(indexScreen);
        currScreenPtr = &gs.changeScreeni(indexScreen);

        std::vector<Door>* currentDoors = &gs.changeDoori(indexScreen);
        std::vector<Switch>* currentSwitches = &currScreenPtr->changeScreenSwitches();
        std::vector<key>* currentKeys = &currScreenPtr->changeScreenKeys();
        std::vector<Bomb>* currentBombs = &currScreenPtr->changeScreenBombs();

        //create two players
        player players[] = {
            player(point(10, 21, Direction::directions[Direction::STAY], '$'), "wdxas", 'e'),
            player(point(70, 21, Direction::directions[Direction::STAY], '&'), "ilmjk", 'o')
        };

        // Track which player has finished the current level
        bool playerFinished[GameScreens::NUM_OF_PLAYERS] = { false, false };

        gs.printPlayorInventory(players[0].changeKey(), (players[1].changeKey()));
        currScreenPtr->draw();

        //draw players
        for (auto& s : players) {
            s.draw();
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

                        //*************************************************************************************************
                        // The door itself now checks its own keys and switches via pointers
                        if (door.canOpen(s.changeKey())) {

                            //take the key from the player
                            s.updateKey(nullptr);
                            s.updateItemType(ItemType::EMPTY);
                            gs.printPlayorInventory(players[0].changeKey(), players[1].changeKey());

                            door.addPlayer(); // Add player to door count

                            // Visuals: Remove player
                            s.draw(' ');
                            playerFinished[i] = true;
                        }
                    }
                }

                if (!hitDoor) {
                    
                    // Check wall collisions / puzzles
                    if (s.move(*currScreenPtr)) {
                        gs.printPlayorInventory(players[0].changeKey(), players[1].changeKey());

                        // --- Check Collision with KEYS ---
                        for (auto& k : *currentKeys) {
                            if (s.getBody() == k.getPlaceP() && !k.isTaken()) {
                                if (s.getItemType() == ItemType::EMPTY) {
                                    //only if player isn't holding anything and key isn't taken the player takes the key
                                    s.updateKey(&k);
                                    k.changeTaken(true);
                                    s.updateItemType(ItemType::KEY);
                                }
                                gs.printPlayorInventory(players[0].changeKey(), players[1].changeKey());
                            }
                        }

                        // --- Check Collision with SWITCHES ---
                        for (auto& sw : *currentSwitches) {
                            if (s.getBody() == sw.getPlace()) {
                                sw.toggle(); // This updates the boolean inside the switch
                            }
                        }

                        //check collision with bomb
                        for (auto& b : *currentBombs) {
                            
                            if (s.getBody() == b.getPlaceP()) {
                                if (s.getItemType() == ItemType::EMPTY) {
                                    s.updateBomb(&b);
                                    s.updateItemType(ItemType::BOMB);
                                    b.setTaken(true);
                                }
                            }
                        }
                            
                            // --- Redraw Logic ---

                        // 1. Draw Keys (if not taken)
                        for (auto& k : *currentKeys) {
                            k.draw();
                        }

                        // 2. Draw Switches (To show state changes / or \)
                        for (auto& sw : *currentSwitches) {
                            sw.draw();
                        }

                        // 3. Draw Doors
                        for (const auto& d : *currentDoors) {
                            d.getPlace().draw();
                        }

                        for (auto& b : *currentBombs) {
                            if (!b.getTaken())
                                b.getPlaceP().draw();
                        }

                        // 4. Draw Players
                        for (int j = 0; j < GameScreens::NUM_OF_PLAYERS; j++) {
                            if (!playerFinished[j]) players[j].draw();
                        }
                    }
                    //for riddle
                    else {
                        restart = true;
                        running = false;
                        break; // break player loop
                    }
                }
            } // End of Player Loop

            if (restart) break;

            //check iteration for bomb
            for (auto& b : *currentBombs) {
                if (b.isTicking()) {
                    b.countdown(players[0], players[1], *currScreenPtr);
                }
            }

            bool levelComplete = false;
            //********************************************************************************************************************************
            // ADD: CHECK IF PLAYERS WENT THROUGH DIFFRENT DOORS AND WHAT TO DO
            //check if players left screen
            
            if (playerFinished[0] && playerFinished[1]) {
                indexScreen++;

                // Check if End of Game
                if (indexScreen == GameScreens::NUM_OF_SCREENS - 1) {
                    currScreenPtr = &gs.changeScreeni(indexScreen); // End screen
                    cls();
                    currScreenPtr->draw();
                    running = false;
                }

                else {
                    //next level setup
                    gs.buildLevel(indexScreen);
                    currScreenPtr = &gs.changeScreeni(indexScreen);
                    currentDoors = &gs.changeDoori(indexScreen);
                    currentSwitches = &currScreenPtr->changeScreenSwitches();
                    currentKeys = &currScreenPtr->changeScreenKeys();
                }


                cls();
                currScreenPtr->draw();

                // Reset Players
                players[0].bodyToChange().changePosition(30, 10, Direction::directions[Direction::STAY]);
                players[1].bodyToChange().changePosition(32, 10, Direction::directions[Direction::STAY]);
                players[0].updateKey(nullptr);
                players[1].updateKey(nullptr);

                playerFinished[0] = false;
                playerFinished[1] = false;

                players[0].draw();
                players[1].draw();
                gs.printPlayorInventory(players[0].changeKey(), players[1].changeKey());
            }

            // --- Input Handling (Pause/Exit) ---
            if (_kbhit()) {
                char keyBoard = _getch();
                //if we pressed ESC- pause and check if we are restarting 
                if (keyBoard == KeyBoardKeys::ESC) {
                    gotoxy(0, Screen::MAX_Y);
                    std::cout << "Paused. 'h' to home, ESC to continue.                                                     " << std::flush;
                    keyBoard = _getch();
                    if (tolower(keyBoard) == HOME) {
                        restart = true;
                        running = false;
                    }
                    else if (tolower(keyBoard) == ESC) {
                        gotoxy(0, Screen::MAX_Y);
                        std::cout << "                                                                                ";
                        gs.printPlayorInventory(players[0].changeKey(), players[1].changeKey());
                    }
                }
                
                else {
                    //if we pressed a direction key

                    for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {
                        if (!playerFinished[i]) players[i].keyPressed(std::tolower(keyBoard));
                    }

                    //if player let go of an element
                    for (auto& p : players) {
                        //if we pressed the char to get rid od something and we're holding something
                        if (keyBoard == p.getDisposeChar() && p.getItemType() != ItemType::EMPTY) {

                            if (p.getItemType() == ItemType::KEY) {
                                p.changeKey()->getPlaceP().changePosition(p.getBody().getX(), p.getBody().getY(), Direction::directions[Direction::STAY]);//change position of key
                                p.changeKey()->changeTaken(false); //key knows it isnt being held
                                p.updateKey(nullptr); //player doesnt have key
                                p.updateItemType(ItemType::EMPTY); // the player knows it isn't holding an item

                            }

                            else if (p.getItemType() == ItemType::BOMB) {
                                p.changeBomb()->turnOn();
                                p.changeBomb()->getPlaceP().changePosition(p.getBody().getX(), p.getBody().getY(), Direction::directions[Direction::STAY]);//change position of bomb
                                p.updateBomb(nullptr);
                                p.updateItemType(ItemType::EMPTY);
                            }
                        }
                    }
                }
            }
            Sleep(50);
            
        }

    } while (restart);


    // End screen wait
    while (true) {
        if (_kbhit()) {
            if (_getch() == KeyBoardKeys::ESC) return;
        }
    }
}

