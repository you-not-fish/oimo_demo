#pragma once

#include <string>
#include <playerData.h>

struct Player {
    // 玩家名
    std::string name;
    // socket fd
    int fd;
    // 位置
    float x;
    float y;
    float z;
    // 欧拉角
    float ex;
    float ey;
    float ez;
    // 血量
    int hp;
    // 房间id
    int roomId;
    // 阵营
    int camp;
    // agent id
    uint32_t agent;
    // 其他数据
    PlayerData data;
};