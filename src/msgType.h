#pragma once

#include <cstring>
#include <string>

enum class MsgType {
    MsgRegister,
    MsgLogin,
    MsgUnkown
};

inline MsgType toMsgType(int i) {
    return static_cast<MsgType>(i);
}

inline int toInt(MsgType type) {
    return static_cast<int>(type);
}

inline MsgType toMsgType(const char* str, int len) {
    if (len == 11 && strncmp(str, "MsgRegister", 11) == 0) {
        return MsgType::MsgRegister;
    } else if (len == 7 && strncmp(str, "MsgLogin", 7) == 0) {
        return MsgType::MsgLogin;
    } else {
        return MsgType::MsgUnkown;
    }
}

inline const char* toChars(MsgType type) {
    switch (type) {
    case MsgType::MsgRegister:
        return "MsgRegister";
    case MsgType::MsgLogin:
        return "MsgLogin";
    default:
        return "MsgUnkown";
    }
}

inline MsgType toMsgType(const std::string& str) {
    return toMsgType(str.c_str());
}

inline std::string toString(MsgType type) {
    return toChars(type);
}