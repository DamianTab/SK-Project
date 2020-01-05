#include <sys/socket.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <string>
#include <error.h>

#define BUFFER_SIZE 512

using namespace std;

// server socket
int serverFd;
// Map with user login and socket (FD)
map<string, int> usersMap;
//Sample message
char buffer[] = "to gang z albanii otwieraj drzwi";

//Functions
void createServerSocket(int argc, char **argv);

int main(int argc, char **argv) {

    createServerSocket(argc, argv);

    int res = listen(serverFd, SOMAXCONN);
    if(res)
        error(1, errno, "listen failed");

    int new_connection = accept(serverFd, NULL, NULL);
    printf("To jest nowe połączenie %d \n", new_connection);

    usersMap.insert(pair<string, int>("login", new_connection));

    printf("To jest w srodku: %d , A tego nie ma: %d \n",usersMap["login"], usersMap["login111"]);

    write(new_connection, buffer, sizeof(buffer));

    close(serverFd);
    return 0;
}

void createServerSocket(int argc, char **argv){
    if (argc != 3)
        error(1, 0, "To correctly run type: %s <IP adress> <port>", argv[0]);
    sockaddr_in serverAddress{
            .sin_family = AF_INET,
            .sin_port   = htons(atoi(argv[2])),
            .sin_addr   = {inet_addr(argv[1])}
    };
    serverFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverFd == -1)
        error(1, errno, "Failed to create server socket");

//    Only allows to faster use the same port - delete later
    const int one = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
//    Delete to this place <<<<<<<<<<<

    if(bind(serverFd, (sockaddr*) &serverAddress, sizeof(serverAddress)))
        error(1,errno,"Failed to bind server address!");
}