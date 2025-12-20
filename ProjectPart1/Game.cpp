//Game.cpp

#include "Game.h"
#include "player.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "utillities.h"


enum KeyBoardKeys { ESC = 27, HOME = 104, SPACE = 32 };




void Game::startGame() {

    //in case of restart
    hideCursor();
    bool restart = false;

    do {
        restart = false; // Reset flag for this run

        //create screen
        GameScreens gs;
        gs.createScreenArray();

        int indexScreen = 0;
        Screen* currScreenPtr = &gs.Screeni(indexScreen);
        currScreenPtr->draw();
        bool running = true; //is false at the end of the game


        //waits to start the game
        while (running) {
            if (_kbhit()) {
                char keyBoard = _getch();
                if (keyBoard == KeyBoardKeys::SPACE) {
                    indexScreen++;
                    cls();
                    running = false;
                }
            }
            Sleep(10);
        }

        running = true;
        gs.buildLevel(indexScreen);
        currScreenPtr = &gs.Screeni(indexScreen);

        vector<Door>* currentDoors = &gs.Doori(indexScreen);
        vector<Switch>* currentSwitches = &currScreenPtr->ScreenSwitches();
        vector<key>* currentKeys = &currScreenPtr->ScreenKeys();
        vector<Bomb> currentBombs = &currScreenPtr->screenBombs();

        //create two players
        player players[] = {
            player(point(10, 21, Direction::directions[Direction::STAY], '$'), "wdxas", 'e', gs.nullkey()),
            player(point(70, 21, Direction::directions[Direction::STAY], '&'), "ilmjk", 'o', gs.nullkey())
        };

        // Track which player has finished the current level
        bool playerFinished[GameScreens::NUM_OF_PLAYERS] = { false, false };

        gs.printPlayorInventory(players[0].getInventory(), (players[1].getInventory()));
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

                        // The door itself now checks its own keys and switches via pointers
                        if (door.canOpen(s.getInventory())) {

                            //take the key from the player
                            s.updateKey(gs.nullkey());
                            gs.printPlayorInventory(players[0].getInventory(), players[1].getInventory());

                            door.addPlayer(); // Add player to door count

                            // Visuals: Remove player
                            s.draw(' ');
                            playerFinished[i] = true;
                        }
                    }
                }

                if (!hitDoor) {
                    s.getBody().move();

                    // Check wall collisions / puzzles
                    if (s.move(*currScreenPtr)) {
                        gs.printPlayorInventory(players[0].getInventory(), players[1].getInventory());

                        // --- Check Collision with KEYS ---
                        for (auto& k : *currentKeys) {
                            if (s.getBody() == k.getPlace() && !k.isTaken()) {
                                if (gs.isNullKey(s.getInventory())) {
                                    s.updateKey(&k);
                                    k.changeTaken(true);
                                }
                                gs.printPlayorInventory(players[0].getInventory(), players[1].getInventory());
                            }
                        }

                        // --- Check Collision with SWITCHES ---
                        for (auto& sw : *currentSwitches) {
                            if (s.getBody() == sw.getPlace()) {
                                sw.toggle(); // This updates the boolean inside the switch
                            }
                        }

                        //check collision with bomb
                        if (s.getBody() == )
                            //*************************************************************************************************************
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

                        // 4. Draw Players
                        for (int j = 0; j < GameScreens::NUM_OF_PLAYERS; j++) {
                            if (!playerFinished[j]) players[j].draw();
                        }
                    }
                    else {
                        restart = true;
                        running = false;
                        break; // break player loop
                    }
                }
            } // End of Player Loop

            if (restart) break;

            bool levelComplete = false;
            //********************************************************************************************************************************
            // ADD: CHECK IF PLAYERS WENT THROUGH DIFFRENT DOORS AND WHAT TO DO
            //check if players left screen
            
            if (playerFinished[0] && playerFinished[1]) {
                indexScreen++;

                // Check if End of Game
                if (indexScreen == GameScreens::NUM_OF_SCREENS - 1) {
                    currScreenPtr = &gs.Screeni(indexScreen); // End screen
                    cls();
                    currScreenPtr->draw();
                    running = false;
                }

                else {
                    //next level setup
                    gs.buildLevel(indexScreen);
                    currScreenPtr = &gs.Screeni(indexScreen);
                    currentDoors = &gs.Doori(indexScreen);
                    currentSwitches = &currScreenPtr->ScreenSwitches();
                    currentKeys = &currScreenPtr->ScreenKeys();
                }


                cls();
                currScreenPtr->draw();

                // Reset Players
                players[0].bodyToChange().changePosition(30, 10, Direction::directions[Direction::STAY]);
                players[1].bodyToChange().changePosition(32, 10, Direction::directions[Direction::STAY]);
                players[0].updateKey(gs.nullkey());
                players[1].updateKey(gs.nullkey());

                playerFinished[0] = false;
                playerFinished[1] = false;

                players[0].draw();
                players[1].draw();
                gs.printPlayorInventory(players[0].getInventory(), players[1].getInventory());
            }

            // --- Input Handling (Pause/Exit) ---
            if (_kbhit()) {
                char keyBoard = _getch();
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
                        gs.printPlayorInventory(players[0].getInventory(), players[1].getInventory());
                    }
                }
                else {
                    for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {
                        if (!playerFinished[i]) players[i].keyPressed(std::tolower(keyBoard));
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

