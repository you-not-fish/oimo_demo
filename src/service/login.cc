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
    // 处理注册消息
    auto node = std::any_cast<Json::Value>(packle->userData);
    node["result"] = 0;
    LOG_DEBUG << "register response: " << JSONUtils::stringify(node);
    // 返回注册结果
    packle->userData = node;
    packle->setType(toInt(MsgType::Resp));
    send(packle->source(), packle);
}

void LoginService::handleLogin(Packle::sPtr packle) {
    // 处理登录消息
    auto node = std::any_cast<Json::Value>(packle->userData);
    do {
        // 校验用户名和密码
        if (node["pw"].asString() != "123") {
            node["result"] = 1;
            break;
        }
        // 向AgentMgr请求建立agent
        Packle::sPtr req = std::make_shared<Packle>(
            toInt(MsgType::ReqLogin)
        );
        req->userData = node;
        call("agentmgr", req);
        auto resp = responsePackle();
        auto agent = std::any_cast<uint32_t>(resp->userData);
        if (agent == 0) {
            node["result"] = 1;
            break;
        }
        // 告知gateway
        Packle::sPtr pack = std::make_shared<Packle>(
            toInt(MsgType::UpdateAgent)
        );
        pack->setFd(packle->fd());
        pack->userData = agent;
        call("gateway", pack);
        // 登录成功
        node["result"] = 0;
    } while (0);
    // 返回登录结果
    packle->setType(toInt(MsgType::Resp));
    packle->userData = node;
    send(packle->source(), packle);
}