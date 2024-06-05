#include <vector>
#include <string>
#include <fstream>
#include "stage.h"
#include "enums.h"
#include "point.h"

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
        //예외처리
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
    //예외처리 추가하면 좋음
    return map[x][y];
}

void Stage::setMap(int x, int y, mapTile data)
{   
    map[x][y] = data;
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

const int Stage::checkEdge(int x, int y) //not edge 0, up 1, right 2, down 3, left 4
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
