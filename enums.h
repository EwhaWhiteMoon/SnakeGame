#pragma once

#define screen_height 30
#define screen_width 90

enum class mapTile : short {
    inVaild = -1, // 잘못된 위치를 참조하였을 경우.
    None = 0,
    Wall = 1,
    IWall = 2,
    Growth = 3,
    Poison = 4,
    Portal = 5,
    Super = 6,
    Snake = 99,
};

enum class gameStatus : short {
    Err = - 1,
    Progress = 0,
    Lose = 1,
    Win = 2,
};

enum class menuStatus : short {
    Main,
    Playing,
    Over,
    End,
    Next,
};
