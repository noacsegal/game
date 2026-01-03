//Game.cpp

#include "Game.h"
#include "player.h"
#include <iostream>

#include <windows.h>
#include <conio.h>
#include "utillities.h"


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


void Game::startGame() {

    //in case of restart
    hideCursor();
    bool restart = false;

    //create screen
    GameScreens gs;


    if (gs.LoadGameScreens()) {
        do {
            restart = false; // Reset flag for this run



            Screen* currScreenPtr = &gs.startScreenByRef();
            currScreenPtr->draw();
            bool running = true; //is false at the end of the game


            //waits to start the game
            startingScreen();
            int indexScreen = 0;

            //load screen
            currScreenPtr = &gs.ScreeniByRef(indexScreen);

            std::vector<Door>* currentDoors = &currScreenPtr->screenDoorByRef();
            std::vector<Switch>* currentSwitches = &currScreenPtr->screenSwitchesByRef();
            std::vector<key>* currentKeys = &currScreenPtr->screenKeysByRef();
            std::vector<Bomb>* currentBombs = &currScreenPtr->screenBombsByRef();

            //create two players
            player players[] = {
                player(currScreenPtr->player1posRef(), "wdxas", 'e'),
                player(currScreenPtr->player2posRef(), "ilmjk", 'o')
            };

            players[0].changeBodyChar('$');
            players[1].changeBodyChar('&');

            // Track which player has finished the current level
            bool playerFinished[GameScreens::NUM_OF_PLAYERS] = { false, false };

            gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);
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

                        // Check wall collisions / puzzles
                        if (s.move(*currScreenPtr)) {
                            gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);

                            // --- Check Collision with KEYS ---
                            for (auto& k : *currentKeys) {
                                if (s.getBody() == k.getPlaceP() && !k.isTaken()) {
                                    if (s.getItemType() == ItemType::EMPTY) {
                                        //only if player isn't holding anything and key isn't taken the player takes the key
                                        s.updateKey(&k);
                                        k.changeTaken(true);
                                        s.updateItemType(ItemType::KEY);
                                    }
                                    gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);
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
                                    if (s.getItemType() == ItemType::EMPTY && !b.isTicking()) {
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
                //check if players left screen

                if (playerFinished[0] && playerFinished[1]) {
                    indexScreen++;

                    // Check if End of Game
                    if (indexScreen == gs.numOfScreens() ) {
                        currScreenPtr = &gs.endScreenByRef(); // End screen
                        cls();
                        currScreenPtr->draw();
                        running = false;
                    }

                    else {
                        //next level setup
                        currScreenPtr = &gs.ScreeniByRef(indexScreen);
                        currentDoors = &currScreenPtr->screenDoorByRef();
                        currentSwitches = &currScreenPtr->screenSwitchesByRef();
                        currentKeys = &currScreenPtr->screenKeysByRef();
                    }


                    cls();
                    currScreenPtr->draw();

                    // Reset Players
                    players[0].moveScreen(currScreenPtr->player1posRef());
                    players[1].moveScreen(currScreenPtr->player2posRef());
                    players[0].updateKey(nullptr);
                    players[1].updateKey(nullptr);

                    playerFinished[0] = false;
                    playerFinished[1] = false;

                    players[0].draw();
                    players[1].draw();
                    gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);
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
                            gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);
                        }
                    }

                    else {
                        //if we pressed a direction key

                        for (int i = 0; i < GameScreens::NUM_OF_PLAYERS; i++) {
                            if (!playerFinished[i]) players[i].keyPressed(std::tolower(keyBoard));
                        }

                        //if player let go of an element
                        for (auto& p : players) {
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
                            }
                        }
                    }
                }
                Sleep(50);

            }

        } while (restart);

        

    }
    // End screen wait
    while (true) {
        if (_kbhit()) {
            if (_getch() == KeyBoardKeys::ESC) return;
        }
    }
}


