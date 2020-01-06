//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_SOCKETHANDLER_H
#define SK_PROJECT_SOCKETHANDLER_H


#include <cstdint>
#include <netinet/in.h>

class SocketHandler {
protected:
    static int epollFd;
public:
    int fd;
    sockaddr_in sockAddr;
    virtual void handleEvent (uint32_t events) = 0;
    virtual ~SocketHandler() {}

    int getEpollFd(){
        return epollFd;
    }

    void setEpollFd(int _epollFd) {
        epollFd = _epollFd;
    }
};


#endif //SK_PROJECT_SOCKETHANDLER_H
