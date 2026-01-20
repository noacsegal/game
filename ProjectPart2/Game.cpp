//Game.cpp

#include "Game.h"
#include "player.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "utillities.h"

//outer game function
void Game::startGame() {

    std::streambuf* originalCoutBuffer = std::cout.rdbuf();
    std::ofstream devNull;

    if (silentMode) {
        // Redirect cout to a null file (cross-platform "trash can")
        devNull.open("nul"); // On Windows use "nul", on Linux/Mac use "/dev/null"
        std::cout.rdbuf(devNull.rdbuf());
    }

    //in case of restart
    hideCursor();
    bool restart = false;

    do {
        GameScreens gs;
        currentCycle = 0;

        if (!gs.LoadGameScreens()) break;

        restart = false; // Reset flag for this run

        Screen* currScreenPtr = &gs.startScreenByRef();

        levelStatus result = levelStatus::RESTARTFAIL;


        //waits to start the game
        if (!loadMode) {
            if (!startingScreen(currScreenPtr)) {
                return;
            }

        }

        int indexScreen = 0;


        //create two players
        player players[] = {
            player(currScreenPtr->player1posRef(), "wdxas", 'e'),
            player(currScreenPtr->player2posRef(), "ilmjk", 'o')
        };

        players[0].changeBodyChar('$');
        players[1].changeBodyChar('&');

        //loop all screens
        while (indexScreen < gs.numOfScreens()) {

            //load screen
            currScreenPtr = &gs.ScreeniByRef(indexScreen);

            players[0].changeHeld();
            players[1].changeHeld();

            players[0].moveScreen(currScreenPtr->player1posRef());
            players[1].moveScreen(currScreenPtr->player2posRef());

            if (!silentMode) {
                cls();
                gs.printPlayorInventory(currScreenPtr->legendPosByRef(), players[0], players[1]);

                currScreenPtr->drawOriginal();

                //draw players
                for (auto& s : players) {
                    s.draw();
                }
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
                logEvent("Players moved to Screen " + std::to_string(indexScreen + 1)); // +2 because index 0 is Screen 1, moving to Screen 2

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
            int score = (players[0].getNumLives() + players[1].getNumLives()) * 100;
            logEvent("Game Ended. Final Score: " + std::to_string(score));

            if (resultFile.is_open()) resultFile.close();

            currScreenPtr = &gs.endScreenByRef();
            cls();
            currScreenPtr->drawOriginal();

            gotoxy(25, 13);
            std::cout << "The Final Score: " << score;

            if (!loadMode) {
                
                // Simple wait for exit
                while (true) {
                    if (_kbhit() && _getch() == KeyBoardKeys::ESC) break;
                }
            }

        }

        //restart if lives ended
        if (result == levelStatus::RESTARTFAIL && !loadMode && !silentMode) {

            cls();
            gotoxy(0, 0);
            std::cout << "You have lost the game :(" << std::endl;
            Sleep(1000);
            std::cout << "better luck next time" << std::endl;
            Sleep(1000);
            std::cout << "the game will restart now" << std::endl;
            Sleep(1000);
        }
        if (loadMode) restart = false;
    } while (restart);

    std::cout.rdbuf(originalCoutBuffer);

    if (silentMode) {
        devNull.close();
    }

}

//prints the first screen and waits to start - true if we are playing
bool Game::startingScreen(Screen* currScreenPtr)
{
    cls();
    std::cout << "Press 1 to start a new game" << std::endl;
    std::cout << "Press 8 to present instructions and keys" << std::endl;
    std::cout << "Press 9 to exit" << std::endl;

    int answer; 
    std::cin >> answer;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    if (answer == 1) return true;

    if (answer == 8) {
        cls();
        currScreenPtr->drawOriginal();
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
        return true;
    }

    //if we havn't left yet then answer is 9
    return false;

}

//play one level
Game::levelStatus Game::playLevel(Screen* currScreenPtr, player* players, GameScreens& gs, int indexScreen)
{
    bool running = true;
    bool playerFinished[GameScreens::NUM_OF_PLAYERS] = { false, false };

    // Initial Draw
    if (!silentMode) {
        currScreenPtr->drawOriginal();
        for (const auto& d : currScreenPtr->screenDoorByRef()) d.getPlace().draw();
    }

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
       
        // draw whats needed if silent mode if false
        if (!silentMode) {
            drawLevel(currScreenPtr, players, indexScreen, playerFinished);
            Sleep(sleepTime);
        }

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

        //checks if any lives were lost when player moved
        int livesAfterPlayer1 = players[0].getNumLives();
        int livesAfterPlayer2 = players[1].getNumLives();

        if (livesAfterPlayer1 < livesBeforePlayer1) {
            logEvent("Player 1 lost a life");
        }

        if (livesAfterPlayer2 < livesBeforePlayer2) {
            logEvent("Player 2 lost a life");
        }
      
        currentCycle++;
    }
    return levelStatus::RESTARTFAIL;
}



