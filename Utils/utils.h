//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_UTILS_H
#define SK_PROJECT_UTILS_H

#include <zconf.h>
#include <cerrno>
#include <error.h>

#define BUFFER_SIZE 255

int readData(int fd, char * buffer, int buffsize);
void writeData(int fd, char * buffer, int count);


#endif //SK_PROJECT_UTILS_H


