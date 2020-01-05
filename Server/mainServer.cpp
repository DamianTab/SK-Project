#include <sys/socket.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <string>
#include <error.h>
#include <sys/epoll.h>
#include <functional>

#define BUFFER_SIZE 512

using namespace std;

// server socket
int serverFd;
// Epoll socket
int epollFd;
// Map with user login and socket (FD)
map<string, int> usersMap;
//Sample message
char buffer[] = "to gang z albanii otwieraj drzwi";

//Functions
void createServerSocket(int argc, char **argv);
void closeServer();
void writeData(int fd, char *buffer, int count);
int readData(int fd, char *buffer, int buffsize);
void handleEpollEvents(int epollFd);


//Lambdas
std::function<void()> handleServer = [] () -> const auto {
    writeData(1, buffer, sizeof(buffer));

    int new_connection = accept(serverFd, NULL, NULL);
    printf("New connection noticed with socket: %d \n", new_connection);

//    char tempBuffer[5];
//    int bytes = readData(new_connection, tempBuffer, sizeof(tempBuffer));
//    string login(tempBuffer);

//    if (usersMap.find())
    usersMap.insert(pair<string, int>("login", new_connection));
    printf("To jest w srodku: %d , A tego nie ma: %d \n",usersMap["login"], usersMap["login111"]);

    writeData(new_connection, buffer, sizeof(buffer));
};

std::function<void()> handleClient = [] () {

};

int main(int argc, char **argv) {
    createServerSocket(argc, argv);

    int res = listen(serverFd, SOMAXCONN);
    if(res)
        error(1, errno, "Failed to execute 'listen'\n");

    epollFd = epoll_create1(0);
    epoll_event ee {EPOLLIN, {.ptr=&handleServer}};
//    ee.data.fd = serverFd;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &ee);

    handleEpollEvents(epollFd);

    closeServer();
    return 0;
}

void createServerSocket(int argc, char **argv){
    if (argc != 3)
        error(1, 0, "To correctly run type: %s <IP adress> <port>\n", argv[0]);
    sockaddr_in serverAddress{
            .sin_family = AF_INET,
            .sin_port   = htons(atoi(argv[2])),
            .sin_addr   = {inet_addr(argv[1])}
    };
    serverFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverFd == -1)
        error(1, errno, "Failed to create server socket\n");

//    Only allows to faster use the same port - can be deleted later
    const int one = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    if(bind(serverFd, (sockaddr*) &serverAddress, sizeof(serverAddress)))
        error(1,errno,"Failed to bind server address!\n");
}

void handleEpollEvents(int epollFd){
    epoll_event ee;
    while(1) {
        if (-1 == epoll_wait(epollFd, &ee, 1, -1)) {
            closeServer();
            error(1, errno, "Function epoll_wait failed\n");
        }
//        Server
//        if(ee.data.fd == serverFd){
//            Server error
            if (ee.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                error(1, errno, "Internal server error. Please contact developers ASAP!\n");
                closeServer();
                exit(0);
            }
//            New connection
            if (ee.events & EPOLLIN) {
                (*(std::function<void()>*)ee.data.ptr)();
//                int new_connection = accept(serverFd, NULL, NULL);
//                writeData(new_connection, buffer, sizeof(buffer));
            }
//         Clients
//        }else {
//
//        }
    }

}

void closeServer(){
    close(serverFd);
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

