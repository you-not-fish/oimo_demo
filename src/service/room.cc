#include "room.h"
#include <msgType.h>
#include <log.h>
#include <cmath>
#include <player.h>

using namespace Oimo;

float RoomService::birthPos[2][3][6] = {
    //阵营1出生点
	{
		{262.3f, -8.0f, 342.7f, 0, -151.0f, 0},//出生点1
		{229.7f, -5.5f, 354.4f, 0, -164.2f, 0},//出生点2
		{197.1f, -3.6f, 347.7f, 0, -193.0f, 0},//出生点3
	},
	//阵营2出生点
	{
		{-80.3f,  9.5f, 114.6f, 0, -294.0f,  0},//出生点1
		{-91.1f, 15.5f, 139.1f, 0, -294.2f, 0},//出生点2
		{-62.3f,  1.2f, 76.1f,  0, -315.4f, 0},//出生点3
	}
};

RoomService::~RoomService() {
	if (m_timeID != 0) {
		removeTimer(m_timeID);
		m_timeID = 0;
	}
}

void RoomService::init(Packle::sPtr packle) {
	// 初始化房间id
	std::string roomId = std::any_cast<std::string>(packle->userData);
	m_id = std::stoi(roomId);
	m_status = RoomStatus::Preparing;
	m_maxPlayers = 6;
	m_ownerID = "";
	m_timeID = 0;
	// 注册消息处理函数
    registerFunc((Packle::MsgID)MsgType::AddPlayer,
        std::bind(&RoomService::handleAddPlayer, this, std::placeholders::_1)
    );
	registerFunc((Packle::MsgID)MsgType::RemovePlayer,
		std::bind(&RoomService::handleRemovePlayer, this, std::placeholders::_1)
	);
	registerFunc((Packle::MsgID)MsgType::StartBattle,
		std::bind(&RoomService::handleStartBattle, this, std::placeholders::_1)
	);
	registerFunc((Packle::MsgID)MsgType::ReqRoomInfo,
		std::bind(&RoomService::handleReqRoomInfo, this, std::placeholders::_1)
	);
	registerFunc((Packle::MsgID)MsgType::GetRoomInfo,
		std::bind(&RoomService::handleGetRoomInfo, this, std::placeholders::_1)
	);
	registerFunc((Packle::MsgID)MsgType::SyncTank,
		std::bind(&RoomService::handleSyncTank, this, std::placeholders::_1)
	);
	registerFunc((Packle::MsgID)MsgType::Fire,
		std::bind(&RoomService::handleFire, this, std::placeholders::_1)
	);
	registerFunc((Packle::MsgID)MsgType::Hit,
		std::bind(&RoomService::handleHit, this, std::placeholders::_1)
	);
}

void RoomService::handleAddPlayer(Packle::sPtr packle) {
    auto playr = std::any_cast<std::shared_ptr<Player> >(packle->userData);
	bool result = true;
	do {
		// 判断房间是否已满
		if (m_players.size() >= m_maxPlayers) {
			result = false;
			break;
		}
		// 判断玩家是否已经在房间中
		if (m_players.find(playr->name) != m_players.end()) {
			LOG_WARN << "player " << playr->name << " already in room " << m_id << "!";
			result = false;
			break;
		}
		// 只有房间状态为Preparing时才能加入
		if (m_status != RoomStatus::Preparing) {
			LOG_WARN << "player " << playr->name << " can't join room " << m_id << " because room status is not Preparing!";
			result = false;
			break;
		}
		// 将玩家加入房间
		m_players[playr->name] = playr;
		// 设置玩家阵营
		playr->camp = switchCamp();
		// 设置玩家房间id
		playr->roomId = m_id;
		// 设置房主
		if (m_players.size() == 1) {
			m_ownerID = playr->name;
		}
		LOG_DEBUG << "player " << playr->name << " join room " << m_id << "!";
		// 广播
		broadcast(roomInfo());
	} while (0);
	packle->userData = result;
	setReturnPackle(packle);
}

