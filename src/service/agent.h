#pragma once

#include <service.h>

using namespace Oimo;

class AgentService : public Service {
public:
    void init(Packle::sPtr packle) override;
};