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
        Screen screent;
        std::ifstream screenFile(filename);

        gameScreens.push_back(screent);
        Screen& screen = gameScreens.back();
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
        point p = { -1,-1, Direction::directions[Direction::STAY], ' ' };
        if (screen.player1posRef() == p || screen.player2posRef() == p) {
            allFiles.errorFunction("No player start position");
            return false;
        }


        //finds: keys, switches, doors and bombs
        fillAddedData(screen);
        createStartAndEndScreen();


        if (!allFiles.createRiddles(gameRiddles)) {
            return false;
        }

        //connects doors to keys/switches
        if (!allFiles.createMetaData(screenFile, screen)) {
            return false;
        }

        if (screen.legendPosByRef() == p) {
            allFiles.errorFunction("No legend position");
            return false;
        }
        screenFile.close();
    }

    return true;
}

//goes over the screen we got and creates the needed vectors
void GameScreens::fillAddedData(Screen& screen) {
    screen.createBombArray();
    screen.createKeyArray();
    screen.createDoorArray();
    screen.createSwitchArray();


}

//function written by AI
void GameScreens::printPlayorInventory(point topLeft, player& p1, player& p2)
{
    // Helper lambda to get string representation of the item
    auto getItemName = [](ItemType t) -> std::string {
        switch (t) {
        case ItemType::KEY:  return "KEY ";
        case ItemType::BOMB: return "BOMB";
        case ItemType::TORCH: return "TORCH";
        default:             return "NONE";
        }
        };

    // Print Player 1 (e.g., $ Life:3 Hold:KEY )
    gotoxy(topLeft.getX(), topLeft.getY());
    std::cout << p1.getBody().getChar() << " Life:" << p1.getNumLives()
        << " Hold:" << getItemName(p1.getItemType()) << "      ";

    // Print Player 2 (e.g., & Life:3 Hold:NONE)
    gotoxy(topLeft.getX(), topLeft.getY() + 1);
    std::cout << p2.getBody().getChar() << " Life:" << p2.getNumLives()
        << " Hold:" << getItemName(p2.getItemType()) << "      ";

    // Print Player 1 (e.g., $ Life:3 Hold:KEY )
    gotoxy(topLeft.getX(), topLeft.getY());
    std::cout << p1.getBody().getChar() << " Life:" << p1.getNumLives()
        << " Hold:" << getItemName(p1.getItemType());

    // Print Player 2 (e.g., & Life:3 Hold:NONE)
    gotoxy(topLeft.getX(), topLeft.getY() + 1);
    std::cout << p2.getBody().getChar() << " Life:" << p2.getNumLives()
        << " Hold:" << getItemName(p2.getItemType());

    // Clear the third line for cleanliness
    gotoxy(topLeft.getX(), topLeft.getY() + 2);
    std::cout << "                    ";
}

void GameScreens::createStartAndEndScreen()
{
    const char* screenStart[Screen::MAX_Y] = {
        // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
           "                                                                               ",
           "                                                                               ",
           "                       WELCOME TO THE ESCAPE ROOM CHALLENGE                    ",
           "                                                                               ",
           "                        OBJECTIVE: Work together to escape!                    ",
           "                                                                               ",
           "          --------------------------- CONTROLS ---------------------------     ",
           "                                                                               ",
           "                  PLAYER 1 ($)                        PLAYER 2 (&)             ",
           "                  ------------                        ------------             ",
           "             MOVE UP:      W                     MOVE UP:      I               ",
           "             MOVE LEFT:    A                     MOVE LEFT:    J               ",
           "             MOVE DOWN:    X                     MOVE DOWN:    M               ",
           "             MOVE RIGHT:   D                     MOVE RIGHT:   L               ",
           "             STAY:         S                     STAY:         K               ",
           "             DROP ITEM:    E                     DROP ITEM:    O               ",
           "                                                                               ",
           "                                                                               ",
           "                                                                               ",
           "                                                                               ",
           "                                                                               ",
           "                       PRESS SPACE TO START THE GAME                           ",
           "                                                                               ",
           "                                                                               ",
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

//function written by AI
void GameScreens::clearPlayerInventory(point topLeft)
{
    // A string of spaces long enough to cover the longest possible output
    // (1 char + " Life:" + 2 digits + " Hold:" + "NONE" is about 20 chars)
    std::string eraser = "                          ";

    // Clear Line 1 (Player 1's previous stats)
    gotoxy(topLeft.getX(), topLeft.getY());
    std::cout << eraser;

    // Clear Line 2 (Player 2's previous stats)
    gotoxy(topLeft.getX(), topLeft.getY() + 1);
    std::cout << eraser;

    // Clear Line 3 (The extra clean-up line)
    gotoxy(topLeft.getX(), topLeft.getY() + 2);
    std::cout << eraser;
}


