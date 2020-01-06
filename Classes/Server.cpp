//
// Created by dejmian on 05.01.2020.
//

#include "Server.h"
#include <Utils/utils.h>

//Sample message
char messageBuffer[] = "Please enter unique login: ";
char successBuffer[] = "Success";

Server::Server() {
    usersMap.clear();
}

void Server::handleEvent(uint32_t events) {
    int new_connection = accept(fd, NULL, NULL);
    printf("New connection noticed with socket: %d \n", new_connection);

//    char tempBuffer[BUFFER_SIZE];
//    std::string login;
//    int bytes;
//
//    do {
//        writeData(new_connection, messageBuffer, sizeof(messageBuffer));
//
//        bytes = readData(new_connection, tempBuffer, sizeof(tempBuffer));
////        todo usunac linijke nizej
//        writeData(1, tempBuffer, bytes);
//
//        std::string login(tempBuffer);
//    } while (usersMap.find(login) == usersMap.end());
//    writeData(new_connection, successBuffer, sizeof(successBuffer));
//    usersMap.insert(std::pair<std::string, int>(login, new_connection));
//    printf("New login has been registered: %s \n", login.c_str());


    writeData(new_connection, messageBuffer, sizeof(messageBuffer));
}


void Server::addClient(Client client) {
    //todo implement

}

void Server::deleteClient(std::string login) {
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
