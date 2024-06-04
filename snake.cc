#include <vector>
#include "snake.h"
#include "point.h"

Snake::Snake(int x, int y, int len)
    : body(), length(len)
{
    body.push_back(point(x, y));
}

const vector<point> &Snake::getSnake()
{
    vector<point>* s = new vector<point>(body.begin(), body.end());
    return *s;
}

const point &Snake::getHead()
{
    return body.back();
}

const point &Snake::getTail()
{
    return body.front();
}

bool Snake::isSnake(int x, int y)
{
    for(point p: body)
        if(p.x == x && p.y == y)
            return true;

    return false;
}

int Snake::len()
{
    return length;
}

int Snake::moveTo(int x, int y, bool isGrowth)
{
    body.push_back(point(x, y));
    if(!isGrowth) length += 1;
    if(!isGrowth || body.size() < length) cut();
    return len();
}

int Snake::cut()
{
    body.pop_front();
    length -= 1;
    return len();
}

bool Snake::isConnected()
{
    point cur = getTail();
    for(point p: body)
        if((cur.x - p.x) + (cur.y - cur.y) != 1)
            return false;
    return true;
}
