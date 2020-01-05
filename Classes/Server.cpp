//
// Created by dejmian on 05.01.2020.
//

#include "Server.h"


Server::Server() {
    usersMap.clear();
}

void Server::handleEvent(uint32_t events) {
    //todo implement

}

int Server::getEpollFd() const {
    return epollFd;
}

void Server::setEpollFd(int epollFd) {
    Server::epollFd = epollFd;
}

std::map<std::string, int> &Server::getUsersMap() {
    return usersMap;
}

void Server::setUsersMap(std::map<std::string, int> &usersMap) {
    Server::usersMap = usersMap;
}

void Server::addClient(Client client) {
    //todo implement

}

void Server::deleteClient(std::string login) {
    //todo implement

}
