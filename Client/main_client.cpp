#include <iostream>
#include <sys/socket.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <error.h>
#include <cstring>
#include <Utils/utils.h>
using namespace std;

int main(int argc, char **argv) {
    char duzybufor[BUFFER_SIZE];

    if (argc != 3)
        error(1, 0, "To correctly run type: %s <IP adress> <port>", argv[0]);

    sockaddr_in serverAdress;
    inet_aton(argv[1], &serverAdress.sin_addr);
    serverAdress.sin_port = htons(atoi(argv[2]));
    serverAdress.sin_family = PF_INET;

    int serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(serverSocket, (sockaddr *) &serverAdress, sizeof(serverAdress));

    std::string response;
    int bytes;

    do {
        char messageBuffer[BUFFER_SIZE];
        bytes = readData(serverSocket, messageBuffer, sizeof(messageBuffer));
        writeData(1, messageBuffer, bytes);

        string login;
        cin >> login;
        char tempBuffer[login.size()+1];
        strcpy(tempBuffer, login.c_str());
        writeData(serverSocket, tempBuffer, sizeof(tempBuffer));

        bytes = readData(serverSocket, messageBuffer, sizeof(messageBuffer));
        string response(messageBuffer);

        //todo usunac
        writeData(1, messageBuffer, sizeof(messageBuffer));
    } while (response != "Success");

//    int x = read(serverSocket, duzybufor, BUFFER_SIZE);
//    perror("Reading from socket");
//    printf("\n%d Data: %s\n",x, duzybufor);

    shutdown(serverSocket, SHUT_RDWR);
    close(serverSocket);
    return 0;
}