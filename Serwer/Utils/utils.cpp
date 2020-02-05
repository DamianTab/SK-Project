//
// Created by dejmian on 05.01.2020.
//

#include <cstring>
#include <regex>
#include "utils.h"
#include "string"


// jak header jest -1 to jest logowanie
// jak header jest 0 to jest błąd
// jak header >0 to rundy

std::mutex mutexRound;
std::mutex mutexClientsMap;
std::thread gameThread;

int minPlayersNumber = 2;
float sleepWriteToAll = 0.5;
int roundTime = 40;

void readConfig(int players, float sleepTime, int round){
    minPlayersNumber = players;
    sleepWriteToAll = sleepTime;
    roundTime = round;
}

int readData(int fd, char *buffer, int *roundPtr) {

    char tempBuffer[BUFFER_SIZE];
    int bytes = read(fd, tempBuffer, sizeof(tempBuffer));
    if (bytes == -1) error(0, errno, "Read failed on descriptor %d\n", fd);
    std::string str(tempBuffer);
    std::string header = str.substr(0, HEADER_SIZE);
    std::string message = str.substr(HEADER_SIZE, strlen(tempBuffer) - HEADER_SIZE);

//    Delete '0' before number of game roundPtr
    while (header[0] == '0') {
        header.erase(header.begin());
    }

    if (header[0] != ':') {
        //    Delete :: after number
        header.erase(header.size() - 2, 2);
        //    Return roundPtr value by pointer
        *roundPtr = std::stoi(header);
    } else {
        *roundPtr = 0;
    }
    strcpy(buffer, message.c_str());
    return bytes - HEADER_SIZE;
}

void writeData(int fd, char *buffer, int roundValue, bool shouldSleep) {

    char header[HEADER_SIZE] = {'0', '0', '0', '0', '0', '0', ':', ':'};
    char message[strlen(buffer) + HEADER_SIZE + 1];
    std::string tempStr = std::to_string(roundValue);
    char const *pchar = tempStr.c_str();  //use char const* as target type
    for (int i = 0; i < (int) strlen(pchar); ++i) {
        header[HEADER_SIZE - 2 + i - strlen(pchar)] = pchar[i];
    }
    strcpy(message, header);
    strcat(message, buffer);
    strcat(message, "\n");
    int bytes = write(fd, message, strlen(message));
    if (bytes == -1) error(0, errno, "Write failed on descriptor %d\n", fd);
    if (bytes != (int) strlen(message))
        error(0, errno, "Wrote less than requested to descriptor %d (%d/%zu)\n", fd, bytes, strlen(message));
    if (shouldSleep) {
        sleep(SLEEP_WRITE);
    }
}

bool isCorrectRound(int expected, int actual) {
    if (actual == 0) {
        error(0, errno, "Critical Error ! Round number is 0. Communication between sockets is broken !");
        return false;
    } else if (expected != actual) {
        error(0, errno, "Error ! Round number not matching actual value. Received %d but actual is %d.", actual,
              expected);
        return false;
    }
    return true;
}

std::string removeLeadingAndTrailingSpaces(std::string phrase) {
    phrase = std::regex_replace(phrase, std::regex("^ +"), "");
    phrase = std::regex_replace(phrase, std::regex(" +$"), "");
    return phrase;
}

void toLower(std::string *phrase) {
    std::transform(phrase->begin(), phrase->end(), phrase->begin(), ::tolower);
}

std::vector<std::string> extractPhrase(std::string phrase, std::string delimiter) {

    std::vector<std::string> vec;
    std::string tempString;
    size_t pos = 0;

    toLower(&phrase);

    while (vec.size() < GAME_WORDS_AMOUNT && (pos = phrase.find(delimiter)) != std::string::npos) {
        tempString = phrase.substr(0, pos);
        tempString = removeLeadingAndTrailingSpaces(tempString);
        vec.push_back(tempString);
        phrase.erase(0, pos + delimiter.length());
    }
//    If message words = GAME_WORDS_AMOUNT (to not omit last word)
    if (vec.size() < GAME_WORDS_AMOUNT) {
        phrase = removeLeadingAndTrailingSpaces(phrase);
        vec.push_back(phrase);
    }
    return vec;
}
