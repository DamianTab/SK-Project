//
// Created by dejmian on 05.01.2020.
//

#pragma once

//#include <zconf.h>
#include <cerrno>
#include <error.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>


#define BUFFER_SIZE 255
#define HEADER_SIZE 8
//#define roundTime 40                // do configu
#define SLEEP_WRITE 0.5
//#define sleepWriteToAll 0.5              // do configu
#define CONNECTION_ROUND_VALUE -1
//#define minPlayersNumber 2            // do configu
#define GAME_WORDS_AMOUNT 4
#define DEFAULT_DELIMITER ","
#define CORRECT_ANSWER_POINTS 15
#define CORRECT_REPEATED_ANSWER_POINTS 10
#define ROUNDS_NUMBER_TO_REMOVE_INACTIVE_CLIENT 2

extern int roundTime;
extern int minPlayersNumber;
extern float sleepWriteToAll;


extern std::mutex mutexRound;
extern std::mutex mutexClientsMap;
extern std::thread gameThread;

void readConfig(int players, float sleepTime, int round);

int readData(int fd, char *buffer, int *roundPtr);

void writeData(int fd, char *buffer, int roundValue, bool shouldSleep = true);

bool isCorrectRound(int expected, int actual);

std::vector<std::string> extractPhrase(std::string phrase, std::string delimiter = DEFAULT_DELIMITER);

// Removing leading and trailing spaces
std::string removeLeadingAndTrailingSpaces(std::string phrase);

void toLower(std::string *phrase);

//#endif //SK_PROJECT_UTILS_H


