#pragma once

enum class mapTile : short {
    inVaild = -1, // 잘못된 위치를 참조하였을 경우.
    None = 0,
    Wall = 1,
    IWall = 2,
    Growth = 3,
    Poison = 4,
    Portal = 5,
    Snake = 99,
};

enum class gameStatus : short {
    Err = - 1,
    Progress = 0,
    Lose = 1,
    Win = 2,
};
