//GameScreens.cpp

#include "GameScreens.h"
#include "files.h"
#include "player.h"

//load all data from files
bool GameScreens::LoadGameScreens() {
    files allFiles;
    if (!allFiles.getAllScreenFileNames()) {
        return false;
    }

    std::vector<std::string> fileNames = allFiles.fileNameRef();

    for (const auto& filename : fileNames) {
        Screen screen;
        std::ifstream screenFile(filename);

        //check if file was opened
        if (!screenFile.is_open()) {
            allFiles.errorFunction("File could not be opened");
            return false;
        }

        //from each file create a screen 
        if (!allFiles.createScreen(screenFile, screen)) {
            return false;
        }

        //checks if we are missing a player position
        point p = { 0,0, Direction::directions[Direction::STAY], ' ' };
        if (screen.player1posRef() == p || screen.player2posRef() == p) {
            allFiles.errorFunction("No player start position");
            return false;
        }
        gameScreens.push_back(screen);

        //finds: keys, switches, doors and bombs
        fillAddedData(screen);
        createStartAndEndScreen();
        
        //connects doors to keys/switches
        if (!allFiles.createMetaData(screenFile, screen)) {
            return false;
        }

        screenFile.close();
    }

    return true;
}

//goes over the screen we got and creates the needed vectors
void GameScreens::fillAddedData(Screen& screen){
    screen.createBombArray();
    screen.createKeyArray();
    screen.createDoorArray();
    screen.createSwitchArray();
}


    
    

void GameScreens::printPlayorInventory(point topLeft, player& p1, player& p2)
{
    gotoxy(topLeft.getX(), topLeft.getY());
    
    if (p1.changeKey() != nullptr) {
        std::cout << "player: " << p1.getBody().getChar() << " has a key";
    }
    else {
        std::cout << "player: " << p1.getBody().getChar() << " does not have a key";
    }

    std::cout << "                                  ";

    if (p2.changeKey() != nullptr) {
        std::cout << "player: " << p2.getBody().getChar() << " has a key";
    }
    else {
        std::cout << "player: " << p2.getBody().getChar() << " does not have a key";
    }
}

void GameScreens::createStartAndEndScreen()
{
    const char* screenStart[Screen::MAX_Y] = {
 // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
    "                                                                               ",
    "                                                                               ",
    "                           Welcome to the Game                                 ",
    "                                                                               ",
    "                                                                               ",
    "     Your objective is to escape this room through the door marked '1' or '2'. ",
    "                                                                               ",
    "     The door is heavily locked. To open it, you must bring two keys (K).      ",
    "     Be aware: each player can only carry one key at a time.                   ",
    "     You must coordinate your movements to bring both keys to the exit.        ",
    "                                                                               ",
    "     Furthermore, the door mechanism is currently unpowered.                   ",
    "     Even with the keys, the door will not open until the switch ('\\')         ",
    "     has been located and activated.                                           ",
    "                                                                               ",
    "     Navigate carefully. You can push the crates (*) to clear a path,          ",
    "     but be careful not to trap yourself.                                      ",
    "     If you encounter a '?', you must answer correctly to pass.                ",
    "                                                                               ",
    "                                                                               ",
    "                                                                               ",
    "                                                                               ",
    "                      Please press space to start playing                      ",
    "                                   Good luck.                                  ",
    "                                                                               "

    };

    static const char* screenEnd[Screen::MAX_Y] = {
      //01234567890123456789012345678901234567890123456789012345678901234567890123456789
        "                                                                               ",//0
        "                                                                               ",//1
        "                                                                               ",//2
        "                                                                               ",//3
        "                                                                               ",//4
        "                                                                               ",//5
        "                                                                               ",//6
        "                                                                               ",//7
        "                                                                               ",//8
        "                                                                               ",//9
        "                                                                               ",//10
        "                                                                               ",//11
        "                              Congratulations!                                 ",//12
        "                     You have succesfully finished the game                    ",//13
        "                       press escape to end the simulation                      ",//14
        "                                                                               ",//15
        "                                                                               ",//16
        "                                                                               ",//17
        "                                                                               ",//18
        "                                                                               ",//19
        "                                                                               ",//20
        "                                                                               ",//21
        "                                                                               ",//22
        "                                                                               ",//23
        "                                                                               "//24
    };


    for (int row = 0; row < Screen::MAX_Y; row++) {
        startScreen.createScreenLine(screenStart[row], row);
        endScreen.createScreenLine(screenEnd[row], row);
    }
}










    //static const char* screen1[Screen::MAX_Y] = {
    //    //1          2         3         4         5         6         7         8
    //    // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
    //      "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//0
    //      "W                                                                              W",//1
    //      "W                                                                              W",//2
    //      "W          K                                                            K      W",//3
    //      "W              W                                                               W",//4
    //      "W         W                                                                    W",//5
    //      "W           W    W                                                             W",//6
    //      "W      W         \\                                                             W",//7
    //      "W          W                                                                   W",//8
    //      "W                                                                              W",//9
    //      "W                                                                              W",//10
    //      "W                                                                              W",//11
    //      "W                                                                              W",//12
    //      "W               @                                                              W",//13
    //      "W                                                                              W",//14
    //      "W                                                                              W",//15
    //      "W                                                                              W",//16
    //      "W                                                                              W",//17
    //      "W                                                                              W",//18
    //      "W                                                                              W",//19
    //      "W                                                                              W",//20
    //      "W                                                                              W",//21
    //      "W                                                                              W",//22
    //      "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//23
    //};
    //static const char* screen2[Screen::MAX_Y] = {
    //    //             1         2         3         4         5         6         7         8


    //};

  