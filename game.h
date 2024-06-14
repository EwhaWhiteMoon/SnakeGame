#pragma once

#include "enums.h"
#include "point.h"
#include "snake.h"
#include "stage.h"
#include <utility>
#include <vector>

class Game{
    private:
        Snake snake;
        Stage& stage;
        vector<int> score;
        pair<int, int> direction;
        long long timer;
        long long last_updated;
        void update();
        static bool checkVaild(mapTile tile);
        static pair<int, int> turnDir(const pair<int, int>& dir, int cnt);
        int gateTimer;
        int gameSpeed;

    public:
        Game(Stage&, int speed = 500);
        gameStatus tick(pair<int, int> input, long long timestamp);
        const vector<vector<mapTile>>& getEntireMap();
        const vector<point>& getSnake();
        const vector<int>& getGoal();
        const vector<int>& getPoint();
        const vector<bool>& getDone();
};
