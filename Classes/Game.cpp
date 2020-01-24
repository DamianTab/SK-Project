//
// Created by dejmian on 23.01.2020.
//

#include <Utils/utils.h>
#include <thread>
#include <cstring>
#include "Game.h"
#include "Server.h"
#include <thread>
#include <mutex>
#include <csignal>

int Game::round;
std::vector<Client *> Game::clientsRankingByTime;
Game* Game::gameInstance;

char startMessage[] = "The new game has started !!!";
char letterMessage[] = "New letter is : ";
char roundMessage[] = "Starting round: ";
char endMessage[] = "The game has ended. Not enough players to to continue :( ";

Game::Game() {
    printf("++++ Starting new game ... \n");
    gameInstance = this;
    clearClientsPoints(true);
//    New thread on this instance
    gameThread = std::thread (&Game::run, this);
    gameThread.detach();
}

Game::~Game() {
    clearClientsPoints(true);
    gameInstance = nullptr;
    printf("++++ The game has ended ... \n");
}

void Game::run() {
    srand(time(NULL));
    char receiveBuffer[BUFFER_SIZE];
    std::string tempString;
    incrementRound();

//    Sending greetings
    Server::sendToAllClients(startMessage);
    mutexClientsMap.lock();
    while (Server::getClientsMap().size() >= MINIMUM_PLAYERS_NUMBER) {
        mutexClientsMap.unlock();
        drawLetter();
        printf("++++ Drawn letter: %c ... Starting new round %d ... \n", letter, getRound());

//        Sending letter message
        tempString = std::string(letterMessage);
        tempString += letter;
        strcpy(receiveBuffer, tempString.c_str());
        Server::sendToAllClients(receiveBuffer);

//        Sending round number (to user view)
        tempString = std::string(roundMessage) + std::to_string(getRound());
        strcpy(receiveBuffer, tempString.c_str());
        Server::sendToAllClients(receiveBuffer);
        printf("++++ Start of round: %d ... \n", getRound());


        sleep(SERVER_ROUND_TIME);
        printf("++++ End of round: %d ... \n", getRound());
//        Ignoring new messages from other threads
        printf("@@@@@@@ DEBUG1: %d ... \n", getRound());
        incrementRound();
        printf("@@@@@@@ DEBUG2: %d ... \n", getRound());

        mutexClientsMap.lock();
        //        Removing inactive clients
        for (const auto &kv : Server::getClientsMap()) {
            if (kv.second->inactiveRoundsNumber >= ROUNDS_NUMBER_TO_REMOVE_INACTIVE_CLIENT){
                printf("++++ Removing inactive player with login: '%s' with rounds inactive: %d\n", kv.first.c_str(), kv.second->inactiveRoundsNumber);
                delete kv.second;
            }else if(kv.second->lastAnswers.size() == 0){
                kv.second->inactiveRoundsNumber++;
            }
        }
        mutexClientsMap.unlock();

        printf("@@@@@@@ DEBUG3: %d ... \n", getRound());

        mutexClientsMap.lock();
//        Calculating results
        float coefficient = 1.0;
        for (auto it = clientsRankingByTime.begin(); it != clientsRankingByTime.end(); ++it) {
            Client *client = *it;
            printf("@@@@@@@ DEBUG31: %d ... \n", getRound());


//            If player has been already disconnected then next client from ranking
            if(! Server::isInsideClientMap(client->getLogin())){
                printf("@@@@@@@ DEBUG331: %d ... \n", getRound());
                continue;
            }

//            This client already has default values (0);
            client->lastScore.clear();

            for (int i = 0; i < (int) client->lastAnswers.size(); ++i) {
                bool isAnswerRepeated = false;
//                If answer is correct
                if (client->lastAnswers[i][0] == letter) {
                    for (const auto &kv : Server::getClientsMap()) {
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
            //todo delete
//            printf("--------------------- Total points: %f of player: '%s'\n", client->getScore(), client->getLogin().c_str());
        }
        mutexClientsMap.unlock();


        printf("@@@@@@@ DEBUG4: %d ... \n", getRound());

        mutexClientsMap.lock();
//        Sending result
        for (const auto &kv : Server::getClientsMap()) {
            kv.second->sendAnswersAndPoints();
            printf("--------------------- Player: '%s' Total Points: %f\n", kv.second->getLogin().c_str(), kv.second->getScore());
        }
        mutexClientsMap.unlock();

        printf("@@@@@@@ DEBUG5: %d ... \n", getRound());

        clearClientsPoints();

        mutexClientsMap.lock();
    }
    mutexClientsMap.unlock();

    printf("@@@@@@@ DEBUG6: %d ... \n", getRound());

//    Sending farewell mesage (to user view)
    Server::sendToAllClients(endMessage);
    printf("%s\n", endMessage);
    delete this;
}


void Game::pushClientToTimeRankingWhenPossible(Client *client) {
    if (clientsRankingByTime.size() < 10) {
        printf("@@@@@@@ DEBUG-PUSH: %d ... \n", getRound());
        clientsRankingByTime.push_back(client);
    }
}

// Getters and Setters
int Game::getRound() {
//    std::scoped_lock lock{mutexRound};
    return round;
}

void Game::incrementRound() {
//    std::scoped_lock lock{mutexRound};
    round++;
}

void Game::setRound(int _round) {
//    std::scoped_lock lock{mutexRound};
    Game::round = _round;
}

//private
void Game::clearClientsPoints(bool shouldClearTotalPointsAndRound) {
    std::scoped_lock lock{mutexClientsMap};
    clientsRankingByTime.clear();
    for (const auto &kv : Server::getClientsMap()) {
        kv.second->lastAnswers.clear();
        kv.second->lastScore.clear();
        if (shouldClearTotalPointsAndRound) {
            kv.second->setScore(0);
        }
    }
    if (shouldClearTotalPointsAndRound) {
        setRound(0);
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