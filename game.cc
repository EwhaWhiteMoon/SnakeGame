#include "enums.h"
#include "point.h"
#include "game.h"
#include "stage.h"
#include <optional>
#include <cstdlib>
#include <vector>
#include <algorithm>

Game::Game(Stage& S)
    : snake(S.getMapSize() / 2, S.getMapSize() / 2), stage(S), score(4, 0), direction({-1, 0}), timer(0), itemCnt(2, 0), gateTimer(0)
{
}

const vector<vector<mapTile>>& Game::getEntireMap(){
    return stage.getEntireMap();
}
const vector<point>& Game::getSnake(){
    return snake.getSnake();
}
const vector<int>& Game::getGoal(){
    return stage.getGoal();
}
const vector<int>& Game::getPoint(){
    return this->score;
}
const vector<bool>& Game::getDone(){
    vector<bool> *done = new vector<bool>(4, false);
    for(int i = 0; i < 4; i++){
        (*done)[i] = stage.getGoal()[i] <= score[i];
    }
    return *done;
}

bool Game::checkVaild(mapTile tile){
    return tile != mapTile::Wall && tile != mapTile::IWall && tile != mapTile::inVaild;
}

pair<int, int> Game::turnDir(const pair<int, int>& dir, int cnt){
    vector<pair<int, int>> dirList = {
        pair<int, int>({1, 0}),
        pair<int, int>({0, -1}),
        pair<int, int>({-1, 0}),
        pair<int, int>({0, 1}),
    };
    int dirInt;
    for(dirInt = 0; dir == dirList[dirInt]; dirInt++);
    return dirList[(dirInt + cnt + 16) % 4];
}

gameStatus Game::tick(pair<int, int> input, long long timestamp){

    if(timer < (timestamp / __MS_PER_TICK__)){
        timer += 1; // 다음 timer로 넘어갈 만큼 시간이 지났을 경우
    }else if(input == pair<int, int>({0 ,0})){
        return gameStatus::Progress; // 시간이 충분히 지나지 않았고 입력이 없을 경우
    }else{
        timer = timestamp / __MS_PER_TICK__; // 시간이 충분히 지나기 전에 입력이 들어왔다면, 시간이 지난 걸로 취급
        timer += 1;                          // (다음 timer 이벤트가 일어나지 않음)
    }

    if(input.first != 0 || input.second != 0) direction = input;
    
    // Movement
    point cur = snake.getHead();
    cur.x += direction.first;
    cur.y += direction.second;
    
    if(snake.isSnake(cur.x, cur.y)) return gameStatus::Lose;
 
    bool isGrowth = false;
    switch (stage.getMap(cur.x, cur.y)) {
        case mapTile::Wall:
        case mapTile::IWall:
        case mapTile::Snake:
            return gameStatus::Lose;

        case mapTile::Growth:
            isGrowth = true;
            itemCnt[0] -= 1;
            score[1] += 1;
            break;
        case mapTile::Poison:
            if(snake.cut() < 3) return gameStatus::Lose;
            itemCnt[1] -= 1;
            score[2] += 1;
            break;
        case mapTile::None:
            break;

        case mapTile::Portal:{
            auto gates = stage.getGate();
            if(!gates.has_value()) return gameStatus::Err;
            auto [a, b] = gates.value();
            if(cur.x == a.x) cur = b;
            else cur = a;
            
            int edge = stage.checkEdge(cur.x, cur.y);

            if(edge != 0){
                switch (edge) {
                    case 1: direction = {1, 0}; break;
                    case 2: direction = {0, -1}; break;
                    case 3: direction = {-1, 0}; break;
                    case 4: direction = {0, 1}; break;
                }
            }else{
                pair<int, int> startDir = direction;
                int cnt = 0;
                int turn[] = {1, -1, 2};
                while(!checkVaild(stage.getMap(cur.x + direction.first, cur.y + direction.second))){
                    direction = turnDir(startDir, turn[cnt ++]);
                }
            }
            cur.x += direction.first;
            cur.y += direction.second;
            score[3] += 1;}
            break;

        case mapTile::inVaild:
            return gameStatus::Err;
    }
    snake.moveTo(cur.x, cur.y, isGrowth);
    stage.setMap(cur.x, cur.y, mapTile::None); //새 머리가 들어갈 자리는 무조건 비어있음.

    score[0] = snake.len();
    auto done = getDone();
    if(all_of(done.begin(), done.end(), [](bool i){return i;}))
        return gameStatus::Win;

    update();

    return gameStatus::Progress;
}

void Game::update(){
    //item 등장
    if(itemCnt[0] <= 3){
        int x, y;
        do{
        x = rand() % stage.getMapSize();
        y = rand() % stage.getMapSize();
        }while(stage.getMap(x, y) != mapTile::None);
        stage.setMap(x, y, mapTile::Growth);
        itemCnt[0] += 1;
    }

    if(itemCnt[1] <= 3){
        int x, y;
        do{
        x = rand() % stage.getMapSize();
        y = rand() % stage.getMapSize();
        }while(stage.getMap(x, y) != mapTile::None);
        stage.setMap(x, y, mapTile::Poison);
        itemCnt[1] += 1;
    }
    //portal 등장
    if((timer - gateTimer) > 5 && snake.isConnected()){
        int x1, y1, x2, y2;
        do{
        x1 = rand() % stage.getMapSize();
        y1 = rand() % stage.getMapSize();
        x2 = rand() % stage.getMapSize();
        y2 = rand() % stage.getMapSize();
        }while(stage.getMap(x1, y1) != mapTile::Wall ||
                stage.getMap(x2, y2) != mapTile::Wall);

        stage.createGate(x1, y1, x2, y2);
        gateTimer = timer;
    }
}
