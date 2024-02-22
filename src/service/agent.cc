#include "agent.h"
#include <msgType.h>
#include <log.h>

using namespace Oimo;

void AgentService::init(Packle::sPtr packle) {
    registerFunc((Packle::MsgID)MsgType::PlayerInfo,
        std::bind(&AgentService::handlePlayerInfo, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::Kick,
        std::bind(&AgentService::handleKick, this, std::placeholders::_1));
}