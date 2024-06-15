#include <cstdlib>
#include <ncurses.h>
#include <vector>
#include <chrono>
#include <fstream>
#include "game.h"
#include "stage.h"
#include "enums.h"
#include "gameplay.cc"
#include "menu.cc"

#define screen_height 30
#define screen_width 90

// 점수 보드를 출력하는 함수
void displayScoreBoard(WINDOW *scoreWin, Game &game, long long duration, int score)
{
    int startY = 1;
    int startX = 1;

    werase(scoreWin); // Only erase the score window
    box(scoreWin, 0, 0);
    mvwprintw(scoreWin, ++startY, startX, "Score Board");

    auto &goals = game.getGoal();
    auto &points = game.getPoint();
    auto &done = game.getDone();

    mvwprintw(scoreWin, ++startY, startX, "Score: %5d", score);
    mvwprintw(scoreWin, ++startY, startX, "Time: %03.02f", ((double)duration / 1000));

    mvwprintw(scoreWin, ++startY, startX, "B: %d / %d", game.getSnake().size(), points[0]);
    mvwprintw(scoreWin, ++startY, startX, "+: %d", points[1]);
    mvwprintw(scoreWin, ++startY, startX, "-: %d", points[2]);
    mvwprintw(scoreWin, ++startY, startX, "G: %d", points[3]);

    startY += 1;

    mvwprintw(scoreWin, ++startY, startX, "Mission");
    mvwprintw(scoreWin, ++startY, startX, "B: %d (%c)", goals[0], done[0] ? 'v' : ' ');
    mvwprintw(scoreWin, ++startY, startX, "+: %d (%c)", goals[1], done[1] ? 'v' : ' ');
    mvwprintw(scoreWin, ++startY, startX, "-: %d (%c)", goals[2], done[2] ? 'v' : ' ');
    mvwprintw(scoreWin, ++startY, startX, "G: %d (%c)", goals[3], done[3] ? 'v' : ' ');

    wrefresh(scoreWin);
}

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

void init();

int main()
{
    init();

    menuStatus curMenu = menuStatus::Main;
    int curStage = 0;
    vector<string> stageList({"blankmap.txt", "largemap.txt", "largemap2.txt", "testmap.txt"});
    vector<int> scores(stageList.size(), 0);

    while (curMenu != menuStatus::End)
    {
        if (curMenu == menuStatus::Main)
            curMenu = mainMenu();
        if (curMenu == menuStatus::Playing)
        {
            int score = gamePlay(stageList[curStage], 350 - curStage * 30);
            scores[curStage] = score;
            if (curStage == stageList.size() - 1 and score > 0)
            {
                curMenu = gameClear(scores);
                continue;
            }
            curMenu = gameOver(score);
        }
        if (curMenu == menuStatus::Next)
        {
            curStage++;
            curMenu = menuStatus::Playing;
        }
    }
    endwin();
    return 0;
}

void init()
{
    // ncurses 초기화
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);

    // 색상 초기화
    if (has_colors())
    {
        start_color();
        init_color(COLOR_CYAN, 700, 933, 933);    // #afeee
        init_color(COLOR_GREEN, 678, 1000, 184);  // #adff2f
        init_color(COLOR_MAGENTA, 576, 439, 859); // #9370db
        init_color(COLOR_RED, 1000, 714, 757);    // #ffb6c1
        init_color(COLOR_BLUE, 753, 753, 753);    // #c0c0c0

        init_pair(1, COLOR_CYAN, COLOR_BLACK);    // @
        init_pair(2, COLOR_GREEN, COLOR_BLACK);   // A
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // +
        init_pair(4, COLOR_RED, COLOR_BLACK);     // -
        init_pair(5, COLOR_BLUE, COLOR_BLACK);    // #
    }

    // 키패드 활성화
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // getch가 non-blocking으로 실행되도록 함
}