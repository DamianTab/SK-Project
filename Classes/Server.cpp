//
// Created by dejmian on 05.01.2020.
//

#include "Server.h"
#include <Utils/utils.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

//Sample message
char messageBuffer[] = "Please enter unique login: ";
char successBuffer[] = "Success";

Server::Server(int argc, char **argv) {
    usersMap.clear();
    createServerSocket(argc, argv);
}

void Server::handleEvent(uint32_t events) {
    if(events & EPOLLIN) {
        int new_connection = accept(fd, NULL, NULL);
        printf("New connection noticed with socket: %d \n", new_connection);

        writeData(new_connection, messageBuffer, sizeof(messageBuffer));

        char duzybufor[BUFFER_SIZE];
        int x = read(new_connection, duzybufor, BUFFER_SIZE);
        printf("\n%d Data: %s\n", x, duzybufor);

//        char tempBuffer[BUFFER_SIZE];
//        std::string login;
//        int bytes;
//
//        do {
//            writeData(new_connection, messageBuffer, sizeof(messageBuffer));
//
//            bytes = readData(new_connection, tempBuffer, sizeof(tempBuffer));
//    //        todo usunac linijke nizej
//            writeData(1, tempBuffer, bytes);
//
//            std::string login(tempBuffer);
//        } while (usersMap.find(login) == usersMap.end());
//        writeData(new_connection, successBuffer, sizeof(successBuffer));
//        usersMap.insert(std::pair<std::string, int>(login, new_connection));
//        printf("New login has been registered: %s \n", login.c_str());

    }
    if(events & ~EPOLLIN){
        error(0, errno, "Event %x on server socket", events);
        closeServer();
        exit(0);
    }
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

void Server::closeServer() {
    //todo czysczenie mapy
    close(fd);
}
