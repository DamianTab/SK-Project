#include <iostream>
#include <sys/socket.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <error.h>
#include <cstring>

#define BUFFER_SIZE 512

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
    connect(serverSocket, (sockaddr*)&serverAdress, sizeof(serverAdress));

//    Wczytywanie loginu
//    string login;
//    cin>>login;
//    char buffer[login.size()+1];
//    strcpy(buffer, login.c_str());
//    cout<<login<<endl;
//    write(serverSocket, buffer, sizeof(buffer));
//    cout<<"Dziala"<<endl;

    int x = read(serverSocket, duzybufor, BUFFER_SIZE);
    perror("Reading from socket");
    printf("\n%d Data: %s\n",x, duzybufor);

//    while(true);

    shutdown(serverSocket, SHUT_RDWR);
    close(serverSocket);
    return 0;
}