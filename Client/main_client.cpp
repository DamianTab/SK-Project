#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>

#define BUFFER_SIZE 512
 
using namespace std;

int main(int argc, char **argv) {
    char duzybufor[BUFFER_SIZE];

    if (argc != 3)
        error(1, 0, "To correctly run type: %s <IP adress> <port>", argv[0]);

    sockaddr_in clientAdress;
    inet_aton(argv[1], &clientAdress.sin_addr);
    clientAdress.sin_port = htons(atoi(argv[2]));
    clientAdress.sin_family = PF_INET;

    int clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(clientSocket, (sockaddr*)&clientAdress, sizeof(clientAdress));
    
    int x = read(clientSocket, duzybufor, BUFFER_SIZE);
    perror("Reading from socket");
    printf("\n%d Data: %s\n",x, duzybufor);

    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
    return 0;
}