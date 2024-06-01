### Main 함수
Game을 생성해서 초기화한다.
매 틱마다 game.tick을 호출해서 입력(키코드를 x, y 방향을 가진 pair로 변경해서 전달)과 timestamp를 ms로 전달한다
game함수의 getEntireMap, getGoal, getPoint, getDone 등 참조해서 스크린에 UI를 그린다.
game.tick의 반환값을 참조하여 게임 오버 스크린 출력, 다음 스테이지로 진출 등을 진행한다.

입력값이 없으면 0, 0으로 처리한다

### Game 클래스
한 스테이지의 게임을 담당한다
생성자 : Stage 클래스를 받는다.
gameStatus game.tick(input, timestamp)
const vector<vector<mapTile>>& getEntireMap() : 맵 전체를 벡터로 반환한다.
const vector<point>& getSnake() : snake 각 요소의 위치를 머리부터 꼬리 순서로 벡터로 반환한다.

const vector<int> getGoal() : 스테이지 목표를 vector로 반환한다. 스테이지 목표는 목표 길이, Growth 획득수, Poison 획득 수, Gate 사용 횟수 순서로 저장한다.
const vector<int> getPoint() : 스테이지 목표에 따른 현재 달성량을 vector로 반환한다.
const vector<bool> getDone() : 각 스테이지 목표의 완료 여부를 vector로 반환한다.

### Stage 클래스
스테이지를 저장한다
생성자 : stage 파일의 경로를 받는다
mapTile getMap(int x, int y) : x, y 위치에 있는 요소를 반환한다
void setMap(int x, int y, mapTile data) : x, y 위치에 있는 요소를 설정한다
const vector<vector<mapTile>>& getEntireMap() : 맵 전체를 벡터로 반환한다.
const vector<int> getGoal() : 스테이지 목표를 vector로 반환한다. 스테이지 목표는 목표 길이, Growth 획득수, Poison 획득 수, Gate 사용 횟수 순서로 저장한다.
const pair<point, point> getGate() : 현재 존재하는 Gate의 위치를 반환한다.

### Snake 클래스
스네이크를 저장한다
const vector<point>& getSnake() : snake 각 요소의 위치를 머리부터 꼬리 순서로 벡터로 반환한다.
const point& getHead() : snake의 머리 위치를 반환한다
int len() : snake의 길이를 반환한다
int moveTo(int x, int y, bool isGrowth) : x, y 위치로 머리를 이동시킨다, isGrowth가 참이라면 길이를 1 늘리며 이동하고, 아니면 길이를 보존한다. 이동을 마친 뒤 snake의 길이를 반환한다.
int cut() : 꼬리를 하나 줄인다. 그 후 snake의 길이를 반환한다.
