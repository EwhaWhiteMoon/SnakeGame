#pragma once

#include "enums.h"
#include "point.h"
#include <vector>
using namespace std;

class Stage{
    public:
        mapTile getMap(int x, int y);
        void setMap(int x, int y, mapTile data);
        const vector<vector<mapTile>>& getEntireMap();
        const vector<int>& getGoal();
        const pair<point, point>& getGate();
        const int checkEdge(int x, int y); //not edge 0, up 1, right 2, down 3, left 4
        void createGate(int x1, int y1, int x2, int y2);
};
