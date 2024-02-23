#pragma once

#include <service.h>
#include <string>
#include <map>
#include <JSONUtils.h>

using namespace Oimo;

class Player;
enum class RoomStatus {
    Preparing = 0,
    Fighting
};

class RoomService : public Service {
public:
    ~RoomService();
    void init(Packle::sPtr packle) override;
private:
    void handleAddPlayer(Packle::sPtr packle);
    void handleRemovePlayer(Packle::sPtr packle);
    void handleStartBattle(Packle::sPtr packle);
    void handleReqRoomInfo(Packle::sPtr packle);
    int switchCamp();
    std::string switchOwner();
    void broadcast(Json::Value node);
    Json::Value roomInfo();
    Json::Value leaveInfo(const std::string& name);
    Json::Value startInfo();
    Json::Value resultInfo(int camp);
    bool canStartBattle();
    void setBirthPos(std::shared_ptr<Player> player, int index);
    void resetPlayers();
    void update();
    int judgeWin();
    static float birthPos[2][3][6];
    int m_id;
    int m_maxPlayers;
    uint64_t m_timeID;
    std::string m_ownerID;
    RoomStatus m_status;
    std::map<std::string, std::shared_ptr<Player> > m_players;
};