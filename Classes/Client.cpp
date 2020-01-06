//
// Created by dejmian on 05.01.2020.
//

#include <sys/epoll.h>
#include <Utils/utils.h>
#include "Client.h"

Client::Client() {

}

void Client::handleEvent(uint32_t events) {
    if (events & EPOLLIN) {
        char buffer[BUFFER_SIZE];
        int count = readData(fd, buffer, sizeof(buffer));
        if (count > 0)
            writeData(1, buffer, count);
        else
            events |= EPOLLERR;
    }
    if (events & EPOLLOUT) {
        printf("++++++++++++++ UWAGA EPOLLOUT");
    }
    if (events & ~EPOLLIN) {
        printf("++++++++++++++ UWAGA BLAAAAAAAAAAAADDDDDDD CLIENTA");
    }

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
