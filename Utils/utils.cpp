//
// Created by dejmian on 05.01.2020.
//

#include <cstring>
#include <regex>
#include "utils.h"
#include "string"


int readData(int fd, char *buffer, int *round) {

    //todo delete printf

    char tempBuffer[BUFFER_SIZE];
    int bytes = read(fd, tempBuffer, sizeof(tempBuffer));
    if (bytes == -1) error(0, errno, "Read failed on descriptor %d\n", fd);
//    printf("TO JEST WLASNIE WIADOMOSC: %s\n", tempBuffer);
    std::string str(tempBuffer);
    std::string header = str.substr(0, HEADER_SIZE);
    std::string message = str.substr(HEADER_SIZE, strlen(tempBuffer) - HEADER_SIZE);

//    Delete '0' before number of game round
    while (header[0] == '0') {
        header.erase(header.begin());
    }

    if (header[0] != ':') {
        //    Delete :: after number
        header.erase(header.size() - 2, 2);
        //    Return round value by pointer
        *round = std::stoi(header);
    } else {
        *round = 0;
    }


//    printf("TO JEST WLASNIE WIADOMOSC: %s\n", str.c_str());
//    printf("TO JEST wielkosc WIADOMOSCI: %zu\n", strlen(str.c_str()));
//    printf("TO JEST WLASNIE WIADOMOSC: %s\n", header.c_str());
//    printf("TO JEST wielkosc WIADOMOSCI: %zu\n", strlen(header.c_str()));
//    printf("TO JEST WLASNIE WIADOMOSC: %s\n", message.c_str());
//    printf("TO JEST wielkosc WIADOMOSCI: %zu\n", strlen(message.c_str()));

    strcpy(buffer, message.c_str());
    return bytes - HEADER_SIZE;
}

void writeData(int fd, char *buffer, int round, bool shouldSleep) {

    char header[HEADER_SIZE] = {'0', '0', '0', '0', '0', '0', ':', ':'};
    char message[strlen(buffer) + HEADER_SIZE];

    std::string tempStr = std::to_string(round);
    char const *pchar = tempStr.c_str();  //use char const* as target type
//    printf("TO JEST WLASNIE INT: %s\n", pchar);
//    printf("TO JEST wielkosc INT: %zu\n", strlen(pchar));
//    printf("TO JEST PRZED header: %s\n", header);
//    printf("TO JEST HEADER: %zu\n", strlen(header));
    for (int i = 0; i < (int) strlen(pchar); ++i) {
        header[HEADER_SIZE - 2 + i - strlen(pchar)] = pchar[i];
    }
//    printf("TO JEST PO header: %s\n", header);
    strcpy(message, header);
    strcat(message, buffer);
//    printf("TO JEST WLASNIE WIADOMOSC: %s\n", message);
//    printf("TO JEST wielkosc WIADOMOSCI: %zu\n", strlen(message));
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
        error(0, errno, "Critical Error ! Round number is 0. Communication between sockets is broken !\n");
        return false;
    } else if (expected != actual) {
        error(0, errno, "Error ! Round number not matching actual value. Received %d but actual is %d.\n", actual,
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
//    printf("OTRZYMANA WIADOMOSC OD KLIENTA: %s \n", phrase.c_str());

    while (vec.size() < GAME_WORDS_AMOUNT && (pos = phrase.find(delimiter)) != std::string::npos) {
        tempString = phrase.substr(0, pos);
        tempString = removeLeadingAndTrailingSpaces(tempString);
        vec.push_back(tempString);
        //        printf("Czesc: '%s' numer:%d\n", buffer[i].c_str(),i);
        phrase.erase(0, pos + delimiter.length());
    }
//    If message words = GAME_WORDS_AMOUNT (to not omit last word)
    if (vec.size() < GAME_WORDS_AMOUNT) {
        phrase = removeLeadingAndTrailingSpaces(phrase);
        vec.push_back(phrase);
//        printf("OSTATNIA Czesc: '%s' numer:%d\n", buffer[i].c_str(),i);
    }
    return vec;
}
