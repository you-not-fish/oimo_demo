#include "agent.h"
#include <msgType.h>
#include <JSONUtils.h>
#include <log.h>

using namespace Oimo;

void AgentService::init(Packle::sPtr packle) {
    m_room = 0;
    registerFunc((Packle::MsgID)MsgType::PlayerInfo,
        std::bind(&AgentService::handlePlayerInfo, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::Kick,
        std::bind(&AgentService::handleKick, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::MsgGetAchieve,
        std::bind(&AgentService::handleMsgGetAchieve, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::MsgGetRoomList,
        std::bind(&AgentService::handleMsgGetRoomList, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::MsgCreateRoom,
        std::bind(&AgentService::handleMsgCreateRoom, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::MsgEnterRoom,
        std::bind(&AgentService::handleMsgEnterRoom, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::MsgGetRoomInfo,
        std::bind(&AgentService::handleMsgGetRoomInfo, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::MsgLeaveRoom,
        std::bind(&AgentService::handleMsgLeaveRoom, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::StartBattle,
        std::bind(&AgentService::handleStartBattle, this, std::placeholders::_1));
}


void AgentService::handlePlayerInfo(Packle::sPtr packle) {
    auto player = std::any_cast<std::shared_ptr<Player> >(packle->userData);
    m_player = player;
}

void AgentService::handleKick(Packle::sPtr packle) {
    if (m_player) {
        LOG_INFO << "kick player: " << m_player->name;
        if (m_player->roomId > 0) {
            if (m_room == 0) {
                LOG_ERROR << "can't find room room service! room id: " << m_player->roomId;
            } else {
                Packle::sPtr pack = std::make_shared<Packle>(
                    (Packle::MsgID)MsgType::RemovePlayer
                );
                pack->userData = m_player->name;
                call(m_room, pack);
                m_room = 0;
            }
        }
    }
    setReturnPackle(packle);
}

void AgentService::ret(Packle::sPtr packle) {
    packle->setType(
        (Packle::MsgID)MsgType::Resp
    );
    send("gateway", packle);
}

void AgentService::handleMsgGetAchieve(Packle::sPtr packle) {
    Json::Value node;
    node["protoName"] = "MsgGetAchieve";
    node["win"] = m_player->data.win;
    node["lost"] = m_player->data.lose;
    packle->userData = node;
    ret(packle);
}

void AgentService::handleMsgGetRoomList(Packle::sPtr packle) {
    Packle::sPtr pack = std::make_shared<Packle>(
        (Packle::MsgID)MsgType::GetRoomList
    );
    call("roommgr", pack);
    ret(responsePackle());
}

void AgentService::handleMsgCreateRoom(Packle::sPtr packle) {
    auto node = std::any_cast<Json::Value>(packle->userData);
    if (m_player->roomId > 0) {
        LOG_ERROR << "player " << m_player->name << " already in room " << m_player->roomId;
        node["result"] = 1;
        packle->userData = node;
        ret(packle);
        return;
    }
    Packle::sPtr pack = std::make_shared<Packle>(
        (Packle::MsgID)MsgType::CreateRoom
    );
    call("roommgr", pack);
    auto resp = responsePackle();
    m_room = std::any_cast<int>(resp->userData);
    ret(responsePackle());
}