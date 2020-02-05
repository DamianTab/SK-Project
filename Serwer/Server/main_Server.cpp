#include <sys/socket.h>
#include <string>
#include <error.h>
#include <sys/epoll.h>
#include <Classes/Server.h>
#include <csignal>
#include <Classes/Game.h>
#include <fstream>
#include <iostream>

Server *server;

bool startsWith(std::string str, std::string matches){

    for(int i = 0; i < matches.length() || i < str.length(); i++){
        if(str[0] != matches[0]) return false;
    }
    return true;
}

void handleEpollEvents(Server *server);

extern void ctrl_c(int);

int main(int argc, char **argv) {
    if (argc != 3)
        error(1, 0, "To correctly run type: %s <IP adress> <port>\n", argv[0]);

    int v1 = 2;
    int v2 = 40;
    double v3 = 0.5f;
    // load data from file
    std::ifstream config;
    std::string line;
    config.open("config.txt");
    if(config.good()) {

        std::cout << "Loading config..." << std::endl;
        while (std::getline(config, line)) {
            std::string value = "";
            for (int i = 12; i < line.length(); i++) {
                value += line[i];
            }
            if (startsWith(line, "min_player:")) {
                v1 = stoi(value);
            } else if (startsWith(line, "round_time")) {
                v2 = stoi(value);
            }
            else if(startsWith(line, "sleep_time")){
                v3 = stof(value);
            }
        }
        std::cout << v1 << ", " << v2 << ", " << v3 << std::endl;
        readConfig(v1, v3, v2);
        config.close();
    }

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

