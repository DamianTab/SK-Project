//
// Created by dejmian on 05.01.2020.
//

#include <sys/epoll.h>
#include <Utils/utils.h>
#include "Client.h"
#include "Server.h"
#include "Game.h"

Client::Client(std::string _login, int _fd) {
    fd = _fd;
    login = _login;
    epoll_event ee{EPOLLIN | EPOLLRDHUP, {.ptr=this}};
    epoll_ctl(Client::epollFd, EPOLL_CTL_ADD, _fd, &ee);
}

Client::~Client() {
    Server::deleteClientFromMap(login);
    epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

void Client::handleEvent(uint32_t events) {
    int roundValue = 0;
    if (events & ~EPOLLIN) {
        error(0, errno, "Event %#04x on client socket %d with login: '%s'. Disconnect client ...", events, fd,
              login.c_str());
        delete (this);
    } else if (events & EPOLLIN) {
        char buffer[BUFFER_SIZE];
        int count = readData(fd, buffer, &roundValue);
        if (count > 0) {
            if (!isCorrectRound(Game::getRound(), roundValue)) {
                error(0, errno, "Aborting invalid message from client '%s' with fd %d !\n", login.c_str(), fd);
            }
            printf("JAKAS WIADOMOSC PO DOLACZENIU: %s \n", buffer);
        }
        printf("koniec handle event CLIENT\n");
    }
}

// Getters and setters
std::basic_string<char> Client::getLogin() {
    return login;
}

void Client::setLogin(std::string &login) {
    Client::login = login;
}

int Client::getScore() {
    return totalScore;
}

void Client::setScore(int score) {
    Client::totalScore = score;
}