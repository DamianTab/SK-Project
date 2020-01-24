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
    string message;


    char buffer[BUFFER_SIZE];
    int bytes = readData(serverSocket, buffer, &round);
    message = std::string(buffer);
    message = message.substr(0, bytes);
    printf("%s \n", message.c_str());

    bytes = readData(serverSocket, buffer, &round);
    message = std::string(buffer);
    message = message.substr(0, bytes);
    printf("%s \n", message.c_str());

    char messageBuffer[] = "MojLogin";
    writeData(serverSocket, messageBuffer, round);

    bytes = readData(serverSocket, buffer, &round);
    message = std::string(buffer);
    message = message.substr(0, bytes);
    printf("%s", message.c_str());

    printf("----- POPRAWNA AUTORYZACJA\n");

//    PO ROZPOCZECIU GRY --- PROBA WYSLANIA ODPOWIEDZI NA LITERKE

    bytes = readData(serverSocket, buffer, &round);
    message = std::string(buffer);
    message = message.substr(0, bytes);
    printf("%s (ROUND '%d')\n", message.c_str(), round);

    while (1){
        for (int i = 0; i < 2; ++i) {
            bytes = readData(serverSocket, buffer, &round);
            message = std::string(buffer);
            message = message.substr(0, bytes);
            printf("%s (ROUND '%d')\n", message.c_str(), round);
        }
//        char messageBuffer[] = "PAństwo,    miaSTO,rzeka,   imię   ,loląń, CO KOLWIEK";
        char messageBuffer[] = "PAństwo,    miaSTO,rzeka,   imię   ";
        writeData(serverSocket, messageBuffer, round);

        bytes = readData(serverSocket, buffer, &round);
        message = std::string(buffer);
        message = message.substr(0, bytes);
        printf("+++ WYNIKI: %s \n", message.c_str());
    }
}