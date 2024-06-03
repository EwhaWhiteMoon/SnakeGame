#include <vector>
#include "Snake.h"

Snake::Snake(Map& m) {
    direction = 'l';
    maxlen = 3;
}

/*각 요소의 위치를 머리-꼬리 순서를 백터로*/
const vector<point>& Snake::getSnake() {
    return Snake;
}

/*snake 머리 위치*/
const point& Snake::getHead() {
    return Snake[0];
}

/*snake 길이*/
int Snake::len() {
    return Snake.size();
}

/*snake max길이*/
int Snake::getMaxLen() {
    return maxlen;
}

void Snake::setMaxLen() {
    if (len() > maxlen) {
        maxlen = len();
}

/*이동, isGrowth가 참이면 +1*/
int Snake::moveTo(int x, int y, bool isGrowth) {
    Snake[0].x = x;
    Snake[0].y = y;
    if (isGrowth) {
        Snake[Snake.size() + 1].x = x;
        Snake[Snake.size() + 1].y = y;
    }
    return len();
}

/*snake 줄어듦*/
int Snake::cut() {
    Snake[Snake.size() + 1].x;
    Snake[Snake.size() + 1].y;
    Snake.pop_back();
    return len();
}

/*x,y위치에 snake가 있는지*/
bool isSnake(int x, int y) {
    if (isSnake(x, y)) {
        return true;
    }
    else {
        return false;
    }
}
