#pragma once

#include "enums.h"
#include "point.h"
#include "snake.h"
#include "stage.h"
#include <utility>
#include <vector>

#define __MS_PER_TICK__ 1000

class Game{
    private:
        Snake snake;
        Stage& stage;
        vector<int> score;
        pair<int, int> direction;
        long long timer;
        void update();
        static bool checkVaild(mapTile tile);
        static pair<int, int> turnDir(const pair<int, int>& dir, int cnt);

    public:
        Game();
        Game(Stage&);
        gameStatus tick(pair<int, int> input, long long timestamp);
        const vector<vector<mapTile>>& getEntireMap();
        const vector<point>& getSnake();
        const vector<int>& getGoal();
        const vector<int>& getPoint();
        const vector<bool>& getDone();
};
