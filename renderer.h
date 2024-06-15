#include <curses.h>
#include "game.h"

void renderScoreBoard(WINDOW *scoreWin, Game &game, long long duration, int score);
void renderGame(WINDOW *mapWin, Game &game, int mapSize);