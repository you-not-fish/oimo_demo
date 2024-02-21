#pragma once
#include <service.h>

using namespace Oimo;

class LoginService : public Service {
public:
    void init(Packle::sPtr packle) override;
private:
    void handleRegister(Packle::sPtr packle);
    void handleLogin(Packle::sPtr packle);
};