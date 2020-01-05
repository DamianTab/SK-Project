//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_UTILS_H
#define SK_PROJECT_UTILS_H

#include <zconf.h>
#include <cerrno>
#include <error.h>

int readData(int fd, char * buffer, int buffsize){
    int bytes = read(fd, buffer, buffsize);
    if(bytes == -1) error(1, errno, "Read failed on descriptor %d\n", fd);
    return bytes;
}

void writeData(int fd, char * buffer, int count){
    int bytes = write(fd, buffer, count);
    if(bytes == -1) error(1, errno, "Write failed on descriptor %d\n", fd);
    if(bytes != count) error(0, errno, "Wrote less than requested to descriptor %d (%d/%d)\n", fd, count, bytes);
}


#endif //SK_PROJECT_UTILS_H


