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
    char buffer[] = "to gang z albanii otwieraj drzwi";
    sockaddr_in port;

    port.sin_addr.s_addr = INADDR_ANY;
    port.sin_port = htons(3000);
    port.sin_family = PF_INET;
    
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (sockaddr*)&port, sizeof(port));

    listen(sock, SOMAXCONN);

    int new_connection = accept(sock, NULL, NULL);

    write(new_connection, buffer, sizeof(buffer));

    return 0;
}