#include <curses.h>
#include "game.h"

// 점수 보드를 출력하는 함수
void renderScoreBoard(WINDOW *scoreWin, Game &game, long long duration, int score)
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

void renderGame(WINDOW *mapWin, Game &game, int mapSize)
{
    werase(mapWin);
    const auto &map = game.getEntireMap();
    for (int x = 0; x < mapSize; ++x)
    {
        for (int y = 0; y < mapSize; ++y)
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
}
