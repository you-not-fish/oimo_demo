#pragma once

#include <service.h>
#include <string>
#include <player.h>
#include <map>

using namespace Oimo;

class AgentMgr : public Service {
public:
    void init(Packle::sPtr packle) override;
private:
    void handleReqLogin(Packle::sPtr packle);
    void handleReqKick(Packle::sPtr packle);
    void kick(uint32_t agent);
    std::map<std::string, std::shared_ptr<Player> > m_players;
    std::map<uint32_t, std::shared_ptr<Player> > m_agents;
};