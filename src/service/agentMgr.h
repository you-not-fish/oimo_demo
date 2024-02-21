#pragma once

#include <service.h>
#include <string>
#include <map>

using namespace Oimo;

class AgentMgr : public Service {
public:
    void init(Packle::sPtr packle) override;
private:
};