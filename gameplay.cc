#include <cstdlib>
#include <ncurses.h>
#include <vector>
#include <chrono>
#include <fstream>
#include "game.h"
#include "stage.h"
#include "enums.h"
#include "main.cc"

int gamePlay(string StageName, int speed)
{
    // 게임 초기화
    Stage stage(StageName); // 스테이지 파일 경로
    Game game(stage, speed);

    // 지도와 점수 보드를 위한 창 생성
    int mapHeight = stage.getMapSize();
    int mapWidth = stage.getMapSize();
    int scoreBoardWidth = 20;
    WINDOW *mapWin = newwin(mapHeight, mapWidth, 0, 0);
    WINDOW *scoreWin = newwin(mapHeight, scoreBoardWidth, 0, mapWidth);

    // 시작 전 화면 정리
    erase();
    refresh();

    // 메인 게임 루프
    bool gameRunning = true;
    auto lastTick = std::chrono::steady_clock::now();

    // rand 초기화
    srand((unsigned int)lastTick.time_since_epoch().count());

    while (true)
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
            break;
        }

        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count();
        gameStatus status = game.tick({dy, dx}, duration);
        auto scores = game.getPoint();
        int score = scores[0] * 5 + scores[1] + scores[2] + scores[3] * 3 + (duration / 1000);

        // 게임 상태 확인
        if (status != gameStatus::Progress or !gameRunning)
        {
            werase(mapWin);
            werase(scoreWin);
            wrefresh(mapWin);
            wrefresh(scoreWin);
            delwin(mapWin);
            delwin(scoreWin);

            return status == gameStatus::Win ? score : -1;
        }

        // 지도 렌더링
        werase(mapWin);
        const auto &map = game.getEntireMap();
        for (int x = 0; x < mapHeight; ++x)
        {
            for (int y = 0; y < mapWidth; ++y)
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
            mvwprintw(mapWin, i.x, i.y, "O");
            wattroff(mapWin, COLOR_PAIR(1));
        }

        mvwprintw(mapWin, game.getSnake().back().x, game.getSnake().back().y, "@");
        wrefresh(mapWin);

        // 점수 보드 렌더링
        displayScoreBoard(scoreWin, game, duration, score);
    }
}