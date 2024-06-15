#include <cstdlib>
#include <ncurses.h>
#include "game.h"
#include "stage.h"
#include "enums.h"

void displayStartMenu(WINDOW *menuWin);
void displayEndMenu(WINDOW *menuWin, int score);
menuStatus mainMenu(bool isFirstRun);
menuStatus gameOver(int score);
menuStatus gameClear(vector<int> scores);