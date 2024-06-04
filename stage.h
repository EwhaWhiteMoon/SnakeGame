#pragma once

#include "enums.h"
#include "point.h"
#include <vector>
#include <string>
#include <optional>
using namespace std;

class Stage{
    public:
        Stage(int size, vector<int> goal);
        Stage(string fileName);
        mapTile getMap(int x, int y);
        void setMap(int x, int y, mapTile data);
        const vector<vector<mapTile>>& getEntireMap();
        const vector<int>& getGoal();
        const optional<pair<point, point>>& getGate();
        const int checkEdge(int x, int y); //not edge 0, up 1, right 2, down 3, left 4
        void createGate(int x1, int y1, int x2, int y2);
        void removeGate();
        int getMapSize();
    private:
        vector<vector<mapTile>> map;
        vector<int> goal;
        optional<pair<point, point>> gate;
        int size;
};
