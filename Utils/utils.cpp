//
// Created by dejmian on 05.01.2020.
//

#include <cstdio>
#include <cstring>
#include "utils.h"
#include "string"


int readData(int fd, char *buffer, int *round) {
    int bytes = read(fd, buffer, sizeof(buffer));
    if (bytes == -1) error(1, errno, "Read failed on descriptor %d\n", fd);
    return bytes;
}

void writeData(int fd, char *buffer, int round) {

    char header[HEADER_SIZE] = {'0', '0', '0', '0', '0', '0', ':', ':'};
    char message [strlen(buffer) + HEADER_SIZE];

    std::string tempStr = std::to_string(round);
    char const *pchar = tempStr.c_str();  //use char const* as target type
//    printf("TO JEST WLASNIE INT: %s\n", pchar);
//    printf("TO JEST wielkosc INT: %zu\n", strlen(pchar));
//    printf("TO JEST PRZED header: %s\n", header);
//    printf("TO JEST HEADER: %zu\n", strlen(header));
    for (int i = 0; i < strlen(pchar); ++i) {
        header[HEADER_SIZE-2 + i - strlen(pchar)] = pchar[i];
    }
    printf("TO JEST PO header: %s\n", header);

    strcpy(message, header);
    strcat(message, buffer);
    printf("TO JEST WLASNIE WIADOMOSC: %s\n", message);
    printf("TO JEST wielkosc WIADOMOSCI: %zu\n", strlen(message));
    int bytes = write(fd, message, strlen(message));
    if (bytes == -1) error(1, errno, "Write failed on descriptor %d\n", fd);
    if (bytes != strlen(message)) error(0, errno, "Wrote less than requested to descriptor %d (%d/%zu)\n", fd, bytes, strlen(message));
}