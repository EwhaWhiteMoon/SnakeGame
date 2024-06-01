#pragma once

#include "enums.h"
#include "point.h"
#include <vector>

using namespace std;

class Snake{
    public:
        const vector<point>& getSnake();
        const point& getHead();
        bool isSnake(int x, int y);
        int len();
        int moveTo(int x, int y, bool isGrowth);
        int cut();
};