//handles input for the level
Game::levelStatus Game::handleInput(player* players, Screen* currScreenPtr, GameScreens& gs, long cycle)
{
    char keyBoard = input->getInput(cycle, players);

    //only checks input when in normal game
    if (keyBoard != 0) {

        //can only pause and exit when keyboard game is played
        if (!loadMode) {
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
        }
        
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

        char itemAtDest = currScreenPtr->getChar(temp);
        
        if (itemAtDest == player::TORCH) {
            if (s.getItemType() == ItemType::EMPTY) {
                s.updateItemType(ItemType::TORCH);  
               currScreenPtr->setCharCurrent(temp, ' ');
            }
        }

        if (s.move(*currScreenPtr, gs.riddleByRef(), *this)) {

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
    bool isDarkRoom = currScreenPtr->getIsDark();
    int darkLimitY = currScreenPtr->getDarkRows();
    int darkLimitX = currScreenPtr->getDarkCols();

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
        if (!isDarkRoom) return true;
        if (x >= darkLimitX || y >= darkLimitY) return true;
        if (playerHasTorch) {
            int dx = x - torchPos.getX();
            int dy = y - torchPos.getY();
            return (dx * dx + dy * dy) <= 25; // Radius 5
        }
        //if the player is in the dark space wuthout a torch
        return false;
        };

    // 3. Draw Darkness / Reveal Map
    if (isDarkRoom) {
        for (int y = 0; y < darkLimitY; y++) {
            for (int x = 0; x < darkLimitX; x++) {
                point p(x, y, Direction::directions[Direction::STAY], ' ');
                if (isVisible(x, y)) {
                    p.draw(currScreenPtr->getChar(y, x));
                }
                else{
                    if (currScreenPtr->getChar(y, x) == player::TORCH) {
                        p.draw(player::TORCH);
                    }
                    else {
                        p.draw(' ');
                    }
                }
            }
        }
    }

    // 4. Draw Game Objects (Only if Visible)
    auto& keys = currScreenPtr->screenKeysByRef();
    for (auto& k : keys) {
        if (isVisible(k.getPlaceP().getX(), k.getPlaceP().getY()) && currScreenPtr->getChar(k.getPlaceP().getX(), k.getPlaceP().getY()) != ' ')
            k.draw();
    }

    auto& switches = currScreenPtr->screenSwitchesByRef();
    for (auto& sw : switches) {
        if (isVisible(sw.getPlace().getX(), sw.getPlace().getY())) sw.draw();
    }

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
    //  Draw Players (Only if Visible)
    for (int j = 0; j < GameScreens::NUM_OF_PLAYERS; j++) {
        if (!playerFinished[j]) {
            if (isVisible(players[j].getBody().getX(), players[j].getBody().getY())) {
                players[j].draw();
            }
        }
    }
}

//checks what kind of game we are playing
void Game::setGameMode(gameType type)
{
    this->activeType = type;

    //game that reads input from file
    if (type == gameType::FILE) {
        loadMode = true;
        input = &filePlay;
        filePlay.init();
        if (this->silentMode) {
            this->sleepTime = 0;
        }
        else {
            this->sleepTime = 10;
        }
    }
    //game that writes input to file
    else if (type == gameType::RECORDING_KEYBOARD) {
        input = &recordingPlay;
        recordingPlay.init();
        sleepTime = 50;
        resultFile.open("adv-world.result");
    }

    //reguler game like part 2
    else {
        sleepTime = 50;
        input = &keyboardPlay;
    }
}


void Game::logRiddleEvent(std::string riddle, std::string answer, bool isCorrect)
{
        std::string status = isCorrect ? "Correct" : "Incorrect";
        std::string entry = std::to_string(currentCycle) + ": Riddle: " + riddle
            + " | Answer: " + answer
            + " | Result: " + status;

        // 1. Log to file (for -save mode)
        if (resultFile.is_open()) {
            resultFile << entry << std::endl;
        }

        // 2. Log to vector (for -load -silent mode verification)
        if (activeType == gameType::FILE) {
            actualEvents.push_back(entry);
        }
}


// Helper to check if a line starts with a timestamp (e.g., "88: ")
bool isStartOfEvent(const std::string& line) {
    if (line.empty() || !isdigit(line[0])) return false;

    // Check if there is a ':' shortly after the number
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos) return false;

    // Ensure everything before the colon is a number
    for (size_t i = 0; i < colonPos; ++i) {
        if (!isdigit(line[i])) return false;
    }
    return true;
}

