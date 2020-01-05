#include <sys/socket.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <error.h>
#include <sys/epoll.h>
#include <Classes/Server.h>
using namespace std;

//Server class
Server* server = new Server;

//Functions
void createServerSocket(int argc, char **argv);
void closeServer();
void handleEpollEvents();


int main(int argc, char **argv) {
    createServerSocket(argc, argv);

    int res = listen(server->fd, SOMAXCONN);
    if(res)
        error(1, errno, "Failed to execute 'listen'\n");

    server->setEpollFd(epoll_create1(0));
    epoll_event ee {EPOLLIN, {.ptr=server}};
    epoll_ctl(server->getEpollFd(), EPOLL_CTL_ADD, server->fd, &ee);

    handleEpollEvents();

    closeServer();
    return 0;
}

void createServerSocket(int argc, char **argv){
    if (argc != 3)
        error(1, 0, "To correctly run type: %s <IP adress> <port>\n", argv[0]);
    server->sockAddr={
            .sin_family = AF_INET,
            .sin_port   = htons(atoi(argv[2])),
            .sin_addr   = {inet_addr(argv[1])}
    };
    server->fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server->fd == -1)
        error(1, errno, "Failed to create server socket\n");

//    Only allows to faster use the same port - can be deleted later
    const int one = 1;
    setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    if(bind(server->fd, (sockaddr*) &server->sockAddr, sizeof(server->sockAddr)))
        error(1,errno,"Failed to bind server address!\n");
}

void handleEpollEvents(){
    epoll_event ee;
    while(1) {
        if (-1 == epoll_wait(server->getEpollFd(), &ee, 1, -1)) {
            error(1, errno, "Function epoll_wait failed\n");
            closeServer();
            exit(0);
        }
        if (ee.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
            error(1, errno, "Internal server error. Please contact developers ASAP!\n");
            closeServer();
            exit(0);
        }
        if (ee.events & EPOLLIN) {
            ((SocketHandler*)ee.data.ptr)->handleEvent(ee.events);
        }
    }

}

void closeServer(){
    close(server->fd);
}

