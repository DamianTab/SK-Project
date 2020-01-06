//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_CLIENT_H
#define SK_PROJECT_CLIENT_H


#include "SocketHandler.h"
#include <string>

class Client : public SocketHandler {
private:
    std::string login;
    int score = 0;


public:
    Client();

    void handleEvent(uint32_t events) override;

    // Getters and setters
    std::string &getLogin();

    void setLogin(std::string &login);

    int getScore();

    void setScore(int score);
};


#endif //SK_PROJECT_CLIENT_H
