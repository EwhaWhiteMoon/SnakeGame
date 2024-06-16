#pragma once

#include "enums.h"
#include "point.h"
#include <vector>
#include <deque>

using namespace std;

class Snake{
    public:
        Snake(int x, int y, int len = 3);
        const vector<point>& getSnake();
        const point& getHead();
        const point& getTail();
        bool isSnake(int x, int y);
        int len();
        int moveTo(int x, int y, bool isGrowth);
        int cut();
        void doubleLen();
        bool isConnected();
    private:
        deque<point> body;
        int length;
};
