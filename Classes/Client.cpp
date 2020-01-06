//
// Created by dejmian on 05.01.2020.
//

#include "Client.h"

void Client::handleEvent(uint32_t events) {
    //todo implement
}

// Getters and setters
std::string &Client::getLogin() {
    return login;
}

void Client::setLogin(std::string &login) {
    Client::login = login;
}

int Client::getScore() {
    return score;
}

void Client::setScore(int score) {
    Client::score = score;
}