void RoomService::handleRemovePlayer(Packle::sPtr packle) {
	std::string name = std::any_cast<std::string>(packle->userData);
	bool result = true;
	do {
		// 判断玩家是否在房间中
		if (m_players.find(name) == m_players.end()) {
			LOG_WARN << "player " << name << " not in room " << m_id << "!";
			result = false;
			break;
		}
		auto player = m_players[name];
		// 删除玩家
		m_players.erase(name);
		player->roomId = -1;
		player->camp = 0;
		// 战斗状态下退出
		if (m_status == RoomStatus::Fighting) {
			player->data.lose++;
			// 广播退出消息
			broadcast(leaveInfo(player->name));
		}
		// 如果房间为空，删除房间
		if (m_players.empty()) {
			// 删除房间
			Packle::sPtr pack = std::make_shared<Packle>(
				(Packle::MsgID)MsgType::RemoveRoom
			);
			pack->userData = m_id;
			send("roommgr", pack);
			break;
		}
		// 如果房主退出，重新设置房主
		if (m_ownerID == name) {
			m_ownerID = switchOwner();
		}
		// 广播
		broadcast(roomInfo());
	} while (0);
	packle->userData = result;
	setReturnPackle(packle);
}

void RoomService::handleReqRoomInfo(Packle::sPtr packle) {
	Json::Value node;
	node["id"] = m_id;
	node["count"] = m_players.size();
	node["status"] = (int)m_status;
	packle->userData = node;
	setReturnPackle(packle);
}

void RoomService::handleGetRoomInfo(Packle::sPtr packle) {
	packle->userData = roomInfo();
	setReturnPackle(packle);
}

void RoomService::handleSyncTank(Packle::sPtr packle) {
	auto node = std::any_cast<Json::Value>(packle->userData);
	if (m_status != RoomStatus::Fighting) {
		LOG_WARN << "can't sync tank! room id: " << m_id;
		return;
	}
	auto name = node["id"].asString();
	auto it = m_players.find(name);
	if (it == m_players.end()) {
		LOG_WARN << "can't find player " << name << " in room " << m_id;
		return;
	}
	auto player = it->second;
	// 检查作弊
	if (std::abs(player->x - node["x"].asFloat()) > 5.0f ||
		std::abs(player->y - node["y"].asFloat()) > 5.0f ||
		std::abs(player->z - node["z"].asFloat()) > 5.0f) {
		LOG_INFO << "player " << name << " is cheating!";
	}
	player->x = node["x"].asFloat();
	player->y = node["y"].asFloat();
	player->z = node["z"].asFloat();
	player->ex = node["ex"].asFloat();
	player->ey = node["ey"].asFloat();
	player->ez = node["ez"].asFloat();
	broadcast(node);
}

void RoomService::handleFire(Packle::sPtr packle) {
	auto node = std::any_cast<Json::Value>(packle->userData);
	if (m_status != RoomStatus::Fighting) {
		LOG_WARN << "can't fire! room id: " << m_id;
		return;
	}
	auto name = node["id"].asString();
	auto it = m_players.find(name);
	if (it == m_players.end()) {
		LOG_WARN << "can't find player " << name << " in room " << m_id;
		return;
	}
	broadcast(node);
}

void RoomService::handleHit(Packle::sPtr packle) {
	auto node = std::any_cast<Json::Value>(packle->userData);
	if (m_status != RoomStatus::Fighting) {
		LOG_WARN << "can't hit! room id: " << m_id;
		return;
	}
	auto name = node["id"].asString();
	auto it = m_players.find(name);
	if (it == m_players.end()) {
		LOG_WARN << "can't find player " << name << " in room " << m_id;
		return;
	}
	auto player = it->second;
	it = m_players.find(node["targetId"].asString());
	if (it == m_players.end()) {
		LOG_WARN << "can't find target player " << node["targetId"].asString() << " in room " << m_id;
		return;
	}
	auto target = it->second;
	int dmg = 22;
	target->hp -= dmg;
	node["damage"] = dmg;
	node["hp"] = player->hp;
	// LOG_INFO << "hit resp : " << JSONUtils::stringify(node);
	broadcast(node);
}

void RoomService::handleStartBattle(Packle::sPtr packle) {
	auto name = std::any_cast<std::string>(packle->userData);
	if (name != m_ownerID) {
		packle->userData = false;
		setReturnPackle(packle);
		return;
	}
	if (!canStartBattle()) {
		LOG_INFO << "can't start battle! room id: " << m_id;
		packle->userData = false;
		setReturnPackle(packle);
		return;
	}
	m_status = RoomStatus::Fighting;
	resetPlayers();
	// 每10s检查一次游戏是否结束
	m_timeID = addTimer(1000, 10000, std::bind(&RoomService::update, this));
	broadcast(startInfo());
	packle->userData = true;
	setReturnPackle(packle);
}

