#pragma once

#include <service.h>
#include <player.h>

using namespace Oimo;

class AgentService : public Service {
public:
    void init(Packle::sPtr packle) override;
private:
    void handlePlayerInfo(Packle::sPtr packle);
    void handleKick(Packle::sPtr packle);
    std::shared_ptr<Player> m_player;
};