#pragma once

#include <cstring>
#include <string>

enum class MsgType {
    // 客户端注册
    MsgPing,
    MsgRegister,
    MsgLogin,
    MsgKick,
    MsgGetAchieve,
    MsgGetRoomList,
    MsgGetRoomInfo,
    MsgCreateRoom,
    MsgEnterRoom,
    MsgLeaveRoom,
    MsgStartBattle,
    MsgUnkown,

    // 系统内部消息
    Resp,
    UpdateAgent,
    ReqLogin,
    ReqKick,
    PlayerInfo,
    Kick,
    AddPlayer,
    RemovePlayer,
    GetRoomInfo,
    StartBattle,
    CreateRoom,
    RemoveRoom,
    GetRoom,
    ReqRoomInfo,
    GetRoomList,
};

inline MsgType toMsgType(int i) {
    return static_cast<MsgType>(i);
}

inline int toInt(MsgType type) {
    int i = static_cast<int>(type);
    return i;
}

inline MsgType toMsgType(const char* str, int len) {
    if (strncmp(str, "MsgPing", len) == 0) {
        return MsgType::MsgPing;
    } else if (strncmp(str, "MsgRegister", len) == 0) {
        return MsgType::MsgRegister;
    } else if (strncmp(str, "MsgLogin", len) == 0) {
        return MsgType::MsgLogin;
    } else if (strncmp(str, "MsgKick", len) == 0) {
        return MsgType::MsgKick;
    } else if (strncmp(str, "Resp", len) == 0) {
        return MsgType::Resp;
    } else if (strncmp(str, "UpdateAgent", len) == 0) {
        return MsgType::UpdateAgent;
    } else if (strncmp(str, "ReqLogin", len) == 0) {
        return MsgType::ReqLogin;
    } else if (strncmp(str, "ReqKick", len) == 0) {
        return MsgType::ReqKick;
    } else if (strncmp(str, "PlayerInfo", len) == 0) {
        return MsgType::PlayerInfo;
    } else if (strncmp(str, "Kick", len) == 0) {
        return MsgType::Kick;
    } else {
        return MsgType::MsgUnkown;
    }
}

inline const char* toChars(MsgType type) {
    switch (type) {
    case MsgType::MsgPing:
        return "MsgPing";
    case MsgType::MsgRegister:
        return "MsgRegister";
    case MsgType::MsgLogin:
        return "MsgLogin";
    case MsgType::MsgKick:
        return "MsgKick";
    case MsgType::Resp:
        return "Resp";
    case MsgType::UpdateAgent:
        return "UpdateAgent";
    case MsgType::ReqLogin:
        return "ReqLogin";
    case MsgType::ReqKick:
        return "ReqKick";
    case MsgType::PlayerInfo:
        return "PlayerInfo";
    case MsgType::Kick:
        return "Kick";
    default:
        return "MsgUnkown";
    }
}

inline MsgType toMsgType(const std::string& str) {
    return toMsgType(str.c_str(), str.size());
}

inline std::string toString(MsgType type) {
    return toChars(type);
}