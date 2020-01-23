//
// Created by dejmian on 23.01.2020.
//

#include <Utils/utils.h>
#include <thread>
#include <cstring>
#include "Game.h"
#include "Server.h"

int Game::round;
std::vector<Client *> Game::clientsRankingByTime;

char startMessage[] = "The new game has started !!!";
char drawMessage[] = "Drawing new letter ...";
char letterMessage[] = "New letter is : ";
char roundMessage[] = "Starting round: ";
char endMessage[] = "The game has ended. Not enough players to to continue :( ";

Game::Game() {
    printf("++++ Starting new game ... \n");
    clearClientsPoints(true);
    std::thread t1(&Game::run, this);
    t1.detach();
}

Game::~Game() {
    clearClientsPoints(true);
    printf("++++ The game has ended ... \n");
}

void Game::run() {
    srand(time(NULL));
    char receiveBuffer[BUFFER_SIZE];
    std::string tempString;

//    Sending greetings
    Server::sendToAllClients(startMessage);
    while (Server::getUsersMap().size() >= MINIMUM_PLAYERS_NUMBER) {
        drawLetter();
        printf("++++ Drawn letter: %c ... \n", letter);

//        Sending letter message
        tempString = std::string(letterMessage);
        tempString += letter;
        strcpy(receiveBuffer, tempString.c_str());
        Server::sendToAllClients(receiveBuffer);

//        Sending round number (to user view)
        tempString = std::string(roundMessage) + std::to_string(++round);
        strcpy(receiveBuffer, tempString.c_str());
        Server::sendToAllClients(receiveBuffer);

        sleep(SERVER_ROUND_TIME);

        printf("++++ End of round: %d ... \n", round);
    }

//    Sending farewell mesage (to user view)
    Server::sendToAllClients(endMessage);
    delete this;
}


void Game::pushClientToTimeRankingWhenPossible(Client *client) {
    if (clientsRankingByTime.size() < 10) {
        clientsRankingByTime.push_back(client);
    }
}

// Getters and Setters
int Game::getRound() {
    return round;
}

void Game::setRound(int _round) {
    Game::round = _round;
}

//private
void Game::clearClientsPoints(bool shouldClearTotalPoints) {
    round = 0;
    for (const auto &kv : Server::getUsersMap()) {
        kv.second->lastAnswers.clear();
        kv.second->lastScore.clear();
        if (shouldClearTotalPoints) {
            kv.second->setScore(0);
        }
    }
}

void Game::drawLetter() {
    letter = rand() % 26 + 97;
}

// Getters and Setters
char Game::getLetter() const {
    return letter;
}

void Game::setLetter(char letter) {
    Game::letter = letter;
}

std::vector<Client *> &Game::getClientsRankingByTime() {
    return clientsRankingByTime;
}

void Game::setClientsRankingByTime(std::vector<Client *> &clientsRankingByTime) {
    Game::clientsRankingByTime = clientsRankingByTime;
}
