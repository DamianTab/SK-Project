//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_CLIENT_H
#define SK_PROJECT_CLIENT_H


#include "SocketHandler.h"
#include <string>

class Client: public SocketHandler {
private:
    std::string login;
    int score = 0;


public:
    void handleEvent (uint32_t events) override;
};


#endif //SK_PROJECT_CLIENT_H
