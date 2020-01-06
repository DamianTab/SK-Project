#include <sys/socket.h>
#include <string>
#include <error.h>
#include <sys/epoll.h>
#include <Classes/Server.h>

using namespace std;

//Functions
void handleEpollEvents(Server *server);

int main(int argc, char **argv) {
    if (argc != 3)
        error(1, 0, "To correctly run type: %s <IP adress> <port>\n", argv[0]);

    Server *server = new Server(argc, argv);

    int res = listen(server->fd, SOMAXCONN);
    if (res)
        error(1, errno, "Failed to execute 'listen'\n");

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
