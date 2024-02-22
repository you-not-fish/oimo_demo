#pragma once
#include <map>
#include <service.h>
#include <tcpServer.h>
#include <clientState.h>

using namespace Oimo;

#define MAX_TYPE_LENGTH 32
#define MAX_MSG_LENGTH 1024

class GatewayService : public Service {
public:
    void init(Packle::sPtr packle) override;
private:
    void onConnect(Net::Connection::sPtr conn);
    bool checkLens(int msgLen, int typeLen);
    void forward(Packle::sPtr packle, uint32_t agent);
    void handleMsg(const char *msg, ClientState::sPtr state);
    void handleResp(Packle::sPtr packle);
    void handleUpdaeAgent(Packle::sPtr packle);
    void close(ClientState::sPtr state);
    static const int kPingInterval;
    std::map<int, ClientState::sPtr> m_clients;
    Net::TcpServer m_server;
};
