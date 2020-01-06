//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_SERVER_H
#define SK_PROJECT_SERVER_H

#include <string>
#include <map>
#include "SocketHandler.h"
#include "Client.h"

class Server : public SocketHandler {
private:
    // Epoll socket
    int epollFd;
    // Map with user login and socket (FD)
    std::map<std::string, int> usersMap;

    void createServerSocket(int argc, char **argv);

public:
    Server(int argc, char **argv);

    void handleEvent(uint32_t events) override;

    void closeServer();

    void addClient(Client client);

    void deleteClient(std::string login);

    // Getters and setters
    std::map<std::string, int> &getUsersMap();

    void setUsersMap(std::map<std::string, int> &usersMap);

    int getEpollFd() const;

    void setEpollFd(int epollFd);
};


#endif //SK_PROJECT_SERVER_H
