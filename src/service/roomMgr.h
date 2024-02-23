#pragma once

#include <service.h>
#include <map>

using namespace Oimo;

class RoomMgr : public Service {
public:
    void init(Packle::sPtr packle) override;
private:
    void handleCreateRoom(Packle::sPtr packle);
    void handleRemoveRoom(Packle::sPtr packle);
    void handleGetRoom(Packle::sPtr packle);
    void handleGetRoomList(Packle::sPtr packle);
    std::map<int, uint32_t> m_rooms;
};