//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_SERVER_H
#define SK_PROJECT_SERVER_H

#include <string>
#include <map>
#include "SocketHandler.h"

class Server: public SocketHandler {
private:
    // server socket
    int serverFd;
    // Epoll socket
    int epollFd;
    // Map with user login and socket (FD)
    std::map<std::string, int> usersMap;
public:
    Server();
    void handleEvent (uint32_t events) override;
};


#endif //SK_PROJECT_SERVER_H
