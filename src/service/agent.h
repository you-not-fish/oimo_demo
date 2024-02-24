#pragma once

#include <service.h>
#include <player.h>

using namespace Oimo;

class AgentService : public Service {
public:
    void init(Packle::sPtr packle) override;
private:
    void ret(Packle::sPtr packle);
    void handlePlayerInfo(Packle::sPtr packle);
    void handleKick(Packle::sPtr packle);
    void handleMsgGetAchieve(Packle::sPtr packle);
    void handleMsgGetRoomList(Packle::sPtr packle);
    void handleMsgCreateRoom(Packle::sPtr packle);
    void handleMsgEnterRoom(Packle::sPtr packle);
    void handleMsgGetRoomInfo(Packle::sPtr packle);
    void handleMsgLeaveRoom(Packle::sPtr packle);
    void handleMsgStartBattle(Packle::sPtr packle);
    uint32_t m_room;
    std::shared_ptr<Player> m_player;
};