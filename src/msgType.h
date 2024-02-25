#pragma once

#include <cstring>
#include <string>

enum class MsgType {
    // 客户端注册
    MsgPing = 0,
    MsgRegister = 1,
    MsgLogin = 2,
    MsgKick = 3,
    MsgGetAchieve = 4,
    MsgGetRoomList = 5,
    MsgGetRoomInfo = 6,
    MsgCreateRoom = 7,
    MsgEnterRoom = 8,
    MsgLeaveRoom = 9,
    MsgStartBattle = 10,
    MsgSyncTank = 11,
    MsgFire = 12,
    MsgHit = 13,
    MsgUnkown = 14,

    // 系统内部消息
    Resp = 100,
    UpdateAgent = 101,
    ReqLogin = 102,
    ReqKick = 103,
    PlayerInfo = 104,
    Kick = 105,
    AddPlayer = 106,
    RemovePlayer = 107,
    GetRoomInfo = 108,
    StartBattle = 109,
    CreateRoom = 110,
    RemoveRoom = 111,
    GetRoom = 112,
    ReqRoomInfo = 113,
    GetRoomList = 114,
    SyncTank = 115,
    Fire = 116,
    Hit = 117,
};

inline MsgType toMsgType(int i) {
    return static_cast<MsgType>(i);
}

inline int toInt(MsgType type) {
    int i = static_cast<int>(type);
    return i;
}

inline MsgType toMsgType(const char* str, int len) {
    #define TO_MSG_TYPE(msg, msglen) \
        if (len == msglen && strncmp(str, #msg, msglen) == 0) { \
            return MsgType::msg; \
        }
    TO_MSG_TYPE(MsgPing, 7);
    TO_MSG_TYPE(MsgRegister, 11);
    TO_MSG_TYPE(MsgLogin, 8);
    TO_MSG_TYPE(MsgKick, 7);
    TO_MSG_TYPE(MsgGetAchieve, 13);
    TO_MSG_TYPE(MsgGetRoomList, 14);
    TO_MSG_TYPE(MsgGetRoomInfo, 14);
    TO_MSG_TYPE(MsgCreateRoom, 13);
    TO_MSG_TYPE(MsgEnterRoom, 12);
    TO_MSG_TYPE(MsgLeaveRoom, 12);
    TO_MSG_TYPE(MsgStartBattle, 14);
    TO_MSG_TYPE(MsgSyncTank, 11);
    TO_MSG_TYPE(MsgFire, 7);
    TO_MSG_TYPE(MsgHit, 6);
    TO_MSG_TYPE(MsgUnkown, 9);
    TO_MSG_TYPE(Resp, 4);
    TO_MSG_TYPE(UpdateAgent, 11);
    TO_MSG_TYPE(ReqLogin, 8);
    TO_MSG_TYPE(ReqKick, 7);
    TO_MSG_TYPE(PlayerInfo, 10);
    TO_MSG_TYPE(Kick, 4);
    TO_MSG_TYPE(AddPlayer, 9);
    TO_MSG_TYPE(RemovePlayer, 12);
    TO_MSG_TYPE(GetRoomInfo, 10);
    TO_MSG_TYPE(StartBattle, 11);
    TO_MSG_TYPE(CreateRoom, 10);
    TO_MSG_TYPE(RemoveRoom, 10);
    TO_MSG_TYPE(GetRoom, 7);
    TO_MSG_TYPE(ReqRoomInfo, 11);
    TO_MSG_TYPE(GetRoomList, 11);
    TO_MSG_TYPE(SyncTank, 8);
    TO_MSG_TYPE(Fire, 4);
    TO_MSG_TYPE(Hit, 3);
    #undef TO_MSG_TYPE
    return MsgType::MsgUnkown;
}

inline const char* toChars(MsgType type) {
    switch (type) {
        #define TO_CHARS(msg) \
            case MsgType::msg: \
                return #msg;
        TO_CHARS(MsgPing);
        TO_CHARS(MsgRegister);
        TO_CHARS(MsgLogin);
        TO_CHARS(MsgKick);
        TO_CHARS(MsgGetAchieve);
        TO_CHARS(MsgGetRoomList);
        TO_CHARS(MsgGetRoomInfo);
        TO_CHARS(MsgCreateRoom);
        TO_CHARS(MsgEnterRoom);
        TO_CHARS(MsgLeaveRoom);
        TO_CHARS(MsgStartBattle);
        TO_CHARS(MsgSyncTank);
        TO_CHARS(MsgFire);
        TO_CHARS(MsgHit);
        TO_CHARS(MsgUnkown);
        TO_CHARS(Resp);
        TO_CHARS(UpdateAgent);
        TO_CHARS(ReqLogin);
        TO_CHARS(ReqKick);
        TO_CHARS(PlayerInfo);
        TO_CHARS(Kick);
        TO_CHARS(AddPlayer);
        TO_CHARS(RemovePlayer);
        TO_CHARS(GetRoomInfo);
        TO_CHARS(StartBattle);
        TO_CHARS(CreateRoom);
        TO_CHARS(RemoveRoom);
        TO_CHARS(GetRoom);
        TO_CHARS(ReqRoomInfo);
        TO_CHARS(GetRoomList);
        TO_CHARS(SyncTank);
        TO_CHARS(Fire);
        TO_CHARS(Hit);
        #undef TO_CHARS
    default:
        return "Unkown";
    }
}

inline MsgType toMsgType(const std::string& str) {
    return toMsgType(str.c_str(), str.size());
}

inline std::string toString(MsgType type) {
    return toChars(type);
}