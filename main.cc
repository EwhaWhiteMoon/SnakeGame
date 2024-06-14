#include <cstdlib>
#include <ncurses.h>
#include <system_error>
#include <vector>
#include <chrono>
#include "game.h"
#include "stage.h"
#include "enums.h"

// 점수 보드를 출력하는 함수
void displayScoreBoard(WINDOW *scoreWin, Game &game)
{
    int startY = 1;
    int startX = 1;

    werase(scoreWin); // Only erase the score window
    box(scoreWin, 0, 0);
    mvwprintw(scoreWin, startY, startX, "Score Board");

    auto &goals = game.getGoal();
    auto &points = game.getPoint();
    auto &done = game.getDone();

    mvwprintw(scoreWin, startY + 2, startX, "B: %d / %d", points[0], goals[0]);
    mvwprintw(scoreWin, startY + 3, startX, "+: %d", points[1]);
    mvwprintw(scoreWin, startY + 4, startX, "-: %d", points[2]);
    mvwprintw(scoreWin, startY + 5, startX, "G: %d", points[3]);

    mvwprintw(scoreWin, startY + 7, startX, "Mission");
    mvwprintw(scoreWin, startY + 8, startX, "B: %d (%c)", goals[0], done[0] ? 'v' : ' ');
    mvwprintw(scoreWin, startY + 9, startX, "+: %d (%c)", goals[1], done[1] ? 'v' : ' ');
    mvwprintw(scoreWin, startY + 10, startX, "-: %d (%c)", goals[2], done[2] ? 'v' : ' ');
    mvwprintw(scoreWin, startY + 11, startX, "G: %d (%c)", goals[3], done[3] ? 'v' : ' ');

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
void displayEndMenu(WINDOW *menuWin, bool win)
{
    int startY = 1;
    int startX = 1;

    box(menuWin, 0, 0);
    if (win)
        mvwprintw(menuWin, startY, startX, "Stage Complete!");
    else
        mvwprintw(menuWin, startY, startX, "Game Over!");

    mvwprintw(menuWin, startY + 2, startX, "M : Go to Start Menu");
    mvwprintw(menuWin, startY + 3, startX, "Q : Quit");

    wrefresh(menuWin);
}

int main()
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

    // 메인 게임 루프
    bool running = true;
    while (running)
    {
        // 지도와 점수 보드를 위한 창 생성
        int mapHeight = 21;
        int mapWidth = 21;
        int scoreBoardWidth = 20;
        WINDOW *mapWin = newwin(mapHeight, mapWidth, 0, 0);
        WINDOW *scoreWin = newwin(mapHeight, scoreBoardWidth, 0, mapWidth);

        // 시작 전 화면 정리
        erase();
        refresh();

        // 시작 메뉴 출력
        bool startMenu = true;
        while (startMenu)
        {
            WINDOW *menuWin = newwin(5, 30, (mapHeight - 5) / 2, (mapWidth + scoreBoardWidth - 30) / 2);
            displayStartMenu(menuWin);
            int ch = getch();
            switch (ch)
            {
            case 'S':
            case 's':
                startMenu = false;
                break;
            case 'Q':
            case 'q':
                delwin(mapWin);
                delwin(scoreWin);
                delwin(menuWin);
                endwin();
                return 0;
            }
            delwin(menuWin);
        }

        // 게임 초기화
        Stage stage("blankmap.txt"); // 스테이지 파일 경로
        Game game(stage);

        // 메인 게임 루프
        bool gameRunning = true;
        auto lastTick = std::chrono::steady_clock::now();

        // rand 초기화
        srand((unsigned int)lastTick.time_since_epoch().count());

        while (gameRunning)
        {
            // 입력 처리
            int ch = getch();
            int dx = 0, dy = 0;
            switch (ch)
            {
            case KEY_UP:
                dy = -1;
                break;
            case KEY_DOWN:
                dy = 1;
                break;
            case KEY_LEFT:
                dx = -1;
                break;
            case KEY_RIGHT:
                dx = 1;
                break;
            case 'q':
                gameRunning = false;
                running = false;
                break;
            }

            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count();
            auto status = game.tick({dy, dx}, duration);

            // 게임 상태 확인
            if (status == gameStatus::Lose || status == gameStatus::Win)
            {
                bool win = (status == gameStatus::Win);

                // 종료 메뉴 출력
                WINDOW *menuWin = newwin(7, 30, (mapHeight - 7) / 2, (mapWidth + scoreBoardWidth - 30) / 2);
                displayEndMenu(menuWin, win);

                bool endMenu = true;
                while (endMenu)
                {
                    int ch = getch();
                    switch (ch)
                    {
                    case 'M':
                    case 'm':
                        endMenu = false;
                        gameRunning = false;
                        break;
                    case 'Q':
                    case 'q':
                        endMenu = false;
                        gameRunning = false;
                        running = false;
                        break;
                    }
                }
                delwin(menuWin);
            }

            // 지도 렌더링
            werase(mapWin);
            const auto &map = game.getEntireMap();
            for (int x = 0; x < map.size(); ++x)
            {
                for (int y = 0; y < map[x].size(); ++y)
                {
                    switch (map[y][x])
                    {
                    case mapTile::None:
                        mvwprintw(mapWin, y, x, " ");
                        break;
                    case mapTile::Wall:
                        wattron(mapWin, COLOR_PAIR(5));
                        mvwprintw(mapWin, y, x, "#");
                        wattroff(mapWin, COLOR_PAIR(5));
                        break;
                    case mapTile::IWall:
                        mvwprintw(mapWin, y, x, "I");
                        break;
                    case mapTile::Snake:
                        wattron(mapWin, COLOR_PAIR(1));
                        mvwprintw(mapWin, y, x, "O");
                        wattroff(mapWin, COLOR_PAIR(1));
                        break;
                    case mapTile::Growth:
                        wattron(mapWin, COLOR_PAIR(3));
                        mvwprintw(mapWin, y, x, "+");
                        wattroff(mapWin, COLOR_PAIR(3));
                        break;
                    case mapTile::Poison:
                        wattron(mapWin, COLOR_PAIR(4));
                        mvwprintw(mapWin, y, x, "-");
                        wattroff(mapWin, COLOR_PAIR(4));
                        break;
                    case mapTile::Portal:
                        wattron(mapWin, COLOR_PAIR(2));
                        mvwprintw(mapWin, y, x, "A");
                        wattroff(mapWin, COLOR_PAIR(2));
                        break;
                    case mapTile::inVaild:
                        mvwprintw(mapWin, y, x, "X");
                    }
                }
            }

            for (auto i : game.getSnake())
            {
                wattron(mapWin, COLOR_PAIR(1));
                mvwprintw(mapWin, i.x, i.y, "@");
                wattroff(mapWin, COLOR_PAIR(1));
            }
            wrefresh(mapWin);

            // 점수 보드 렌더링
            displayScoreBoard(scoreWin, game);
        }

        delwin(mapWin);
        delwin(scoreWin);
    }

    // 정리
    endwin();
    return 0;
}
