#include <vector>
#include <string>
#include <fstream>
#include "stage.h"
#include "enums.h"
#include "point.h"
#include "item.h"
#include <set>

using namespace std;

Stage::Stage(int size, vector<int> goals)
    :map(size, vector<mapTile>(size, mapTile::None)), goal(goals), gate(nullopt), size(size)
{
}

Stage::Stage(string fileName) : goal(4, 0), gate(nullopt)
{
    ifstream mapFile;
    mapFile.open(fileName, ios_base::in);
    if(!mapFile.is_open()){
        throw "No map file";
    }

    mapFile >> size;
    mapFile >> goal[0] >> goal[1] >> goal[2] >> goal[3];
    map = vector<vector<mapTile>>(size, vector<mapTile>(size, mapTile::None));

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            int tile;
            mapFile >> tile;
            map[i][j] = (mapTile)tile;
        }
    }
    

    mapFile.close();
}

mapTile Stage::getMap(int x, int y)
{
    if(0 <= x && x < size && y <= x && y < size)
        return map[x][y];
    else return mapTile::inVaild;
}

void Stage::setMap(int x, int y, mapTile data)
{   
    mapTile cur = getMap(x, y);
    if(cur == mapTile::Growth or cur == mapTile::Poison){
        removeItem(x, y);
    }
    map[x][y] = data;
}

void Stage::removeItem(int x, int y)
{
    for (auto it = itemSet.begin(); it != itemSet.end();) {
        if (it->pos.x == x and it->pos.y == y){
            map[it->pos.x][it->pos.y] = mapTile::None;
            it = itemSet.erase(it);
        }else{
            it++;
        }
    }
}

bool Stage::addItem(int x, int y, mapTile item, int time)
{
    if(getMap(x, y) != mapTile::None) return false;
    
    itemSet.insert(Item(x, y, item, time));
    setMap(x, y, item);
    return true;
}

void Stage::itemTick(int time)
{
    for (auto it = itemSet.begin(); it != itemSet.end();) {
        if (it->time < time){
            map[it->pos.x][it->pos.y] = mapTile::None;
            it = itemSet.erase(it);
        }else{
            it++;
        }
    }
}

int Stage::countItem(mapTile item){
    int cnt = 0;
    for(auto i: itemSet)
        if(i.itemType == item) cnt += 1;
    return cnt;
}

const vector<vector<mapTile>> &Stage::getEntireMap()
{
    return map;
}

const vector<int> &Stage::getGoal()
{
    return goal;
}

const optional<pair<point, point>> &Stage::getGate()
{
    return gate;
}

const int Stage::checkEdge(int x, int y)
{
    if (x == 0) return 1;
    if (x == map[0].size() - 1) return 3;
    if (y == 0) return 4; 
    if (y == map.size() - 1) return 2; 
    return 0; 
}

void Stage::removeGate()
{
    if(!gate.has_value()) return;
    auto [a, b] = gate.value();
    setMap(a.x, a.y, mapTile::Wall);
    setMap(b.x, b.y, mapTile::Wall);
    gate = nullopt;
}

void Stage::createGate(int x1, int y1, int x2, int y2)
{
    removeGate();
    point a = point(x1, y1);
    point b = point(x2, y2);
    setMap(a.x, a.y, mapTile::Portal);
    setMap(b.x, b.y, mapTile::Portal);
    gate = pair<point, point>(a, b);
}

int Stage::getMapSize()
{
    return size;
}
