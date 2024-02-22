#include <log.h>
#include <msgType.h>
#include <JSONUtils.h>
#include "gateway.h"

using namespace Oimo;

void GatewayService::init(Packle::sPtr packle) {
    // 注册消息处理
    registerFunc(toInt(MsgType::UpdateAgent),
        std::bind(&GatewayService::handleUpdaeAgent, this, std::placeholders::_1)
    );
    registerFunc(toInt(MsgType::Resp),
        std::bind(&GatewayService::handleResp, this, std::placeholders::_1)
    );
    // 初始化tcp服务
    m_server.init(this);
    // 创建socket并绑定监听
    m_server.createFd("0.0.0.0", 9527);
    // 开始处理连接，当新连接到来时会调用onConnect方法
    m_server.start(
        std::bind(&GatewayService::onConnect, this,
        std::placeholders::_1)
    );
}

void GatewayService::onConnect(Net::Connection::sPtr conn) {
    auto state = std::make_shared<ClientState>();
    state->conn = conn;
    state->lastPingTime = time(nullptr);
    state->agent = 0;
    m_clients[conn->fd()] = state;
    // 开始接受数据
    conn->start();
    char lens[4];
    char type[MAX_TYPE_LENGTH];
    char body[MAX_MSG_LENGTH];
    int msgLen, typeLen, bodyLen;
    while (!conn->isClosing()) {
        // 消息长度（2字节）+ 类型长度（2字节）+ 类型 + 数据
        // 读取消息长度和类型长度
        if (conn->recvN(lens, 4) != 4) {
            break;
        }
        msgLen = (lens[1] << 8) | lens[0];
        typeLen = (lens[3] << 8) | lens[2];
        bodyLen = msgLen - typeLen - 2;
        if (!checkLens(msgLen, typeLen)) {
            break;
        }
        // 读取消息类型
        if (conn->recvN(type, typeLen) != typeLen) {
            break;
        }
        LOG_DEBUG << "type: " << std::string(type, typeLen);
        // 读取消息体
        if (conn->recvN(body, bodyLen) != bodyLen) {
            break;
        }
        body[bodyLen] = '\0';
        LOG_DEBUG << "body: " << body;
        // 处理消息
        handleMsg(body, state);
    }
    // 关闭连接
    close(state);
}

bool GatewayService::checkLens(int msgLen, int typeLen) {
    if (msgLen > MAX_MSG_LENGTH || typeLen > MAX_TYPE_LENGTH) {
        LOG_ERROR << "message too long, msgLen: " << msgLen
            << ", typeLen: " << typeLen;
        return false;
    }
    if (msgLen < typeLen) {
        LOG_ERROR << "msgLen should be greater than typeLen, msgLen: "
            << msgLen << ", typeLen: " << typeLen;
        return false;
    }
    return true;
}

void GatewayService::handleMsg(const char * buf, ClientState::sPtr state) {
    auto conn = state->conn;
    // 反序列化
    Json::Value root;
    if (!JSONUtils::parse(buf, root)) {
        LOG_ERROR << "parse json failed. fd: " << conn->fd();
        close(state);
        return;
    }
    // 心跳包
    std::string protoName = root["protoName"].asString();
    if (protoName == "MsgPing") {
        state->lastPingTime = time(nullptr);
        return;
    }
    // 打包成Packle
    auto packle = std::make_shared<Packle>();
    packle->setFd(conn->fd());
    packle->setType(
        static_cast<Packle::MsgID>(toMsgType(protoName))
    );
    packle->userData = root;
    // 转发到其他服务
    forward(packle, state->agent);
}

void GatewayService::forward(Packle::sPtr packle, uint32_t agent) {
    if (agent == 0) {
        // 尚未登陆，发送到logic服务
        send("login", packle);
    } else {
        // 发送到agent服务
        send(agent, packle);
    }
}
    
void GatewayService::handleResp(Packle::sPtr packle) {
    auto node = std::any_cast<Json::Value>(packle->userData);
    auto conn = m_clients[packle->fd()]->conn;
    // 序列化
    int len;
    char *bytes = JSONUtils::serialize(node, len);
    packle->getAndResetBuf(bytes, len);
    // 发送
    conn->send(packle);
}

void GatewayService::handleUpdaeAgent(Packle::sPtr packle) {
    auto agent = std::any_cast<uint32_t>(packle->userData);
    auto state = m_clients[packle->fd()];
    state->agent = agent;
    if (agent == 0) {
        // 向客户端发送MsgKick消息
        Packle::sPtr pack = std::make_shared<Packle>(
            toInt(MsgType::MsgKick)
        );
        Json::Value node;
        node["protoName"] = "MsgKick";
        node["reason"] = 0;
        pack->userData = node;
        pack->setFd(state->conn->fd());
        handleResp(pack);
    }
    setReturnPackle(packle);
}

void GatewayService::close(ClientState::sPtr state) {
    auto conn = state->conn;
    m_clients.erase(conn->fd());
    if (state->agent != 0) {
        // 通知AgentMgr服务
        Packle::sPtr pack = std::make_shared<Packle>(
            toInt(MsgType::ReqKick)
        );
        pack->userData = state->agent;
        call("AgentMgr", pack);
    }
    conn->close();
}