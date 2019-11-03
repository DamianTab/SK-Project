#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
 
#define BUFFER_SIZE 512
 
using namespace std;
 
int main()
{
    sockaddr_in adres;
    char duzybufor[BUFFER_SIZE];
   
    inet_aton("127.0.0.1", &adres.sin_addr);
        adres.sin_port = htons(3000);
        adres.sin_family = PF_INET;

    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    connect(sock, (sockaddr*)&adres, sizeof(adres));
    
    int x = read(sock, duzybufor, BUFFER_SIZE);
    perror("Reading from socket");

    printf("%d Data: %s\n",x, duzybufor);
    return 0;
}