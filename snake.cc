#include <iostream>
#include <vector>
#include "Snake.h"

POSITION::POSITION(int row, int col) {
    x = col;
    y = row;
}

POSITION::POSITION() {
    x = 0;
    y = 0;
}

bool POSITION::operator==(POSITION p) {
    return ((x == p.x) && (y == p.y));
}

Snake::Snake(Map& m) {
    direction = 'l';
    maxlen = 3;
}

/*각 요소의 위치를 머리-꼬리 순서를 백터로*/
const vector<point>& getSnake() {
    return Snake;
}

/*snake 머리 위치*/
const point& getHead() {
    return Snake[0];
}

/*snake 길이*/
int len() {
    return Snake.size();
}

/*snake max길이*/
int getmaxlen() {
    return maxlen;
}

void setmaxlen() {
    if (len() > maxlen) {
        maxlen = len();
}

/*이동, isGrowth가 참이면 +1*/
int moveTo(int x, int y, bool isGrowth) {
    len.insert(len.begin(), point(x, y));
    if (!isGrowth) {
        Snake.pop_back();
    }
    return Snake.size();
}

/*snake 줄어듦*/
int cut() {
    if (!Snake.empty()) {
        Snake.pop_back();
    }
    return Snake.size();
}

/*x,y위치에 snake가 있는지*/
bool isSnake(int x, int y) {
    for (const auto& pos : Snake) {
        if (pos.x == x && pos.y == y) {
            return true;
        }
    }
    return false;
}
