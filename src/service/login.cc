#include <msgType.h>
#include <log.h>
#include <JSONUtils.h>
#include "login.h"

using namespace Oimo;

void LoginService::init(Packle::sPtr packle) {
    // 注册消息处理
    registerFunc(toInt(MsgType::MsgRegister),
        std::bind(&LoginService::handleRegister, this, std::placeholders::_1)
    );
    registerFunc(toInt(MsgType::MsgLogin),
        std::bind(&LoginService::handleLogin, this, std::placeholders::_1)
    );
}

void LoginService::handleRegister(Packle::sPtr packle) {
    // 解析JSON
    Json::Value root;
    if (!JSONUtils::parse(packle->buf(), root)) {
        LOG_ERROR << "parse json error";
        return;
    }
    // 处理注册消息
    root["result"] = 0;
    LOG_DEBUG << "register response: " << JSONUtils::stringify(root);
    // 发送注册响应
    int len;
    packle->setBuf(JSONUtils::serialize(root, len));
    packle->setSize(len);
    setReturnPackle(packle);
}

void LoginService::handleLogin(Packle::sPtr packle) {
    // 处理登录消息
    LOG_DEBUG << "handle login";
}