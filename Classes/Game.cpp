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
    round ++;

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
        tempString = std::string(roundMessage) + std::to_string(round);
        strcpy(receiveBuffer, tempString.c_str());
        Server::sendToAllClients(receiveBuffer);

        sleep(SERVER_ROUND_TIME);
        printf("++++ End of round: %d ... \n", round);
//        To avoid changes by other threads
        round ++;

        float coefficient = 1.0;
        for (auto it = clientsRankingByTime.begin(); it != clientsRankingByTime.end(); ++it) {
            Client *client = *it;

//            This client already has default values (0);
            client->lastScore.clear();

            for (int i = 0; i < (int) client->lastAnswers.size(); ++i) {
                bool isAnswerRepeated = false;
//                If answer is correct
                if (client->lastAnswers[i][0] == letter) {
//                    printf("TAK JEST ROWNE LITERCE %s\n", client->lastAnswers[i].c_str());
                    for (const auto &kv : Server::getUsersMap()) {
//                        Have to be other client
                        if (kv.second == client) continue;
//                        If other client has the same answer
                        if (kv.second->lastAnswers[i] == client->lastAnswers[i]) {
                            isAnswerRepeated = true;
                            break;
                        }
                    }
                    if (isAnswerRepeated) {
                        client->lastScore.push_back(CORRECT_REPEATED_ANSWER_POINTS * coefficient);
                    } else {
                        client->lastScore.push_back(CORRECT_ANSWER_POINTS * coefficient);
                    }
//                    If answer is incorrect
                } else {
                    client->lastScore.push_back(0);
                }
            }
            coefficient -= 0.1;
            client->recalculateTotalScore();
            printf("-----------------------OGOLNA LICZBA PUNKTOW %f dla gracza: '%s'\n", client->getScore(), client->getLogin().c_str());
        }

//        Sending result
        for (const auto &kv : Server::getUsersMap()) {
            kv.second->sendAnswersAndPoints();
        }

        clearClientsPoints();
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
    clientsRankingByTime.clear();
    for (const auto &kv : Server::getUsersMap()) {
        kv.second->lastAnswers.clear();
        kv.second->lastScore.clear();
        if (shouldClearTotalPoints) {
            kv.second->setScore(0);
            round = 0;
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
