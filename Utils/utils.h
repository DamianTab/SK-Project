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
#define SERVER_ROUND_TIME 10
#define SLEEP_WRITE 0.1
#define SLEEP_WRITE_TO_ALL 0.4
#define CONNECTION_ROUND_VALUE -1


int readData(int fd, char * buffer, int* round);
void writeData(int fd, char * buffer, int round, bool shouldSleep = true);
bool isCorrectRound(int expected, int actual);

#endif //SK_PROJECT_UTILS_H


