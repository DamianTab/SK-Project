#include <sys/socket.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <string>
#include <error.h>
#include <sys/epoll.h>
#include <functional>
#include <Classes/Server.h>

#define BUFFER_SIZE 512

using namespace std;

//Server class
Server* server = new Server;
//Sample message
char buffer[] = "to gang z albanii otwieraj drzwi";

//Functions
void createServerSocket(int argc, char **argv);
void closeServer();
void writeData(int fd, char *buffer, int count);
int readData(int fd, char *buffer, int buffsize);
void handleEpollEvents();


//Lambdas
std::function<void()> handleServer = [] () {
    writeData(1, buffer, sizeof(buffer));

    int new_connection = accept(server->fd, NULL, NULL);
    printf("New connection noticed with socket: %d \n", new_connection);

//    char tempBuffer[5];
//    int bytes = readData(new_connection, tempBuffer, sizeof(tempBuffer));
//    string login(tempBuffer);

//    if (usersMap.find())
//    server->getUsersMap().find()
    map<std::string, int> mapka = server->getUsersMap();
    server->getUsersMap().insert(pair<string, int>("login", new_connection));

    printf("To jest w srodku: %d , A tego nie ma: %d \n",server->getUsersMap()["login"], server->getUsersMap()["login111"]);

    writeData(new_connection, buffer, sizeof(buffer));
};

int main(int argc, char **argv) {
    createServerSocket(argc, argv);

    int res = listen(server->fd, SOMAXCONN);
    if(res)
        error(1, errno, "Failed to execute 'listen'\n");

    server->setEpollFd(epoll_create1(0));
    epoll_event ee {EPOLLIN, {.ptr=&handleServer}};
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
            closeServer();
            error(1, errno, "Function epoll_wait failed\n");
        }
            if (ee.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                error(1, errno, "Internal server error. Please contact developers ASAP!\n");
                closeServer();
                exit(0);
            }
            if (ee.events & EPOLLIN) {
                (*(std::function<void()>*)ee.data.ptr)();
            }
    }

}

void closeServer(){
    close(server->fd);
}

int readData(int fd, char * buffer, int buffsize){
    int bytes = read(fd, buffer, buffsize);
    if(bytes == -1) error(1, errno, "Read failed on descriptor %d\n", fd);
    return bytes;
}

void writeData(int fd, char * buffer, int count){
    int bytes = write(fd, buffer, count);
    if(bytes == -1) error(1, errno, "Write failed on descriptor %d\n", fd);
    if(bytes != count) error(0, errno, "Wrote less than requested to descriptor %d (%d/%d)\n", fd, count, bytes);
}

