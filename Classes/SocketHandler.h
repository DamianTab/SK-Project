//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_SOCKETHANDLER_H
#define SK_PROJECT_SOCKETHANDLER_H


#include <cstdint>
#include <netinet/in.h>

class SocketHandler {
public:
    int fd;
    sockaddr_in sockAddr;
    virtual void handleEvent (uint32_t events) = 0;
    virtual ~SocketHandler() {}
};


#endif //SK_PROJECT_SOCKETHANDLER_H
