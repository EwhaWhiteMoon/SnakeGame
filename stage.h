#include "enums.h"
#include "point.h"
#include <vector>
using namespace std;

class Stage{
    public:
        mapTile getMap(int x, int y);
        void setMap(int x, int y, mapTile data);
        const vector<vector<mapTile>>& getEntireMap();
        const vector<int> getGoal();
        const pair<point, point> getGate();
};
