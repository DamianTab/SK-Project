//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_CLIENT_H
#define SK_PROJECT_CLIENT_H


#include "SocketHandler.h"
#include <string>
#include <set>
#include <vector>
#include <Utils/utils.h>


class Client : public SocketHandler {
private:
    std::string login;
    int totalScore = 0;
public:
    Client(std::string _login, int _fd);

    ~Client() override;

    void handleEvent(uint32_t events) override;

    std::vector<std::string> lastAnswers ;
    std::vector<int> lastScore;

    // Getters and setters
    std::basic_string<char> getLogin();

    void setLogin(std::string &login);

    int getScore();

    void setScore(int score);
};


#endif //SK_PROJECT_CLIENT_H
