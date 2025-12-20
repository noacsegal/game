//GameScreens.cpp

#include "GameScreens.h"

void GameScreens::createScreenArray() {
    static const char* screen0[Screen::MAX_Y] = {
        // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
    "                                                                                ",
    "                                                                                ",
    "                           Welcome to the Game                                  ",
    "                                                                                ",
    "                                                                                ",
    "     Your objective is to escape this room through the door marked '1' or '2'.  ",
    "                                                                                ",
    "     The door is heavily locked. To open it, you must bring two keys (K).       ",
    "     Be aware: each player can only carry one key at a time.                    ",
    "     You must coordinate your movements to bring both keys to the exit.         ",
    "                                                                                ",
    "     Furthermore, the door mechanism is currently unpowered.                    ",
    "     Even with the keys, the door will not open until the switch ('\\')          ",
    "     has been located and activated.                                            ",
    "                                                                                ",
    "     Navigate carefully. You can push the crates (*) to clear a path,           ",
    "     but be careful not to trap yourself.                                       ",
    "     If you encounter a '?', you must answer correctly to pass.                 ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "                      Please press space to start playing                       ",
    "                                   Good luck.                                   "

    };



    static const char* screen1[Screen::MAX_Y] = {
        //1          2         3         4         5         6         7         8
        // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
          "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//0
          "W                                                                              W",//1
          "W                                                                              W",//2
          "W          K                                                            K      W",//3
          "W              W                                                               W",//4
          "W         W B*                                                                 W",//5
          "W           W    W                                                             W",//6
          "W      W         \\                                                             W",//7
          "W          W                                                                   W",//8
          "W                                                                              W",//9
          "W                                                                              W",//10
          "W                                                                              W",//11
          "W                                                                              W",//12
          "W                                                                              W",//13
          "W                                                                              W",//14
          "W                                                                              W",//15
          "W                                                                              W",//16
          "W                                                                              W",//17
          "W                                                                              W",//18
          "W                                                                              W",//19
          "W                                                                              W",//20
          "W                                                                              W",//21
          "W                                                                              W",//22
          "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//23
    };
    static const char* screen2[Screen::MAX_Y] = {
        //             1         2         3         4         5         6         7         8
        //   01234567890123456789012345678901234567890123456789012345678901234567890123456789
            "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//0
            "W                    W                       W                                 W",//1
            "W  WWWWWWWWWWWWWWWW  W  WWWWWWWWWWWWWWWW*WW  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWW  W",//2
            "W  W              W  W  W                 W  W  W                           W  W",//3
            "W  W              W  W  W                 W  W  W      * * W  W                W",//4
            "W  W      \\       *  W  W                 W  W  W    * * * W  W                W",//5
            "W  W              W  W  W                 W  W  W      * * W  W                W",//6
            "W  WWWWWWWWWWWWWWWW  W  W                 W  W  W         K                 W  W",//7
            "W                       W                 W  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWW  W",//8
            "WWWWWWWWWWWWWWWWWWWWWW  W                 W                                    W",//9
            "W                       W                 W   WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//10
            "W  WWWWWWWWWWWWWWWWWWWWWW                 W   W                                W",//11
            "W  W                    **WWWWWWWWWWWWWWWWW   W                                W",//12
            "W  W                                          W           WWWWWWWWWWWWWWWWWW   W",//13
            "W  W                    WWWWWWWWWWWWWWWWWWWWWWW           *              W     W",//14
            "W  WWWWWWWWWWWWWWWWWWW  W                                 W              W     W",//15
            "W                    W  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW              W     W",//16
            "WWWWWWWWWWWWWWWW*WW  W  W  W                              W       *      W     W",//17
            "W              *  *  W  W  W                              W      *?*     W     W",//18
            "W      K          W     W  W                              W      WWW     W     W",//19
            "W                 W     W  W                              WWWWWWWWWWWWWWWW     W",//20
            "W                 W        W                                                   W",//21
            "W                 WWWWWWW  W                                                   W",//22
            "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" //23

    };

    static const char* screenEnd[Screen::MAX_Y] = {
        // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
        "                                                                                ",//0
        "                                                                                ",//1
        "                                                                                ",//2
        "                                                                                ",//3
        "                                                                                ",//4
        "                                                                                ",//5
        "                                                                                ",//6
        "                                                                                ",//7
        "                                                                                ",//8
        "                                                                                ",//9
        "                                                                                ",//10
        "                                                                                ",//11
        "                              Congratulations!                                  ",//12
        "                     You have succesfully finished the game                     ",//13
        "                       press escape to end the simulation                       ",//14
        "                                                                                ",//15
        "                                                                                ",//16
        "                                                                                ",//17
        "                                                                                ",//18
        "                                                                                ",//19
        "                                                                                ",//20
        "                                                                                ",//21
        "                                                                                ",//22
        "                                                                                ",//23

    };

    gameScreens.resize(NUM_OF_SCREENS);
    gameScreens[0].createScreen(screen0);
    gameScreens[1].createScreen(screen1);
    gameScreens[2].createScreen(screen2);
    gameScreens[3].createScreen(screenEnd);

    screenKeys.resize(NUM_OF_SCREENS);
    screenSwitches.resize(NUM_OF_SCREENS);
    screenDoors.resize(NUM_OF_SCREENS);
}

