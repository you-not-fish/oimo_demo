#include "agent.h"
#include <msgType.h>
#include <log.h>

using namespace Oimo;

void AgentService::init(Packle::sPtr packle) {
    m_room = 0;
    registerFunc((Packle::MsgID)MsgType::PlayerInfo,
        std::bind(&AgentService::handlePlayerInfo, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::Kick,
        std::bind(&AgentService::handleKick, this, std::placeholders::_1));
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