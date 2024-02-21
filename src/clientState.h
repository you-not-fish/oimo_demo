#pragma once
#include <connection.h>

using namespace Oimo;

struct ClientState {
    using sPtr = std::shared_ptr<ClientState>;
    Net::Connection::sPtr conn;
    time_t lastPingTime;
    uint32_t agent;
};