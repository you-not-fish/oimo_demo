#include "agentMgr.h"
#include <service/agent.h>
#include <msgType.h>
#include <log.h>
#include <JSONUtils.h>
#include <application.h>

using namespace Oimo;

void AgentMgr::init(Packle::sPtr packle) {
    // 注册消息处理函数
    registerFunc((Packle::MsgID)MsgType::ReqLogin,
        std::bind(&AgentMgr::handleReqLogin, this, std::placeholders::_1));
}

void AgentMgr::handleReqLogin(Packle::sPtr packle) {
    auto node = std::any_cast<Json::Value>(packle->userData);
    std::string name = node["id"].asString();
    auto it = m_players.find(name);
    if (it != m_players.end()) {
        // 已经有其他玩家登录，将其踢下线
        auto player = it->second;
        kick(player->agent);
        //告知gateway
        Packle::sPtr pack = std::make_shared<Packle>(
            (Packle::MsgID)MsgType::UpdateAgent
        );
        pack->setFd(player->fd);
        pack->userData = 0;
        call("gateway", pack);
        m_players.erase(it);
        m_agents.erase(player->agent);
        return;
    }
    // 创建新的agent
    auto agent = 
        APP::instance().newService<AgentService>(name + "_agent");
    auto player = std::make_shared<Player>();
    player->name = name;
    player->agent = agent;
    m_players[name] = player;
    m_agents[agent] = player;
    // 向新建立的agent发送player信息
    Packle::sPtr pack = std::make_shared<Packle>(
        (Packle::MsgID)MsgType::PlayerInfo
    );
    pack->userData = player;
    call(agent, pack);
    LOG_INFO << "create agent for player: " << name;
    packle->userData = player->agent;
    setReturnPackle(packle);
}

void AgentMgr::handleReqKick(Packle::sPtr packle) {
    auto agent = std::any_cast<uint32_t>(packle->userData);
    kick(agent);
    auto it = m_agents.find(agent);
    if (it != m_agents.end()) {
        auto player = it->second;
        m_players.erase(player->name);
        m_agents.erase(agent);
    }
    setReturnPackle(packle);
}

void AgentMgr::kick(uint32_t agent) {
    Packle::sPtr pack = std::make_shared<Packle>(
        (Packle::MsgID)MsgType::Kick
    );
    call(agent, pack);
}