//builds the keys,switches and doors for screen i+1
void GameScreens::buildLevel(int i)
{
	//builds the key,switch and door arrays for screen i
    Screen& currentScreen = gameScreens[i];
    screenKeys[i].clear();
    screenSwitches[i].clear();
    screenDoors[i].clear();
    screenBombs[i].clear();

    currentScreen.createKeyArray();
    currentScreen.createSwitchArray();
	
    vector<key>& detectedKeys = currentScreen.getScreenKeys();
    vector<Switch>& detectedSwitches = currentScreen.getScreenSwitches();

    //TEMP
    point placeB = { 22, 5,  Direction::directions[Direction::STAY], 'B' };
    Bomb b = { placeB };
    screenBombs[i].push_back(b);

    // ====================================================
    // DEFINE DOORS LOGIC HERE
    // ====================================================

    //door logic for first screen
    if (i == 1) {
        // Let's say Door 1 is at (10, 20) and needs the FIRST key found on screen
        if (detectedKeys.size() > 0) {
            vector<key*> keysForDoor1;
            keysForDoor1.push_back(&detectedKeys[0]); // Pointer to the first detected key

            vector<Switch*> switchesForDoor1; // No switches for this door

            vector<char> statesForDoor1; // No switch states

            // Add the door to GameScreens storage
            Door door1(point(10, 20, Direction::directions[Direction::STAY], '1'), keysForDoor1, switchesForDoor1, statesForDoor1);
            screenDoors[i].push_back(door1);
        }

        // --- Example: Door 2 Logic ---
        // Needs the SECOND key and the FIRST switch
        if (detectedKeys.size() > 0 && detectedSwitches.size() > 0) {
            vector<key*> keysForDoor2;
            keysForDoor2.push_back(&detectedKeys[1]);

            vector<Switch*> switchesForDoor2;
            switchesForDoor2.push_back(&detectedSwitches[0]);

            vector<char> statesForDoor2;
            statesForDoor2.push_back(Switch::OPEN);

            Door door2(point(50, 5, Direction::directions[Direction::STAY], '2'), keysForDoor2, switchesForDoor2, statesForDoor2);
            screenDoors[i].push_back(door2);
        }
    }

    //door logic for second screen
    if (i == 2) {

        if (detectedKeys.size() > 1 && detectedSwitches.size() > 0) {
            vector<key*> keysForDoor1;
            keysForDoor1.push_back(&detectedKeys[1]);
            keysForDoor1.push_back(&detectedKeys[1]);

            vector<Switch*> switchesForDoor1;
            switchesForDoor1.push_back(&detectedSwitches[0]);

            vector<char> statesForDoor1;
            statesForDoor1.push_back(Switch::OPEN);

            Door door2(point(31, 6, Direction::directions[Direction::STAY], '1'), keysForDoor1, switchesForDoor1, statesForDoor1);
            screenDoors[i].push_back(door2);
        }
    }

    //add doors
    for (auto& d : screenDoors[i]) {
        d.getPlace().draw();
    }

    
}
    
    

void GameScreens::printPlayorInventory(key* keyp1, key* keyp2)
{
    //prints at the bottom of the screen the inventory
    gotoxy(0, Screen::MAX_Y);
    int p1valid = 0;
    int p2valid = 0;

    if (!isNullKey(keyp1)) {
        p1valid++;
    }

    if (!isNullKey(keyp2)) {
        p2valid++;
    }
    std::cout << "Player $ has " << p1valid << " keys         Player & has " << p2valid << " keys" << std::flush;

}





//{
//    // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
//    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW1WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//0
//        "W                                      *                                       W",//1
//        "W WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW   WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW  W",//2
//        "W W *   W                            W  *W                            W    * W W",//3
//        "W W  *W W                            W * W                            W W *  W W",//4
//        "W W K W W                            W   W                            W W   KW W",//5
//        "W WWWWW W                            W  *W                            W WWWWWW W",//6
//        "W       W                            W * W                            W        W",//7
//        "WWWWWWWWW                            W*  W                            WWWWWWWWWW",//8
//        "W                                    W  *W                                     W",//9
//        "W             WWWWWWWWWWWWWWWWWWWWWWWW   WWWWWWWWWWWWWWWWWWWWWWWWW             W",//10
//        "W             W                                                  W             W",//11
//        "W             W                                                  W             W",//12
//        "W             W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW  W             W",//13
//        "W             W  W                                            W  W             W",//14
//        "W             W  W                                            W  W             W",//15
//        "W             W  W  WWWWWWWWWWWWWWWWWW    WWWWWWWWWWWWWWWWWW  W  W             W",//16
//        "W             W  W  W              W   * W              W  W  W  W             W",//17
//        "W             W  W  W              W \\      W              W  W  W             W",//18
//        "WWWWWWWWWWWWWWW  W  W              W * * W              W  W  W  WWWWWWWWWWWWWWW",//19
//        "W                W  W              W        W              W  W                W",//20
//        "W                W  W              W        W              W  W                W",//21
//        "W                                                                              W",//22
//        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//23
//};