#include <sys/socket.h>
#include <string>
#include <error.h>
#include <sys/epoll.h>
#include <Classes/Server.h>
#include <csignal>
#include <Classes/Game.h>

using namespace std;

Server *server;


void handleEpollEvents(Server *server);

extern void ctrl_c(int);

int main(int argc, char **argv) {
    if (argc != 3)
        error(1, 0, "To correctly run type: %s <IP adress> <port>\n", argv[0]);

    server = new Server(argc, argv);

    int res = listen(server->fd, SOMAXCONN);
    if (res)
        error(1, errno, "Failed to execute 'listen'\n");

    // graceful ctrl+c exit
    signal(SIGINT, ctrl_c);
    // prevent dead sockets from throwing pipe errors on write
    signal(SIGPIPE, SIG_IGN);

    server->setEpollFd(epoll_create1(0));
    epoll_event ee{EPOLLIN, {.ptr=server}};
    epoll_ctl(server->getEpollFd(), EPOLL_CTL_ADD, server->fd, &ee);

    handleEpollEvents(server);

    server->closeServer();
    return 0;
}

void handleEpollEvents(Server *server) {
    epoll_event ee;
    while (1) {
        if (-1 == epoll_wait(server->getEpollFd(), &ee, 1, -1)) {
            error(1, errno, "Function epoll_wait failed\n");
            server->closeServer();
            exit(0);
        }
        ((SocketHandler *) ee.data.ptr)->handleEvent(ee.events);
    }

}


void ctrl_c(int) {
    delete Game::gameInstance;
    server->closeServer();
    printf("\nClosing server - caused by ctrl_c\n");
    exit(0);
}

