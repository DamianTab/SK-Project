//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_CLIENT_H
#define SK_PROJECT_CLIENT_H


#include "SocketHandler.h"
#include <string>
#include <set>
#include <vector>
#include <unistd.h>
#include <Utils/utils.h>


class Client : public SocketHandler {
private:
    std::string login;
    float totalScore = 0;
public:
//    BECAUSE OF ROUND NUMBER - DO NOT NEED TO PLACE MUTEX ON THIS VARIABLE
    int inactiveRoundsNumber = 0;
//    BECAUSE OF ROUND NUMBER - DO NOT NEED TO PLACE MUTEX ON THIS VARIABLE
    std::vector<std::string> lastAnswers ;
//    BECAUSE OF ROUND NUMBER - DO NOT NEED TO PLACE MUTEX ON THIS VARIABLE
    std::vector<float> lastScore;

    Client(std::string _login, int _fd);

    ~Client() override;

    void handleEvent(uint32_t events) override;

    void recalculateTotalScore();

    void sendAnswersAndPoints();

    std::string getFormattedAnswers();

    // Getters and setters
    std::basic_string<char> getLogin();

    void setLogin(std::string &login);

    float getScore();

    void setScore(float score);

    void sendMessage(std::string message);
};


#endif //SK_PROJECT_CLIENT_H
