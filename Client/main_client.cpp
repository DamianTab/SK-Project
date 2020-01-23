#include <iostream>
#include <sys/socket.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <error.h>
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

    char duzybufor[BUFFER_SIZE];
    int x = readData(serverSocket, duzybufor, &round);
    perror("Reading from socket");
    printf("\n%d Data: %s TO JEST ROUND %d\n", x, duzybufor, round);

    char messageBuffer[] = "MojLogin";
    writeData(serverSocket, messageBuffer, round);

    printf("-----SLEEP\n");
    sleep(1);

    char messageBuffer1[] = "LOGIN2";
    writeData(serverSocket, messageBuffer1, round);

    sleep(10);

//    TERAZ PROBA WYSLANIA ODPOWIEDZI NA LITERKE




//    duzybufor[BUFFER_SIZE] = {0};
//    x = readData(serverSocket, duzybufor, &round);
//    perror("Reading from socket");
//    printf("\n%d Data: %s TO JEST ROUND %d\n", x, duzybufor, round);
}