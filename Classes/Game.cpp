//
// Created by dejmian on 23.01.2020.
//

#include <Utils/utils.h>
#include <thread>
#include <cstring>
#include "Game.h"
#include "Server.h"

int Game::round;

char startMessage[] = "The new game has started !!!";
char drawMessage[] = "Drawing new letter ...";
char letterMessage[] = "New letter is : ";
char roundMessage[] = "Starting round: ";
char endMessage[] = "The game has ended. Not enough players to to continue :( ";

Game::Game() {
    printf("++++ Starting new game ... \n");
    clearClientsPoints();
    std::thread t1(&Game::run, this);
    t1.detach();
}

Game::~Game() {
    clearClientsPoints();
    printf("++++ The game has ended ... \n");
}

void Game::run() {
    srand (time(NULL));
    char buffer[BUFFER_SIZE];
    std::string tempString;

//    Sending greetings
    Server::sendToAllClients(startMessage);
    while (Server::getUsersMap().size() >= 2) {
        drawLetter();
        printf("++++ Drawn letter: %c ... \n", letter);

//        Sending letter message
        tempString = std::string(letterMessage);
        tempString += letter;
        strcpy(buffer, tempString.c_str());
        Server::sendToAllClients(buffer);

//        Sending round number (to user view)
        tempString = std::string(roundMessage) + std::to_string(++round);
        strcpy(buffer, tempString.c_str());
        Server::sendToAllClients(buffer);

        sleep(SERVER_ROUND_TIME);

        printf("++++ End of round: %d ... \n", round);

    }

    //        Sending farewell mesage (to user view)
    Server::sendToAllClients(endMessage);
    delete this;
}


// Getters and Setters
int Game::getRound() {
    return round;
}

void Game::setRound(int _round) {
    Game::round = _round;
}

//private
void Game::clearClientsPoints() {
    round = 0;
    for (const auto &kv : Server::getUsersMap()) {
        kv.second->lastAnswers.clear();
        kv.second->lastScore.clear();
        kv.second->setScore(0);
    }
}

void Game::drawLetter() {
    letter = rand() % 26 + 97;
}
