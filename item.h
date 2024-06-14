#pragma once
#include "point.h"
#include "enums.h"
#include <algorithm>
#include <vector>

using namespace std;

class Item{
    public:
        point pos;
        mapTile itemType;
        long long time;
        Item(int x, int y, mapTile type, int time) : pos(x, y), itemType(type), time(time){
        }
        bool operator<(const Item& i) const{
            return (time + 16 * pos.x + 64 * pos.y) < (time + 16 * i.pos.x + 64 * i.pos.y);
        }
};