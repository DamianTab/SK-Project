//
// Created by dejmian on 05.01.2020.
//

#include "Server.h"
#include <Utils/utils.h>


//Sample message
char buffer[] = "to gang z albanii otwieraj drzwi";

Server::Server() {
    usersMap.clear();
}

void Server::handleEvent(uint32_t events) {
    writeData(1, buffer, sizeof(buffer));

    int new_connection = accept(fd, NULL, NULL);
    printf("New connection noticed with socket: %d \n", new_connection);

//    char tempBuffer[5];
//    int bytes = readData(new_connection, tempBuffer, sizeof(tempBuffer));
//    string login(tempBuffer);

//    if (usersMap.find())
//    server->getUsersMap().find()
    usersMap.insert(std::pair<std::string, int>("login", new_connection));

    printf("To jest w srodku: %d , A tego nie ma: %d \n",usersMap["login"], usersMap["login111"]);

    writeData(new_connection, buffer, sizeof(buffer));

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
