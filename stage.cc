#include <vector>
#include "Stage.h"
#include "enums.h"
#include "point.h"


Stage::Stage() {}

Stage::Stage(int map) {}

/*x, y 반환*/ 
mapTile Stage::getMap(int x, int y) {
    if (x >= 0 && x < map[0].size() && y >= 0 && y < map.size()) {
        return map[y][x];
    }
    return mapTile::inVaild;
}

/*x, y 요소 설정*/
void Stage::setMap(int x, int y, mapTile data) {
    if (x >= 0 && x < map[0].size() && y >= 0 && y < map.size()) {
        map[y][x] = data;
    }
}

/*맵 전체 백터 반환*/
const vector<vector<mapTile>>& Stage::getEntireMap() {
    return map;
}

/*스테이지 목표를 vector로 반환*/ 
const vector<int>& Stage::getGoal() {
    return goal;
}

/*존재하는 gate위치 반환*/
const pair<point, point>& Stage::getGate() {
    return gate;
}

/*특정 좌표가 모서리에 위치하는지 확인, 어느모서리에 위치하는지 반환*/ 
const int Stage::checkEdge(int x, int y) {
    if (x == 0) return 4;
    if (x == map[0].size() - 1) return 2;
    if (y == 0) return 1; 
    if (y == map.size() - 1) return 3; 
    return 0; 
}

/*꼭짓점에 위치하는 경우는 0 이외 어느 값을 반환해도 됨*/
void Stage::createGate(int x1, int y1, int x2, int y2) {
    gate = { point(x1, y1), point(x2, y2) };
    setMap(x1, y1, mapTile::Portal);
    setMap(x2, y2, mapTile::Portal);
}
