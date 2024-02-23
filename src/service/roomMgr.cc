#include "roomMgr.h"
#include <msgType.h>
#include <service/room.h>
#include <application.h>

using namespace Oimo;

void RoomMgr::init(Packle::sPtr packle) {
    registerFunc((Packle::MsgID)MsgType::CreateRoom,
        std::bind(&RoomMgr::handleCreateRoom, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::RemoveRoom,
        std::bind(&RoomMgr::handleRemoveRoom, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::GetRoom,
        std::bind(&RoomMgr::handleGetRoom, this, std::placeholders::_1));
    registerFunc((Packle::MsgID)MsgType::GetRoomList,
        std::bind(&RoomMgr::handleGetRoomList, this, std::placeholders::_1));
}

void RoomMgr::handleCreateRoom(Packle::sPtr packle) {
    static int roomID = 0;
    std::string id = std::to_string(++roomID);
    auto room = APP::instance().newService<RoomService>("room" + id, id);
    m_rooms[roomID] = room;
    packle->userData = room;
    setReturnPackle(packle);
}

void RoomMgr::handleRemoveRoom(Packle::sPtr packle) {
    int id = std::any_cast<int>(packle->userData);
    auto it = m_rooms.find(id);
    if (it != m_rooms.end()) {
        APP::instance().removeService(it->second);
        m_rooms.erase(it);
    }
    packle->userData = 0;
    setReturnPackle(packle);
}

void RoomMgr::handleGetRoom(Packle::sPtr packle) {
    int id = std::any_cast<int>(packle->userData);
    auto it = m_rooms.find(id);
    if (it != m_rooms.end()) {
        packle->userData = it->second;
    } else {
        packle->userData = 0;
    }
    setReturnPackle(packle);
}

void RoomMgr::handleGetRoomList(Packle::sPtr packle) {
    Json::Value root;
    root["protoName"] = "MsgGetRoomList";
    root["rooms"] = Json::Value(Json::arrayValue);
    for (auto it : m_rooms) {
        // 查询房间信息
        Packle::sPtr pack = std::make_shared<Packle>(
            (Packle::MsgID)MsgType::ReqRoomInfo
        );
        call(it.second, pack);
        auto roomInfo = std::any_cast<Json::Value>(pack->userData);
        root["rooms"].append(roomInfo);
    }
    LOG_DEBUG << "room list: " << JSONUtils::stringify(root);
    packle->userData = root;
    setReturnPackle(packle);
}

