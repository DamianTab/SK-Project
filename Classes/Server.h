//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_SERVER_H
#define SK_PROJECT_SERVER_H

#include <string>
#include <map>
#include "SocketHandler.h"
#include "Client.h"

class Server: public SocketHandler {
private:
    // Epoll socket
    int epollFd;
    // Map with user login and socket (FD)
    std::map<std::string, int> usersMap;

public:
    Server();
    void handleEvent (uint32_t events) override;

    void addClient(Client client);

    void deleteClient(std::string login);

    std::map<std::string, int> &getUsersMap();

    void setUsersMap(std::map<std::string, int> &usersMap);

    int getEpollFd() const;

    void setEpollFd(int epollFd);
};


#endif //SK_PROJECT_SERVER_H
