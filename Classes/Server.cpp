//
// Created by dejmian on 05.01.2020.
//

#include "Server.h"
#include "Game.h"
#include <Utils/utils.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>

std::map<std::string, Client *> Server::usersMap;

char welcomeMessage[] = "Welcome in Country-Capitals Game !!!\n";
char loginMessage[] = "Please enter unique login: \n";
char successMessage[] = "Success";

Server::Server(int argc, char **argv) {
    usersMap.clear();
    createServerSocket(argc, argv);
}

Server::~Server() {
    auto it = usersMap.begin();
    while (it != usersMap.end()) {
        std::pair<std::string, Client *> pair = *it;
        delete (pair.second);
        it++;
    }
    usersMap.clear();
}

void Server::handleEvent(uint32_t events) {
    int roundValue = 0;

    if (events & EPOLLIN) {
        printf("----------------------------------------------------\n");
        int new_connection = accept(fd, NULL, NULL);
        printf("New connection noticed with socket: %d \n", new_connection);

        writeData(new_connection, welcomeMessage, CONNECTION_ROUND_VALUE);
        char receiveBuffer[BUFFER_SIZE];
        std::string login;

        do {
            writeData(new_connection, loginMessage, CONNECTION_ROUND_VALUE);
            int bytes = readData(new_connection, receiveBuffer, &roundValue);
            login = std::string(receiveBuffer);
            login = login.substr(0,bytes);
        } while (usersMap.find(login) != usersMap.end());

        Client *client = new Client(login, new_connection);
        addClientToMap(client);
        printf("New login has been registered: '%s' \n", login.c_str());
        writeData(new_connection, successMessage, CONNECTION_ROUND_VALUE);

//        If condition are true then starts new thread and the game begins
        if (Game::getRound() == 0 && usersMap.size() >=2){
            new Game();
        }
    }
    if (events & ~EPOLLIN) {
        error(0, errno, "Event %#0x on server socket", events);
        closeServer();
        exit(0);
    }
}


void Server::sendToAllClients(char *buffer) {
    for (const auto &kv : Server::getUsersMap()) {
        kv.second->lastAnswers.clear();
        kv.second->lastScore.clear();
        kv.second->setScore(0);
    }
}


void Server::addClientToMap(Client *client) {
    usersMap.insert(std::pair<std::string, Client *>(client->getLogin(), client));
}

void Server::deleteClientFromMap(std::string login) {
    usersMap.erase(login);
}

void Server::closeServer() {
    close(fd);
    delete this;
}

//private
void Server::createServerSocket(int argc, char **argv) {
    sockAddr = {
            .sin_family = AF_INET,
            .sin_port   = htons(atoi(argv[2])),
            .sin_addr   = {inet_addr(argv[1])}
    };
    fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1)
        error(1, errno, "Failed to create server socket\n");

//    Only allows to faster use the same port - can be deleted later
    const int one = 1;
    int res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if (res) error(1, errno, "setsockopt failed");

    if (bind(fd, (sockaddr *) &sockAddr, sizeof(sockAddr)))
        error(1, errno, "Failed to bind server address!\n");
}


// Getters and setters
std::map<std::string, Client *> &Server::getUsersMap() {
    return usersMap;
}

void Server::setUsersMap(std::map<std::string, Client *> &usersMap) {
    Server::usersMap = usersMap;
}
