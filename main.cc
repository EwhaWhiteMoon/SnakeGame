#include <cstdlib>
#include <ncurses.h>
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

int main()
{
    // ncurses 초기화
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);

    // 키패드 활성화
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); //getch가 non-blocking으로 실행되도록 함


    // 지도와 점수 보드를 위한 창 생성
    int mapHeight = 21;
    int mapWidth = 21;
    int scoreBoardWidth = 20;
    WINDOW *mapWin = newwin(mapHeight, mapWidth, 0, 0);
    WINDOW *scoreWin = newwin(mapHeight, scoreBoardWidth, 0, mapWidth + 1);

    // 게임 초기화
    Stage stage("blankmap.txt"); // 스테이지 파일 경로
    Game game(stage);

    // 메인 게임 루프
    bool running = true;
    auto lastTick = std::chrono::steady_clock::now();

    // rand 초기화
    srand((unsigned int)lastTick.time_since_epoch().count());

    while (running)
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
            running = false;
            break;
        }

        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count();
        auto status = game.tick({dy, dx}, duration);

        // 게임 상태 확인
        if (status == gameStatus::Lose)
        {
            mvprintw(mapHeight + 1, 0, "Game Over! Press any key to quit");
            refresh();
            running = false;
        }
        else if (status == gameStatus::Win)
        {
            mvprintw(mapHeight + 1, 0, "Stage Complete! Press any key to quit.");
            refresh();
            running = false;
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
                    mvwprintw(mapWin, y, x, "#");
                    break;
                case mapTile::IWall:
                    mvwprintw(mapWin, y, x, "I");
                    break;
                case mapTile::Snake:
                    mvwprintw(mapWin, y, x, "O");
                    break;
                case mapTile::Growth:
                    mvwprintw(mapWin, y, x, "+");
                    break;
                case mapTile::Poison:
                    mvwprintw(mapWin, y, x, "-");
                    break;
                case mapTile::Portal:
                    mvwprintw(mapWin, y, x, "A");
                    break;
                case mapTile::inVaild:
                    mvwprintw(mapWin, y, x, "X");
                }
            }
        }

        for(auto i: game.getSnake()){
            mvwprintw(mapWin, i.x, i.y, "@");
        }
        wrefresh(mapWin);

        // 점수 보드 렌더링
        displayScoreBoard(scoreWin, game);
    }

    // 정리
    while(getch() == -1);
    delwin(mapWin);
    delwin(scoreWin);
    endwin();
    return 0;
}
