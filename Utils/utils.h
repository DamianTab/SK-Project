//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_UTILS_H
#define SK_PROJECT_UTILS_H

#include <zconf.h>
#include <cerrno>
#include <error.h>
#include <string>


#define BUFFER_SIZE 255
#define HEADER_SIZE 8
#define SERVER_ROUND_TIME 10
#define SLEEP_WRITE 0.1
#define SLEEP_WRITE_TO_ALL 0.4
#define CONNECTION_ROUND_VALUE -1
#define MINIMUM_PLAYERS_NUMBER 1
#define GAME_WORDS_AMOUNT 4
#define DEFAULT_DELIMITER ","


int readData(int fd, char *buffer, int *round);

void writeData(int fd, char *buffer, int round, bool shouldSleep = true);

bool isCorrectRound(int expected, int actual);

void extractPhrase(std::string phrase, std::string *buffer, std::string delimiter = DEFAULT_DELIMITER);

// Removing leading and trailing spaces
std::string removeLeadingAndTrailingSpaces(std::string phrase);

void toLower(std::string *phrase);

#endif //SK_PROJECT_UTILS_H


