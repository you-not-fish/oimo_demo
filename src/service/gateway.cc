#include <log.h>
#include <msgType.h>
#include "gateway.h"

using namespace Oimo;

void GatewayService::init(Packle::sPtr packle) {
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
        char* body = new char[bodyLen + 1];
        if (conn->recvN(body, bodyLen) != bodyLen) {
            delete[] body;
            break;
        }
        body[bodyLen] = '\0';
        LOG_DEBUG << "body: " << body;
        // 将消息打包成packle
        Packle::sPtr packle = std::make_shared<Packle>(
            (Packle::MsgID)toMsgType(type, typeLen)
        );
        packle->setSize(bodyLen);
        packle->setBuf(body);
        packle->setFd(conn->fd());
        // 将packle发送到logic服务或者agent服务
        sendToService(packle, state->agent);
        // 不用手动释放body的内存，packle在销毁时会自动释放
    }
    // 连接断开
    m_clients.erase(conn->fd());
    if (!conn->isClosing()) {
        // 关闭连接
        conn->close();
    }
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

void GatewayService::sendToService(Packle::sPtr packle, uint32_t agent) {
    if (agent == 0) {
        // 尚未登陆，发送到logic服务
        call("login", packle);
        // 得到响应结果
        auto resp = responsePackle();
        // 将结果发送给客户端
        auto conn = m_clients[packle->fd()]->conn;
        conn->send(packle);
    } else {
        // 发送到agent服务
        send(agent, packle);
    }
    
}