#include <iostream>
#include <sys/socket.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <error.h>
#include <cstring>
#include <Utils/utils.h>

using namespace std;

int serverSocket;

//Functions
void createSocketAndConnect(int argc, char **argv);
void loginServer();

int main(int argc, char **argv) {
    if (argc != 3)
        error(1, 0, "To correctly run, type server IP and port: %s <IP adress> <port>", argv[0]);

    createSocketAndConnect(argc, argv);

    loginServer();

    shutdown(serverSocket, SHUT_RDWR);
    close(serverSocket);
    return 0;
}


void createSocketAndConnect(int argc, char **argv) {
    sockaddr_in serverAdress;
    inet_aton(argv[1], &serverAdress.sin_addr);
    serverAdress.sin_port = htons(atoi(argv[2]));
    serverAdress.sin_family = PF_INET;

    serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(serverSocket, (sockaddr *) &serverAdress, sizeof(serverAdress));
}

void loginServer() {
    int round = 0;


//    std::string response;
//    int bytes;
//
//    do {
//        char messageBuffer[BUFFER_SIZE];
//        bytes = readData(serverSocket, messageBuffer, sizeof(messageBuffer));
//        writeData(1, messageBuffer, bytes);
//
//        string login;
//        cin >> login;
//        char tempBuffer[login.size()+1];
//        strcpy(tempBuffer, login.c_str());
//        writeData(serverSocket, tempBuffer, sizeof(tempBuffer));
//
//        bytes = readData(serverSocket, messageBuffer, sizeof(messageBuffer));
//        string response(messageBuffer);
//
//        //todo usunac
//        writeData(1, messageBuffer, sizeof(messageBuffer));
//    } while (response != "Success");



    char duzybufor[BUFFER_SIZE];
    int x = readData(serverSocket, duzybufor, &round);
    perror("Reading from socket");
    printf("\n%d Data: %s TO JEST ROUND %d\n", x, duzybufor, round);

    char messageBuffer[] = "To jest wiadomosc dla serwera ";
    writeData(serverSocket, messageBuffer, round);

    printf("-----SLEEP\n");
    sleep(3);

    char messageBuffer1[] = "TO JEST NUMER 2";
    writeData(serverSocket, messageBuffer1, round);

    sleep(1);


//    duzybufor[BUFFER_SIZE] = {0};
//    x = readData(serverSocket, duzybufor, &round);
//    perror("Reading from socket");
//    printf("\n%d Data: %s TO JEST ROUND %d\n", x, duzybufor, round);
}