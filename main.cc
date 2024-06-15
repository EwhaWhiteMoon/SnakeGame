#include <ncurses.h>
#include <vector>
#include "game.h"
#include "enums.h"
#include "menu.h"
#include "renderer.h"

void init();
int gamePlay(string StageName, int speed);

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
        init_color(COLOR_BLUE, 700, 933, 933);    // #afeeee
        init_color(COLOR_GREEN, 678, 1000, 184);  // #adff2f
        init_color(COLOR_MAGENTA, 576, 439, 859); // #9370db
        init_color(COLOR_RED, 1000, 714, 757);    // #ffb6c1
        init_color(COLOR_WHITE, 753, 753, 753);   // #c0c0c0

        init_pair(1, COLOR_BLUE, COLOR_BLACK);    // O
        init_pair(2, COLOR_GREEN, COLOR_BLACK);   // A
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // +
        init_pair(4, COLOR_RED, COLOR_BLACK);     // -
        init_pair(5, COLOR_WHITE, COLOR_BLACK);   // #
    }

    // 키패드 활성화
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // getch가 non-blocking으로 실행되도록 함
}

int gamePlay(string StageName, int speed)
{
    // 게임 초기화
    Stage stage(StageName); // 스테이지 파일 경로
    Game game(stage, speed);

    // 지도와 점수 보드를 위한 창 생성
    int mapSize = stage.getMapSize();
    int scoreBoardWidth = 20;
    WINDOW *mapWin = newwin(mapSize, mapSize, 0, 0);
    WINDOW *scoreWin = newwin(mapSize, scoreBoardWidth, 0, mapSize);

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

        renderGame(mapWin, game, mapSize);
        renderScoreBoard(scoreWin, game, duration, score);
    }
}