#include <cstdlib>
#include <ncurses.h>
#include <vector>
#include <chrono>
#include <fstream>
#include "game.h"
#include "stage.h"
#include "enums.h"

// 시작 메뉴를 출력하는 함수
void displayStartMenu(WINDOW *menuWin)
{
    int startY = 1;
    int startX = 1;

    box(menuWin, 0, 0);
    mvwprintw(menuWin, startY, startX, "S : Game Start");
    mvwprintw(menuWin, startY + 1, startX, "Q : Quit Game");

    wrefresh(menuWin);
}

// 종료 메뉴를 출력하는 함수
void displayEndMenu(WINDOW *menuWin, int score)
{
    int startY = 1;
    int startX = 1;

    box(menuWin, 0, 0);
    if (score > 0)
    {
        mvwprintw(menuWin, ++startY, startX, "Stage Complete!");
        mvwprintw(menuWin, ++startY, startX, "Score : %04d", score);
    }
    else
        mvwprintw(menuWin, ++startY, startX, "Game Over!");

    startY += 1;
    mvwprintw(menuWin, ++startY, startX, "M : Go to Start Menu");
    if (score > 0)
        mvwprintw(menuWin, ++startY, startX, "N : Go to Next Stage");
    if (score <= 0)
        mvwprintw(menuWin, ++startY, startX, "R : Retry This Stage");
    mvwprintw(menuWin, ++startY, startX, "Q : Quit");

    wrefresh(menuWin);
}

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