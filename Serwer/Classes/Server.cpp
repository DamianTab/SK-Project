//
// Created by dejmian on 05.01.2020.
//

#include "Server.h"
#include "Game.h"
#include <Utils/utils.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>

std::map<std::string, Client *> Server::clientsMap;

char welcomeMessage[] = "Welcome in Country-Capitals Game !!!\n";
char loginMessage[] = "Please enter unique login: \n";
char successMessage[] = "Success";

Server::Server(int argc, char **argv) {
    clearClientsMap();
    createServerSocket(argc, argv);
}

Server::~Server() {
    clearClientsMap();
}

void Server::handleEvent(uint32_t events) {
    int roundValue = 0;

    if (events & EPOLLIN) {
        printf("----------------------------------------------------\n");
        int new_connection = accept(fd, NULL, NULL);
        printf("New connection noticed with socket: %d \n", new_connection);

//        writeData(new_connection, welcomeMessage, CONNECTION_ROUND_VALUE);
        char receiveBuffer[BUFFER_SIZE];
        std::string login;

        do {
            writeData(new_connection, loginMessage, CONNECTION_ROUND_VALUE);
            int bytes = readData(new_connection, receiveBuffer, &roundValue);
            login = std::string(receiveBuffer);
            login = login.substr(0, bytes);
        } while (isInsideClientMap(login));

        mutexClientsMap.lock();
        Client *client = new Client(login, new_connection);
        addClientToMap(client);
        printf("New login has been registered: '%s' \n", login.c_str());
        writeData(new_connection, successMessage, CONNECTION_ROUND_VALUE);

//        If condition are true then starts new thread and the game begins
        if (Game::getRound() == 0 && (int) getClientsMap().size() >= minPlayersNumber) {
            mutexClientsMap.unlock();
            new Game();
        } else{
            mutexClientsMap.unlock();
        }
    }
    if (events & ~EPOLLIN) {
        error(0, errno, "Event %#0x on server socket", events);
        closeServer();
        exit(0);
    }
}


void Server::sendToAllClients(char *buffer) {
    std::scoped_lock lock{mutexClientsMap};
    for (const auto &kv : clientsMap) {
        writeData(kv.second->fd, buffer, Game::getRound(), false);
    }
    sleep(sleepWriteToAll);
}


void Server::addClientToMap(Client *client) {
//    Mutex outside function
    clientsMap.insert(std::pair<std::string, Client *>(client->getLogin(), client));
}

void Server::deleteClientFromMap(std::string login) {
//    Mutex outside function
    clientsMap.erase(login);
}

bool Server::isInsideClientMap(std::string login) {
//    Mutex outside function (except server-handler)
    return clientsMap.find(login) != clientsMap.end();
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

void Server::clearClientsMap() {
    std::scoped_lock lock{mutexClientsMap};
    auto it = clientsMap.begin();
    while (it != clientsMap.end()) {
        std::pair<std::string, Client *> pair = *it;
        delete (pair.second);
        it++;
    }
    clientsMap.clear();
}


// Getters and setters
std::map<std::string, Client *> &Server::getClientsMap() {
//    Mutex outside function
    return clientsMap;
}

void Server::setClientsMap(std::map<std::string, Client *> &usersMap) {
    std::scoped_lock lock{mutexClientsMap};
    Server::clientsMap = usersMap;
}
