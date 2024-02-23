#include <application.h>
#include <service/gateway.h>
#include <service/agentMgr.h>
#include <service/roomMgr.h>
#include <service/login.h>

using namespace Oimo;

int main(int argc, char** argv) {
    auto& app = APP::instance();
    app.init();
    Logger::setLogLevel(LogLevel::DEBUG);
    app.newService<RoomMgr>("roommgr");
    app.newService<AgentMgr>("agentmgr");
    app.newService<LoginService>("login");
    app.newService<GatewayService>("gateway");
    return app.run();
}