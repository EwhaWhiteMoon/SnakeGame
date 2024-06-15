#include <cstdlib>
#include <ncurses.h>
#include <vector>
#include <chrono>
#include <fstream>
#include "game.h"
#include "stage.h"
#include "enums.h"
#include "gameplay.cc"

menuStatus mainMenu()
{
    WINDOW *menuWin = newwin(5, 30, screen_height / 2 - 3, screen_width / 2 - 15);
    while (true)
    {
        displayStartMenu(menuWin);
        int ch = getch();
        switch (ch)
        {
        case 'S':
        case 's':
            delwin(menuWin);
            return menuStatus::Playing;
        case 'Q':
        case 'q':
            delwin(menuWin);
            return menuStatus::End;
        }
    }
}

menuStatus gameOver(int score)
{
    // 종료 메뉴 출력
    WINDOW *menuWin = newwin(10, 30, screen_height / 2 - 5, screen_width / 2 - 15);
    while (true)
    {
        displayEndMenu(menuWin, score);
        int ch = getch();
        switch (ch)
        {
        case 'M':
        case 'm':
            werase(menuWin);
            wrefresh(menuWin);
            delwin(menuWin);
            return menuStatus::Main;
        case 'Q':
        case 'q':
            werase(menuWin);
            wrefresh(menuWin);
            delwin(menuWin);
            return menuStatus::End;
        case 'N':
        case 'n':
            if (score <= 0)
                break;
            werase(menuWin);
            wrefresh(menuWin);
            delwin(menuWin);
            return menuStatus::Next;
        case 'R':
        case 'r':
            if (score > 0)
                break;
            werase(menuWin);
            wrefresh(menuWin);
            delwin(menuWin);
            return menuStatus::Playing;
        }
    }
}

menuStatus gameClear(vector<int> scores)
{

    int score = 0;
    for (int i : scores)
        score += i;

    int best;
    ifstream bs("bestscore");
    bs >> best;
    bs.close();

    if (best < score)
    {
        ofstream bso("bestscore");
        bso << score;
        bso.close();
    }

    WINDOW *menuWin = newwin(10, 22, screen_height / 2 - 5, screen_width / 2 - 10);
    box(menuWin, 0, 0);

    while (getch() == ERR)
    {
        mvwprintw(menuWin, 2, 1, "   YOU WIN!!!!!!   ");
        mvwprintw(menuWin, 3, 1, "   PRESS ANY KEY   ");

        mvwprintw(menuWin, 6, 1, "  SCORE  : %6d  ", score);
        mvwprintw(menuWin, 7, 1, "LAST BEST: %6d  ", best);
        if (best <= score)
            mvwprintw(menuWin, 8, 1, "   NEW BEST!!!!!   ");
        wrefresh(menuWin);
    };

    werase(menuWin);
    wrefresh(menuWin);
    delwin(menuWin);
    return menuStatus::End;
}