int RoomService::switchCamp() {
	int camp1 = 0;
	int camp2 = 0;
	for (auto& it : m_players) {
		if (it.second->camp == 1) {
			camp1++;
		}
		else if (it.second->camp == 2) {
			camp2++;
		}
	}
	if (camp1 > camp2) {
		return 2;
	}
	else {
		return 1;
	}
}

std::string RoomService::switchOwner() {
	std::string owner;
	for (auto& it : m_players) {
		owner = it.first;
		break;
	}
	return owner;
}


void RoomService::broadcast(Json::Value node) {
	for (auto& it : m_players) {
		Packle::sPtr pack = std::make_shared<Packle>(
			(Packle::MsgID)MsgType::Resp
		);
		pack->userData = node;
		pack->setFd(it.second->fd);
		send("gateway", pack);
	}
}

Json::Value RoomService::roomInfo() {
	Json::Value node;
	node["protoName"] = "MsgGetRoomInfo";
	node["players"] = Json::Value(Json::arrayValue);
	for (auto& it : m_players) {
		Json::Value player;
		player["id"] = it.second->name;
		player["camp"] = it.second->camp;
		player["win"] = it.second->data.win;
		player["lost"] = it.second->data.lose;
		player["isOwner"] = 0;
		if (m_ownerID == it.second->name) {
			player["isOwner"] = 1;
		}
		node["players"].append(player);
	}
	LOG_DEBUG << "roomInfo: " << JSONUtils::stringify(node);
	return node;
}

Json::Value RoomService::leaveInfo(const std::string& name) {
	Json::Value node;
	node["protoName"] = "MsgLeaveBattle";
	node["id"] = name;
	LOG_DEBUG << "leaveInfo: " << JSONUtils::stringify(node);
	return node;
}

Json::Value RoomService::startInfo() {
	Json::Value node;
	node["protoName"] = "MsgEnterBattle";
	node["mapId"] = 1;
	node["tanks"] = Json::Value(Json::arrayValue);
	for (auto& it : m_players) {
		Json::Value tank;
		tank["camp"] = it.second->camp;
		tank["id"] = it.second->name;
		tank["hp"] = it.second->hp;
		tank["x"] = it.second->x;
		tank["y"] = it.second->y;
		tank["z"] = it.second->z;
		tank["ex"] = it.second->ex;
		tank["ey"] = it.second->ey;
		tank["ez"] = it.second->ez;
		node["tanks"].append(tank);
	}
	LOG_DEBUG << "startInfo: " << JSONUtils::stringify(node);
	return node;
}

Json::Value RoomService::resultInfo(int camp) {
	Json::Value node;
	node["protoName"] = "MsgBattleResult";
	node["winCamp"] = camp;
	LOG_DEBUG << "resultInfo: " << JSONUtils::stringify(node);
	return node;
}

bool RoomService::canStartBattle() {
	if (m_status != RoomStatus::Preparing) {
		return false;
	}
	if (m_players.size() < 2) {
		return false;
	}
	return true;
}

void RoomService::setBirthPos(std::shared_ptr<Player> player, int index) {
	int camp = player->camp;
	player->x = birthPos[camp - 1][index][0];
	player->y = birthPos[camp - 1][index][1];
	player->z = birthPos[camp - 1][index][2];
	player->ex = birthPos[camp - 1][index][3];
	player->ey = birthPos[camp - 1][index][4];
	player->ez = birthPos[camp - 1][index][5];
}

void RoomService::resetPlayers() {
	int count1 = 0;
	int count2 = 0;
	for (auto& it : m_players) {
		it.second->hp = 100;
		if (it.second->camp == 1) {
			setBirthPos(it.second, count1);
			count1++;
		}
		else {
			setBirthPos(it.second, count2);
			count2++;
		}
	}
}

void RoomService::update() {
	if (m_status != RoomStatus::Fighting) {
		return;
	}
	int win = judgeWin();
	if (win != 0) {
		if (m_timeID != 0) {
			removeTimer(m_timeID);
			m_timeID = 0;
		}
		m_status = RoomStatus::Preparing;
		for (auto& it : m_players) {
			if (it.second->camp == win) {
				it.second->data.win++;
			} else {
				it.second->data.lose++;
			}
		}
		broadcast(resultInfo(win));
	}
}

int RoomService::judgeWin() {
	int camp1 = 0;
	int camp2 = 0;
	for (auto& it : m_players) {
		if (it.second->hp > 0) {
			if (it.second->camp == 1) {
				camp1++;
			}
			else {
				camp2++;
			}
		}
	}
	if (camp1 == 0) {
		return 2;
	}
	if (camp2 == 0) {
		return 1;
	}
	return 0;
}