void Game::verifySilentResults() {
    std::ifstream expectedFile("adv-world.result");
    if (!expectedFile.is_open()) {
        std::cout << "Test Failed: Could not find adv-world.result" << std::endl;
        Sleep(1000);
        return;
    }

    // 1. PARSE THE FILE INTO A VECTOR
    // We group lines together so one riddle (even with 3 lines) becomes one Event string
    std::vector<std::string> expectedEventsVector;
    std::string line;
    std::string currentEventBuffer = "";

    while (std::getline(expectedFile, line)) {
        if (line.empty()) continue;

        // If this line starts with a Timestamp (e.g., "88: "), it's a new event
        if (isStartOfEvent(line)) {
            // Push the PREVIOUS event to the list (if exists)
            if (!currentEventBuffer.empty()) {
                expectedEventsVector.push_back(currentEventBuffer);
            }
            // Start the new event
            currentEventBuffer = line;
        }
        else {
            // This is a continuation of the previous event (like line 2 of a riddle)
            // Add it to the buffer with a newline
            currentEventBuffer += "\n" + line;
        }
    }
    // Push the final event
    if (!currentEventBuffer.empty()) {
        expectedEventsVector.push_back(currentEventBuffer);
    }
    expectedFile.close();


    // 2. COMPARE THE VECTORS
    bool mismatch = false;
    size_t countToCheck = expectedEventsVector.size();

    // Check if we have enough events
    if (actualEvents.size() < countToCheck) {
        std::cout << "Test Failed: Game ended earlier than expected." << std::endl;
        std::cout << "Missing event: " << expectedEventsVector[actualEvents.size()] << std::endl;
        Sleep(1000);
        return;
    }

    // Check contents
    for (size_t i = 0; i < countToCheck; ++i) {
        if (actualEvents[i] != expectedEventsVector[i]) {
            std::cout << "Test Failed: Mismatch at event " << (i + 1) << std::endl;
            std::cout << "  Expected:\n" << expectedEventsVector[i] << "\n";
            std::cout << "  Actual:\n" << actualEvents[i] << "\n";
            Sleep(1000);
            mismatch = true;
            break;
        }
    }

    // Check for extra events
    if (!mismatch && actualEvents.size() > countToCheck) {
        std::cout << "Test Failed: Game produced extra events not in the result file." << std::endl;
        Sleep(1000);
        mismatch = true;
    }

    if (!mismatch) {
        std::cout << "test passed" << std::endl;
        Sleep(1000);
    }
}

void Game::setSilent(bool mode) {
    this->silentMode = mode;
    if (mode) {
        this->sleepTime = 0; // Run as fast as possible
    }
}

void Game::logEvent(std::string message) {

    std::string entry = std::to_string(currentCycle) + ": " + message;

    // We are recording the game (-save)
    if (resultFile.is_open()) {
        resultFile << entry << std::endl;
    }

    if (activeType == gameType::FILE) {
        actualEvents.push_back(entry);
    }
}