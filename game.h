#pragma once

#include "enums.h"
#include "point.h"
#include "snake.h"
#include "stage.h"
#include <chrono>
#include <utility>
#include <vector>

class Game{
    private:
        Snake snake;
        Stage stage;
    public:
        Game(Stage&);
        gameStatus tick(pair<int, int> input, chrono::milliseconds timestamp);
        vector<vector<mapTile>>& getEntireMap();
        const vector<point>& getSnake();
        const vector<int>& getGoal();
        const vector<int>& getPoint();
        const vector<int>& getDone();
};
