//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_UTILS_H
#define SK_PROJECT_UTILS_H

#include <zconf.h>
#include <cerrno>
#include <error.h>

#define BUFFER_SIZE 255
#define HEADER_SIZE 8
#define SLEEP_TIME 10


int readData(int fd, char * buffer, int* round);
void writeData(int fd, char * buffer, int round);
bool isCorrectRound(int expected, int actual);

#endif //SK_PROJECT_UTILS_H


