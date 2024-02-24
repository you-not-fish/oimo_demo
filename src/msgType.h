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
    if (len == 7 && strncmp(str, "MsgPing", 7) == 0) {
        return MsgType::MsgPing;
    } else if (len == 10 && strncmp(str, "MsgRegister", 10) == 0) {
        return MsgType::MsgRegister;
    } else if (len == 8 && strncmp(str, "MsgLogin", 8) == 0) {
        return MsgType::MsgLogin;
    } else if (len == 7 && strncmp(str, "MsgKick", 7) == 0) {
        return MsgType::MsgKick;
    } else if (len == 12 && strncmp(str, "MsgGetAchieve", 12) == 0) {
        return MsgType::MsgGetAchieve;
    } else if (len == 13 && strncmp(str, "MsgGetRoomList", 13) == 0) {
        return MsgType::MsgGetRoomList;
    } else if (len == 13 && strncmp(str, "MsgGetRoomInfo", 13) == 0) {
        return MsgType::MsgGetRoomInfo;
    } else if (len == 12 && strncmp(str, "MsgCreateRoom", 12) == 0) {
        return MsgType::MsgCreateRoom;
    } else if (len == 11 && strncmp(str, "MsgEnterRoom", 11) == 0) {
        return MsgType::MsgEnterRoom;
    } else if (len == 11 && strncmp(str, "MsgLeaveRoom", 11) == 0) {
        return MsgType::MsgLeaveRoom;
    } else if (len == 12 && strncmp(str, "MsgStartBattle", 12) == 0) {
        return MsgType::MsgStartBattle;
    } else if (len == 9 && strncmp(str, "MsgUnkown", 9) == 0) {
        return MsgType::MsgUnkown;
    } else if (len == 4 && strncmp(str, "Resp", 4) == 0) {
        return MsgType::Resp;
    } else if (len == 11 && strncmp(str, "UpdateAgent", 11) == 0) {
        return MsgType::UpdateAgent;
    } else if (len == 8 && strncmp(str, "ReqLogin", 8) == 0) {
        return MsgType::ReqLogin;
    } else if (len == 7 && strncmp(str, "ReqKick", 7) == 0) {
        return MsgType::ReqKick;
    } else if (len == 10 && strncmp(str, "PlayerInfo", 10) == 0) {
        return MsgType::PlayerInfo;
    } else if (len == 4 && strncmp(str, "Kick", 4) == 0) {
        return MsgType::Kick;
    } else if (len == 9 && strncmp(str, "AddPlayer", 9) == 0) {
        return MsgType::AddPlayer;
    } else if (len == 11 && strncmp(str, "RemovePlayer", 11) == 0) {
        return MsgType::RemovePlayer;
    } else if (len == 11 && strncmp(str, "GetRoomInfo", 11) == 0) {
        return MsgType::GetRoomInfo;
    } else if (len == 11 && strncmp(str, "StartBattle", 11) == 0) {
        return MsgType::StartBattle;
    } else if (len == 11 && strncmp(str, "CreateRoom", 11) == 0) {
        return MsgType::CreateRoom;
    } else if (len == 11 && strncmp(str, "RemoveRoom", 11) == 0) {
        return MsgType::RemoveRoom;
    } else if (len == 8 && strncmp(str, "GetRoom", 8) == 0) {
        return MsgType::GetRoom;
    } else if (len == 10 && strncmp(str, "ReqRoomInfo", 10) == 0) {
        return MsgType::ReqRoomInfo;
    } else if (len == 11 && strncmp(str, "GetRoomList", 11) == 0) {
        return MsgType::GetRoomList;
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
    case MsgType::MsgGetAchieve:
        return "MsgGetAchieve";
    case MsgType::MsgGetRoomList:
        return "MsgGetRoomList";
    case MsgType::MsgGetRoomInfo:
        return "MsgGetRoomInfo";
    case MsgType::MsgCreateRoom:
        return "MsgCreateRoom";
    case MsgType::MsgEnterRoom:
        return "MsgEnterRoom";
    case MsgType::MsgLeaveRoom:
        return "MsgLeaveRoom";
    case MsgType::MsgStartBattle:
        return "MsgStartBattle";
    case MsgType::MsgUnkown:
        return "MsgUnkown";
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
    case MsgType::AddPlayer:
        return "AddPlayer";
    case MsgType::RemovePlayer:
        return "RemovePlayer";
    case MsgType::GetRoomInfo:
        return "GetRoomInfo";
    case MsgType::StartBattle:
        return "StartBattle";
    case MsgType::CreateRoom:
        return "CreateRoom";
    case MsgType::RemoveRoom:
        return "RemoveRoom";
    case MsgType::GetRoom:
        return "GetRoom";
    case MsgType::ReqRoomInfo:
        return "ReqRoomInfo";
    case MsgType::GetRoomList:
        return "GetRoomList";
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