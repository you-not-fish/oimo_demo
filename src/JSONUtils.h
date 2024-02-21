#pragma once

#include <jsoncpp/json/json.h>

class JSONUtils {
public:
    static bool  parse(const char* str, Json::Value& root) {
        Json::Reader reader;
        return reader.parse(str, root);
    }

    static bool  parse(const std::string& str, Json::Value& root) {
        Json::Reader reader;
        return reader.parse(str, root);
    }

    static std::string  stringify(const Json::Value& root, bool pretty = false) {
        if (pretty) {
            Json::StyledWriter writer;
            return writer.write(root);
        } else {
            Json::FastWriter writer;
            return writer.write(root);
        }
    }

    static char *serialize(const Json::Value& root, int &len) {
        std::string type = root["protoName"].asString();
        std::string body = stringify(root);
        int typeLen = type.size();
        int bodyLen = body.size();
        int msgLen = typeLen + bodyLen + 2;
        char *buf = new char[msgLen + 2];
        buf[0] = msgLen & 0xff;
        buf[1] = (msgLen >> 8) & 0xff;
        buf[2] = typeLen & 0xff;
        buf[3] = (typeLen >> 8) & 0xff;
        memcpy(buf + 4, type.c_str(), typeLen);
        memcpy(buf + 4 + typeLen, body.c_str(), bodyLen);
        len = msgLen + 2;
        return buf;
    }
};